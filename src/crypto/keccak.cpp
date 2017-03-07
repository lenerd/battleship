#include <algorithm>
#include <array>
#include <cassert>
#include "keccak.hpp"

Keccak::State::State(permutation_fun permutation, size_t capacity_bytes)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(uint64_t));
    rate_bytes = data.size() * sizeof(uint64_t) - capacity_bytes;
    data.fill(0x0000000000000000);
}

Keccak::State::State(permutation_fun permutation, size_t capacity_bytes, const bytes_t &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(uint64_t));
    rate_bytes = data.size() * sizeof(uint64_t) - capacity_bytes;
    assert(initial.size() == data.size() * sizeof(uint64_t));
    std::copy(initial.cbegin(), initial.cend(), reinterpret_cast<uint8_t*>(data.data()));
}

Keccak::State::State(permutation_fun permutation, size_t capacity_bytes, const std::vector<uint64_t> &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(uint64_t));
    rate_bytes = data.size() * sizeof(uint64_t) - capacity_bytes;
    assert(initial.size() == data.size());
    std::copy(initial.cbegin(), initial.cend(), data.begin());
}

Keccak::State::State(permutation_fun permutation, size_t capacity_bytes, const std::array<uint64_t, 25> &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(uint64_t));
    rate_bytes = data.size() * sizeof(uint64_t) - capacity_bytes;
    std::copy(initial.cbegin(), initial.cend(), data.begin());
}

Keccak::State::State(const State &other)
    : permutation(other.permutation), capacity_bytes(other.capacity_bytes),
    rate_bytes(other.rate_bytes), data(other.data) {}

Keccak::State::State(State &&other)
    : permutation(other.permutation), capacity_bytes(other.capacity_bytes),
    rate_bytes(other.rate_bytes), data(std::move(other.data)) {}

Keccak::State& Keccak::State::operator=(const State &other)
{
    permutation = other.permutation;
    capacity_bytes = other.capacity_bytes;
    rate_bytes = other.rate_bytes;
    data = other.data;
    return *this;
}

Keccak::State& Keccak::State::operator=(State &&other)
{
    permutation = other.permutation;
    capacity_bytes = other.capacity_bytes;
    rate_bytes = other.rate_bytes;
    data = std::move(other.data);
    return *this;
}

void Keccak::State::absorb(const bytes_t &input)
{
    assert(input.size() == rate_bytes);
    absorb(input.begin());
}

void Keccak::State::absorb(bytes_t::const_iterator in_first)
{
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data.data());
    std::transform(data_ptr, data_ptr + rate_bytes, in_first, data.data(),
            [] (auto a, auto b) { return a ^ b; });
    permutation(*this);
}


bytes_t Keccak::State::squeeze()
{
    bytes_t output(rate_bytes);
    squeeze(output.begin());
    return output;
}

void Keccak::State::squeeze(bytes_t::iterator out_first)
{
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data.data());
    std::copy(data_ptr, data_ptr + rate_bytes, out_first);
    permutation(*this);
}

uint64_t& Keccak::State::operator()(size_t x, size_t y)
{
    assert(x < 5 && y < 5);
    return data[x + y*5];
}
const uint64_t& Keccak::State::operator()(size_t x, size_t y) const
{
    assert(x < 5 && y < 5);
    return data[x + y*5];
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


void Keccak::theta(State &state)
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

void Keccak::rho_pi(State &state)
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


void Keccak::chi(State &state)
{
    State tmp{state};
    for (size_t x{0}; x < 5; ++x)
    {
        for (size_t y{0}; y < 5; ++y)
        {
            state(x, y) = tmp(x, y) ^ (~tmp((x+1) % 5, y) & tmp((x+2) % 5, y));
        }
    }
}

void Keccak::iota(State &state, size_t round)
{
    state(0, 0) ^= Keccak::round_constants[round];
}

void Keccak::round(State &state, uint64_t round)
{
    theta(state);
    rho_pi(state);
    chi(state);
    iota(state, round);
}


void Keccak::permutation(State &state)
{
    for (size_t i{0}; i < 24; ++i)
    {
        round(state, i);
    }
}


bytes_t Keccak::keccak(const bytes_t &message)
{
    auto padded{pad(message, rate_bytes)};
    for (auto block_it{padded.cbegin()}; block_it != padded.cend();
            block_it += static_cast<bytes_t::difference_type>(rate_bytes))
    {
        state_.absorb(block_it);
    }
    bytes_t output(output_bytes);
    assert(output_bytes % rate_bytes == 0);
    for (auto block_it{output.begin()}; block_it != output.end();
            block_it += static_cast<bytes_t::difference_type>(rate_bytes))
    {
        state_.squeeze(block_it);
    }
    return output;
}

bytes_t Keccak::pad(const bytes_t &message, size_t blocksize, uint8_t d)
{
    bytes_t padded(message);
    size_t pad_size = blocksize - (message.size() % blocksize);
    padded.resize(message.size() + pad_size);
    auto start_of_padding{std::next(padded.begin(), static_cast<ssize_t>(message.size()))};
    std::fill(start_of_padding, padded.end(), 0x00);
    *start_of_padding ^= d;
    *padded.rbegin() ^= 0x80;

    return padded;
}
