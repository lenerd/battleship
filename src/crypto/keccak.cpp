#ifndef KECCAK_CPP
#define KECCAK_CPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include "keccak.hpp"

template <typename Word>
Keccak<Word>::Keccak(size_t capacity_bits, uint8_t d, size_t output_bits)
        : output_bytes(output_bits / 8),
          capacity_bytes(capacity_bits / 8),
          rate_bytes((1600 - capacity_bits) / 8),
          d(d),
          state_(permutation, capacity_bytes)
{
    assert(capacity_bits % 8 == 0);
    assert(output_bits % 8 == 0);
    assert(capacity_bytes + rate_bytes == 200);
    input_buffer.reserve(rate_bytes);
}


template <typename Word>
void Keccak<Word>::update(const bytes_t &input)
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


template <typename Word>
bytes_t Keccak<Word>::finalize()
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

template <typename Word>
const std::array<Word, 24> Keccak<Word>::round_constants;
template <typename Word>
const std::array<std::array<size_t, 5>, 5> Keccak<Word>::rotation_offsets;

template <typename Word>
Word rotate(Word n, size_t k)
{
    return static_cast<Word>((n << k) | (n >> (sizeof(Word)*8 - k)));
}


template <typename Word>
void Keccak<Word>::theta(KeccakState<Word> &state)
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

template <typename Word>
void Keccak<Word>::rho_pi(KeccakState<Word> &state)
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
    // State tmp{state};
    // for (size_t x{0}; x < 5; ++x)
    // {
    //     for (size_t y{0}; y < 5; ++y)
    //     {
    //         state(y, (2*x+3*y) % 5) = rotate(tmp(x, y), Keccak<Word>::rotation_offsets[x][y]);
    //     }
    // }
}


template <typename Word>
void Keccak<Word>::chi(KeccakState<Word> &state)
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

template <typename Word>
void Keccak<Word>::iota(KeccakState<Word> &state, size_t round)
{
    state(0, 0) ^= Keccak<Word>::round_constants[round];
}

template <typename Word>
void Keccak<Word>::round(KeccakState<Word> &state, uint64_t round)
{
    theta(state);
    rho_pi(state);
    chi(state);
    iota(state, round);
}


template <typename Word>
void Keccak<Word>::permutation(KeccakState<Word> &state)
{
    for (size_t i{0}; i < 24; ++i)
    {
        round(state, i);
    }
}

template <typename Word>
bytes_t Keccak<Word>::pad(const bytes_t &message, size_t blocksize, uint8_t d)
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


inline Keccak<uint64_t> SHA3_256()
{
    return Keccak<uint64_t>(512, 0x06, 256);
}

inline Keccak<uint64_t> SHA3_512()
{
    return Keccak<uint64_t>(1024, 0x06, 512);
}

inline Keccak<uint16_t> Keccak400()
{
    // TODO: fix parameter
    return Keccak<uint16_t>(1024, 0x06, 512);
}

#endif // KECCAK_CPP
