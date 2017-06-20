#ifndef KECCAK_STATE_HPP
#define KECCAK_STATE_HPP

#include <algorithm>
#include <cassert>
#include <vector>
#include <functional>

using bytes_t = std::vector<uint8_t>;

/**
 * State of the sponge construction.
 * The word size is currently fixed at 64 bit.
 */
template <typename Word>
class KeccakState
{
    static_assert(std::is_unsigned<Word>::value, "Unsigned integer required.");
public:
    /**
     * Type of permutation functions.
     */
    using permutation_fun = std::function<void(KeccakState<Word>&)>;
    /**
     * Construct State initialized with zeros.
     */
    KeccakState(permutation_fun permutation, size_t capacity_bytes)
        : permutation(permutation), capacity_bytes(capacity_bytes)
    {
        assert(capacity_bytes < state_bytes);
        rate_bytes = state_bytes - capacity_bytes;
        data.fill(0x0000000000000000);
    }
    /**
     * Construct State initialized with data.
     */
    // TODO: disable bytes_t constructor for Word = uint8_t
    KeccakState(permutation_fun permutation, size_t capacity_bytes, const bytes_t &initial)
        : permutation(permutation), capacity_bytes(capacity_bytes)
    {
        assert(capacity_bytes < state_bytes);
        rate_bytes = state_bytes - capacity_bytes;
        assert(initial.size() == state_bytes);
        std::copy(initial.cbegin(), initial.cend(), reinterpret_cast<uint8_t*>(data.data()));
    }
    KeccakState(permutation_fun permutation, size_t capacity_bytes, const std::vector<Word> &initial)
        : permutation(permutation), capacity_bytes(capacity_bytes)
    {
        assert(capacity_bytes < state_bytes);
        rate_bytes = state_bytes - capacity_bytes;
        assert(initial.size() == data.size());
        std::copy(initial.cbegin(), initial.cend(), data.begin());
    }
    KeccakState(permutation_fun permutation, size_t capacity_bytes, const std::array<Word, 25> &initial)
    : permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < state_bytes);
    rate_bytes = state_bytes - capacity_bytes;
    std::copy(initial.cbegin(), initial.cend(), data.begin());
}

    /**
     * Copy constructor.
     */
    KeccakState(const KeccakState<Word> &other)
        : permutation(other.permutation), capacity_bytes(other.capacity_bytes),
        rate_bytes(other.rate_bytes), data(other.data) {}
    /**
     * Move constructor.
     */
    KeccakState(KeccakState<Word> &&other)
        : permutation(other.permutation), capacity_bytes(other.capacity_bytes),
        rate_bytes(other.rate_bytes), data(std::move(other.data)) {}
    /**
     * Copy assignment.
     */
    KeccakState& operator=(const KeccakState<Word> &other)
    {
        permutation = other.permutation;
        capacity_bytes = other.capacity_bytes;
        rate_bytes = other.rate_bytes;
        data = other.data;
        return *this;
    }
    /**
     * Move assignment.
     */
    KeccakState& operator=(KeccakState<Word> &&other)
    {
        permutation = other.permutation;
        capacity_bytes = other.capacity_bytes;
        rate_bytes = other.rate_bytes;
        data = std::move(other.data);
        return *this;
    }

    /**
     * Absorbs rate_bytes bytes of data into the sponge.
     */
    void absorb(const bytes_t &input)
    {
        assert(input.size() == rate_bytes);
        absorb(input.begin());
    }
    void absorb(bytes_t::const_iterator in_first)
    {
        uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data.data());
        std::transform(data_ptr, data_ptr + rate_bytes, in_first, data_ptr,
                [] (auto a, auto b) { return a ^ b; });
        permutation(*this);
    }
    /**
     * Squeezes rate_bytes bytes from the sponge.
     */
    bytes_t squeeze()
    {
        bytes_t output(rate_bytes);
        squeeze(output.begin());
        return output;
    }
    void squeeze(bytes_t::iterator out_first)
    {
        uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data.data());
        std::copy(data_ptr, data_ptr + rate_bytes, out_first);
        permutation(*this);
    }

    /**
     * Two-dimensional access.
     */
    Word& operator()(size_t x, size_t y)
    {
        assert(x < 5 && y < 5);
        return data[x + y*5];
    }
    const Word& operator()(size_t x, size_t y) const
    {
        assert(x < 5 && y < 5);
        return data[x + y*5];
    }

    /**
     * Chosen permutation.
     */
    permutation_fun permutation;

    /**
     * Size of the state in bytes.
     */
    const static size_t state_bytes = 25 * sizeof(Word);
    /**
     * Number of capacity bytes.
     */
    size_t capacity_bytes;
    /**
     * Number of rate bytes.
     */
    size_t rate_bytes;
    /**
     * Actual state consisting of 25 words.
     */
    std::array<Word, 25> data;
};


#include "keccak_state.cpp"

#endif // KECCAK_STATE_HPP
