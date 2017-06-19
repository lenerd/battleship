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
class Keccak
{
public:

    /**
     * General constructor.
     */
    Keccak(size_t capacity_bits=576, uint8_t d=0x01, size_t output_bits=0);
    /**
     * Constructor for SHA3-256.
     */
    static Keccak SHA3_256();
    /**
     * Constructor for SHA3-512.
     */
    static Keccak SHA3_512();

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
    static void permutation(KeccakState<uint64_t> &state);
    /**
     * Single round of the Keccak-f permutation.
     */
    static void round(KeccakState<uint64_t> &state, uint64_t round);

    /**
     * theta part of the Keccac-f permutation.
     */
    static void theta(KeccakState<uint64_t> &state);
    /**
     * rho and pi part of the Keccac-f permutation.
     */
    static void rho_pi(KeccakState<uint64_t> &state);
    /**
     * chi part of the Keccac-f permutation.
     */
    static void chi(KeccakState<uint64_t> &state);
    /**
     * iota part of the Keccac-f permutation.
     */
    static void iota(KeccakState<uint64_t> &state, size_t round);

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
    const static std::array<uint64_t, 24> round_constants;
    /**
     * Keccak rotation offsets.
     */
    const static std::array<std::array<size_t, 5>, 5> rotation_offsets;

private:
    bytes_t input_buffer;
    KeccakState<uint64_t> state_;
};


#endif // KECCAK_HPP
