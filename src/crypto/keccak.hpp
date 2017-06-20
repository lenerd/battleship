#ifndef KECCAK_HPP
#define KECCAK_HPP

#include <array>
#include <functional>
#include <vector>
#include <iostream>
#include <iomanip>

#include "keccak_state.hpp"

using bytes_t = std::vector<uint8_t>;

template <typename Word>
Word rotate(Word n, size_t k)
{
    auto bit_length{sizeof(Word) * 8};
    k = k % bit_length;
    return static_cast<Word>((n << k) | (n >> (bit_length - k)));
}


/**
 * Implementation of the Keccak family of hash functions.
 */
template <typename Word>
class Keccak
{
    static_assert(std::is_unsigned<Word>::value, "Unsigned integer required.");
public:

    /**
     * General constructor.
     */
    Keccak(size_t capacity_bits=576, uint8_t d=0x01, size_t output_bits=0)
        : output_bytes(output_bits / 8),
          capacity_bytes(capacity_bits / 8),
          rate_bytes(state_bytes - capacity_bytes),
          d(d),
          state_(permutation, capacity_bytes)
    {
        assert(capacity_bits % 8 == 0);
        assert(output_bits % 8 == 0);
        assert(capacity_bytes + rate_bytes == state_.state_bytes);
        input_buffer.reserve(rate_bytes);
    }

    /**
     * Update the state with part of a message.
     */
    void update(const bytes_t& input)
    {
        assert(!finalized);

        if (input_buffer.size() + input.size() < rate_bytes)
        {
            input_buffer.insert(input_buffer.cend(),
                                input.cbegin(),
                                input.cend());
            return;
            
        }

        auto prefix_len{rate_bytes - input_buffer.size()};
        auto suffix_len{(input.size() - prefix_len) % rate_bytes};
        auto next_block_it{input.cbegin() + static_cast<bytes_t::difference_type>(prefix_len)};
        auto suffix_it{input.cend() - static_cast<bytes_t::difference_type>(suffix_len)};

        if (input_buffer.empty())
        {
            next_block_it = input.cbegin();
        }
        else
        {
            input_buffer.insert(input_buffer.cend(),
                                input.cbegin(),
                                next_block_it);
            state_.absorb(input_buffer.cbegin());
            input_buffer.clear();
        }
        for (; next_block_it != suffix_it;
             next_block_it += static_cast<bytes_t::difference_type>(rate_bytes))
        {
            state_.absorb(next_block_it);
        }

        input_buffer.insert(input_buffer.cbegin(), suffix_it, input.cend());
    }
    /**
     * Conclude the hash computation and produce a digest.
     */
    bytes_t finalize()
    {
        assert(!finalized);
        finalized = true;
        bytes_t output(output_bytes);

        // pad content of input_buffer and absorb it
        assert(input_buffer.size() < rate_bytes);
        state_.absorb(pad(input_buffer, rate_bytes, d));

        // squeeze output_bytes
        auto full_blocks{output_bytes / rate_bytes};
        auto extra_bytes{output_bytes % rate_bytes};
        auto last_full_block{std::next(output.begin(), static_cast<bytes_t::difference_type>(full_blocks * rate_bytes))};
        for (auto it{output.begin()}; it != last_full_block; it += static_cast<bytes_t::difference_type>(rate_bytes)) 
        {
            state_.squeeze(it);
        }
        auto tmp{state_.squeeze()};
        std::copy(tmp.cbegin(), std::next(tmp.cbegin(), static_cast<bytes_t::difference_type>(extra_bytes)), last_full_block);

        return output;
    }

    /**
     * Pads a message according to the Keccak padding rules.
     */
    static bytes_t pad(const bytes_t &message, size_t blocksize, uint8_t d=0x01)
    {
        bytes_t padded(message);
        size_t pad_size{blocksize - (message.size() % blocksize)};
        padded.resize(message.size() + pad_size);
        auto start_of_padding{std::next(padded.begin(), static_cast<ssize_t>(message.size()))};
        std::fill(start_of_padding, padded.end(), 0x00);
        *start_of_padding ^= d;
        *padded.rbegin() ^= 0x80;

        return padded;
    }

    /**
     * Keccak-f permutation.
     */
    static void permutation(KeccakState<Word> &state)
    {
        for (size_t i{0}; i < num_rounds; ++i)
        {
            round(state, i);
        }
    }
    /**
     * Single round of the Keccak-f permutation.
     */
    static void round(KeccakState<Word> &state, uint64_t round)
    {
        theta(state);
        rho_pi(state);
        chi(state);
        iota(state, round);
    }

    /**
     * theta part of the Keccac-f permutation.
     */
    static void theta(KeccakState<Word> &state)
    {
        std::array<Word, 5> C;
        Word D;

        for (size_t x{0}; x < 5; ++x)
        {
            C[x] = state(x, 0) ^ state(x, 1) ^ state(x, 2) ^ state(x, 3) ^ state(x, 4);
        }

        for (size_t x{0}; x < 5; ++x)
        {
            D = C[(x+5-1) % 5] ^ rotate(C[(x+1) % 5], 1);
            for (size_t y{0}; y < 5; ++y)
            {
                state(x, y) ^= D;
            }
        }
    }
    /**
     * rho and pi part of the Keccac-f permutation.
     */
    static void rho_pi(KeccakState<Word> &state)
    {
        size_t x = 1;
        size_t y = 0;
        Word current{state(x, y)};
        for (size_t t{0}; t < 24; ++t)
        {
            {
                auto new_y{(2*x + 3*y) % 5};
                x = y;
                y = new_y;
            }
            {
                auto new_lane{rotate(current, (t+1) * (t+2) / 2)};
                current = state(x, y);
                state(x, y) = new_lane;

            }
        }
        // rho(state);
        // pi(state);

    }
    /**
     * rho part of the Keccac-f permutation.
     */
    static void rho(KeccakState<Word> &state)
    {
        for (size_t x{0}; x < 5; ++x)
        {
            for (size_t y{0}; y < 5; ++y)
            {
                state(x, y) = rotate(state(x, y), rotation_offsets[y][x]);
            }
        }
    }
    /**
     * pi part of the Keccac-f permutation.
     */
    static void pi(KeccakState<Word> &state)
    {
        KeccakState<Word> tmp{state};

        for (size_t x{0}; x < 5; ++x)
        {
            for (size_t y{0}; y < 5; ++y)
            {
                state(y, (2*x+3*y) % 5) = tmp(x, y);
            }
        }
    }
    /**
     * chi part of the Keccac-f permutation.
     */
    static void chi(KeccakState<Word> &state)
    {
        KeccakState<Word> tmp{state};
        for (size_t x{0}; x < 5; ++x)
        {
            for (size_t y{0}; y < 5; ++y)
            {
                state(x, y) = tmp(x, y) ^ (~tmp((x+1) % 5, y) & tmp((x+2) % 5, y));
            }
        }
    }
    /**
     * iota part of the Keccac-f permutation.
     */
    static void iota(KeccakState<Word> &state, size_t round)
    {
        state(0, 0) ^= Keccak<Word>::round_constants[round];
    }


    /**
     * Number of rounds in the permutation.
     */
    const static size_t num_rounds;
    /**
     * Size of the state in bytes.
     */
    const static size_t state_bytes = 25 * sizeof(Word);
    /**
     * Size of the output in bytes.
     */
    size_t output_bytes = 32;
    /**
     * Size of the sponge's capacity in bytes.
     */
    size_t capacity_bytes = 72;
    /**
     * Size of the sponge's rate in bytes.
     */
    size_t rate_bytes = 128;
    /**
     * Suffix bits.
     */
    uint8_t d = 0x01;

    /**
     * Is the hash computation complete.
     */
    bool finalized = false;

    /**
     * Keccak round constants.
     */
    const static std::array<Word, 24> round_constants;
    /**
     * Keccak rotation offsets.
     */
    const static std::array<std::array<size_t, 5>, 5> rotation_offsets;

private:
    bytes_t input_buffer;
    KeccakState<Word> state_;
};


template <typename Word>
const std::array<std::array<size_t, 5>, 5> Keccak<Word>::rotation_offsets{{
    {{0, 1, 62, 28, 27}},
    {{36, 44, 6, 55, 20}},
    {{3, 10, 43, 25, 39}},
    {{41, 45, 15, 21, 8}},
    {{18, 2, 61, 56, 14}}
}};

template <typename Word>
const std::array<Word, 24> Keccak<Word>::round_constants{{
    static_cast<Word>(0x0000000000000001),
    static_cast<Word>(0x0000000000008082),
    static_cast<Word>(0x800000000000808A),
    static_cast<Word>(0x8000000080008000),
    static_cast<Word>(0x000000000000808B),
    static_cast<Word>(0x0000000080000001),
    static_cast<Word>(0x8000000080008081),
    static_cast<Word>(0x8000000000008009),
    static_cast<Word>(0x000000000000008A),
    static_cast<Word>(0x0000000000000088),
    static_cast<Word>(0x0000000080008009),
    static_cast<Word>(0x000000008000000A),
    static_cast<Word>(0x000000008000808B),
    static_cast<Word>(0x800000000000008B),
    static_cast<Word>(0x8000000000008089),
    static_cast<Word>(0x8000000000008003),
    static_cast<Word>(0x8000000000008002),
    static_cast<Word>(0x8000000000000080),
    static_cast<Word>(0x000000000000800A),
    static_cast<Word>(0x800000008000000A),
    static_cast<Word>(0x8000000080008081),
    static_cast<Word>(0x8000000000008080),
    static_cast<Word>(0x0000000080000001),
    static_cast<Word>(0x8000000080008008)
}};

/**
 * Constructor for SHA3-256.
 */
inline Keccak<uint64_t> SHA3_256()
{
    return Keccak<uint64_t>(512, 0x06, 256);
}

/**
 * Constructor for SHA3-512.
 */
inline Keccak<uint64_t> SHA3_512()
{
    return Keccak<uint64_t>(1024, 0x06, 512);
}

/**
 * Constructor for Keccak400
 */
inline Keccak<uint16_t> Keccak400()
{
    // TODO: fix parameter
    return Keccak<uint16_t>(256, 0x06, 256);
}

// #include "keccak.cpp"

#endif // KECCAK_HPP
