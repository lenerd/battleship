#ifndef KECCAK_STATE_CPP
#define KECCAK_STATE_CPP

#include <algorithm>
#include <cassert>
#include "keccak_state.hpp"


template <typename Word>
KeccakState<Word>::KeccakState(permutation_fun permutation, size_t capacity_bytes)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(Word));
    rate_bytes = data.size() * sizeof(Word) - capacity_bytes;
    data.fill(0x0000000000000000);
}

template <typename Word>
KeccakState<Word>::KeccakState(permutation_fun permutation, size_t capacity_bytes, const bytes_t &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(Word));
    rate_bytes = data.size() * sizeof(Word) - capacity_bytes;
    assert(initial.size() == data.size() * sizeof(Word));
    std::copy(initial.cbegin(), initial.cend(), reinterpret_cast<uint8_t*>(data.data()));
}

template <typename Word>
KeccakState<Word>::KeccakState(permutation_fun permutation, size_t capacity_bytes, const std::vector<Word> &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(Word));
    rate_bytes = data.size() * sizeof(Word) - capacity_bytes;
    assert(initial.size() == data.size());
    std::copy(initial.cbegin(), initial.cend(), data.begin());
}

template <typename Word>
KeccakState<Word>::KeccakState(permutation_fun permutation, size_t capacity_bytes, const std::array<Word, 25> &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(Word));
    rate_bytes = data.size() * sizeof(Word) - capacity_bytes;
    std::copy(initial.cbegin(), initial.cend(), data.begin());
}

template <typename Word>
KeccakState<Word>::KeccakState(const KeccakState<Word> &other)
    : permutation(other.permutation), capacity_bytes(other.capacity_bytes),
    rate_bytes(other.rate_bytes), data(other.data) {}

template <typename Word>
KeccakState<Word>::KeccakState(KeccakState<Word> &&other)
    : permutation(other.permutation), capacity_bytes(other.capacity_bytes),
    rate_bytes(other.rate_bytes), data(std::move(other.data)) {}

template <typename Word>
KeccakState<Word>& KeccakState<Word>::operator=(const KeccakState<Word> &other)
{
    permutation = other.permutation;
    capacity_bytes = other.capacity_bytes;
    rate_bytes = other.rate_bytes;
    data = other.data;
    return *this;
}

template <typename Word>
KeccakState<Word>& KeccakState<Word>::operator=(KeccakState<Word> &&other)
{
    permutation = other.permutation;
    capacity_bytes = other.capacity_bytes;
    rate_bytes = other.rate_bytes;
    data = std::move(other.data);
    return *this;
}

template <typename Word>
void KeccakState<Word>::absorb(const bytes_t &input)
{
    assert(input.size() == rate_bytes);
    absorb(input.begin());
}

template <typename Word>
void KeccakState<Word>::absorb(bytes_t::const_iterator in_first)
{
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data.data());
    std::transform(data_ptr, data_ptr + rate_bytes, in_first, data_ptr,
            [] (auto a, auto b) { return a ^ b; });
    permutation(*this);
}


template <typename Word>
bytes_t KeccakState<Word>::squeeze()
{
    bytes_t output(rate_bytes);
    squeeze(output.begin());
    return output;
}

template <typename Word>
void KeccakState<Word>::squeeze(bytes_t::iterator out_first)
{
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data.data());
    std::copy(data_ptr, data_ptr + rate_bytes, out_first);
    permutation(*this);
}

template <typename Word>
Word& KeccakState<Word>::operator()(size_t x, size_t y)
{
    assert(x < 5 && y < 5);
    return data[x + y*5];
}
template <typename Word>
const Word& KeccakState<Word>::operator()(size_t x, size_t y) const
{
    assert(x < 5 && y < 5);
    return data[x + y*5];
}

#endif // KECCAK_STATE_CPP
