#include <algorithm>
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
    std::transform(data_ptr, data_ptr + rate_bytes, in_first, data_ptr,
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

