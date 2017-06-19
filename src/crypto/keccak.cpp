#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include "keccak.hpp"

Keccak::Keccak(size_t capacity_bits, uint8_t d, size_t output_bits)
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


Keccak Keccak::SHA3_256()
{
    return Keccak(512, 0x06, 256);
}

Keccak Keccak::SHA3_512()
{
    return Keccak(1024, 0x06, 512);
}


void Keccak::update(const bytes_t &input)
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


bytes_t Keccak::finalize()
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


uint64_t rotate(uint64_t n, size_t k)
{
    return (n << k) | (n >> (64 - k));
}


const std::array<uint64_t, 24> Keccak::round_constants{{
    0x0000000000000001, 0x0000000000008082, 0x800000000000808A,
    0x8000000080008000, 0x000000000000808B, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009, 0x000000000000008A,
    0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
    0x000000008000808B, 0x800000000000008B, 0x8000000000008089,
    0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
    0x000000000000800A, 0x800000008000000A, 0x8000000080008081,
    0x8000000000008080, 0x0000000080000001, 0x8000000080008008
}};

const std::array<std::array<size_t, 5>, 5> Keccak::rotation_offsets{{
    {{0, 1, 62, 28, 27}},
    {{36, 44, 6, 55, 20}},
    {{3, 10, 43, 25, 39}},
    {{41, 45, 15, 21, 8}},
    {{18, 2, 61, 56, 14}}
}};


void Keccak::theta(KeccakState<uint64_t> &state)
{
    std::array<uint64_t, 5> C;
    uint64_t D;

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

void Keccak::rho_pi(KeccakState<uint64_t> &state)
{
    size_t x = 1;
    size_t y = 0;
    uint64_t current{state(x, y)};
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
    //         state(y, (2*x+3*y) % 5) = rotate(tmp(x, y), Keccak::rotation_offsets[x][y]);
    //     }
    // }
}


void Keccak::chi(KeccakState<uint64_t> &state)
{
    KeccakState<uint64_t> tmp{state};
    for (size_t x{0}; x < 5; ++x)
    {
        for (size_t y{0}; y < 5; ++y)
        {
            state(x, y) = tmp(x, y) ^ (~tmp((x+1) % 5, y) & tmp((x+2) % 5, y));
        }
    }
}

void Keccak::iota(KeccakState<uint64_t> &state, size_t round)
{
    state(0, 0) ^= Keccak::round_constants[round];
}

void Keccak::round(KeccakState<uint64_t> &state, uint64_t round)
{
    theta(state);
    rho_pi(state);
    chi(state);
    iota(state, round);
}


void Keccak::permutation(KeccakState<uint64_t> &state)
{
    for (size_t i{0}; i < 24; ++i)
    {
        round(state, i);
    }
}

bytes_t Keccak::pad(const bytes_t &message, size_t blocksize, uint8_t d)
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
