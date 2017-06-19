#ifndef KECCAK_STATE_HPP
#define KECCAK_STATE_HPP

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
    KeccakState(permutation_fun permutation, size_t capacity_bytes);
    /**
     * Construct State initialized with data.
     */
    // TODO: disable bytes_t constructor for Word = uint8_t
    KeccakState(permutation_fun permutation, size_t capacity_bytes, const bytes_t &initial);
    KeccakState(permutation_fun permutation, size_t capacity_bytes, const std::vector<Word> &initial);
    KeccakState(permutation_fun permutation, size_t capacity_bytes, const std::array<Word, 25> &initial);

    /**
     * Copy constructor.
     */
    KeccakState(const KeccakState<Word> &other);
    /**
     * Move constructor.
     */
    KeccakState(KeccakState<Word> &&other);
    /**
     * Copy assignment.
     */
    KeccakState& operator=(const KeccakState<Word> &other);
    /**
     * Move assignment.
     */
    KeccakState& operator=(KeccakState<Word> &&other);

    /**
     * Absorbs rate_bytes bytes of data into the sponge.
     */
    void absorb(const bytes_t &bytes);
    void absorb(bytes_t::const_iterator first);
    /**
     * Squeezes rate_bytes bytes from the sponge.
     */
    bytes_t squeeze();
    void squeeze(bytes_t::iterator out_first);

    /**
     * Two-dimensional access.
     */
    Word& operator()(size_t x, size_t y);
    const Word& operator()(size_t x, size_t y) const;

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
