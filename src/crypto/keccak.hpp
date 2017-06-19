#ifndef KECCAK_HPP
#define KECCAK_HPP

#include <array>
#include <functional>
#include <vector>

#include "keccak_state.hpp"

using bytes_t = std::vector<uint8_t>;


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
    Keccak(size_t capacity_bits=576, uint8_t d=0x01, size_t output_bits=0);

    /**
     * Update the state with part of a message.
     */
    void update(const bytes_t& input);
    /**
     * Conclude the hash computation and produce a digest.
     */
    bytes_t finalize();

    /**
     * Pads a message according to the Keccak padding rules.
     */
    static bytes_t pad(const bytes_t &message, size_t blocksize, uint8_t d=0x01);

    /**
     * Keccak-f permutation.
     */
    static void permutation(KeccakState<Word> &state);
    /**
     * Single round of the Keccak-f permutation.
     */
    static void round(KeccakState<Word> &state, uint64_t round);

    /**
     * theta part of the Keccac-f permutation.
     */
    static void theta(KeccakState<Word> &state);
    /**
     * rho and pi part of the Keccac-f permutation.
     */
    static void rho_pi(KeccakState<Word> &state);
    /**
     * chi part of the Keccac-f permutation.
     */
    static void chi(KeccakState<Word> &state);
    /**
     * iota part of the Keccac-f permutation.
     */
    static void iota(KeccakState<Word> &state, size_t round);

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
    const static std::array<uint64_t, 24> constexpr round_constants{{
        0x0000000000000001, 0x0000000000008082, 0x800000000000808A,
        0x8000000080008000, 0x000000000000808B, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009, 0x000000000000008A,
        0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
        0x000000008000808B, 0x800000000000008B, 0x8000000000008089,
        0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
        0x000000000000800A, 0x800000008000000A, 0x8000000080008081,
        0x8000000000008080, 0x0000000080000001, 0x8000000080008008
    }};
    /**
     * Keccak rotation offsets.
     */
    const static std::array<std::array<size_t, 5>, 5> constexpr rotation_offsets{{
        {{0, 1, 62, 28, 27}},
        {{36, 44, 6, 55, 20}},
        {{3, 10, 43, 25, 39}},
        {{41, 45, 15, 21, 8}},
        {{18, 2, 61, 56, 14}}
    }};

private:
    bytes_t input_buffer;
    KeccakState<Word> state_;
};

/**
 * Constructor for SHA3-256.
 */
Keccak<uint64_t> SHA3_256();
/**
 * Constructor for SHA3-512.
 */
Keccak<uint64_t> SHA3_512();

#include "keccak.cpp"

#endif // KECCAK_HPP
