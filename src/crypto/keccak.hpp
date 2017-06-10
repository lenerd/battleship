#ifndef KECCAK_HPP
#define KECCAK_HPP

#include <array>
#include <functional>
#include <vector>

using bytes_t = std::vector<uint8_t>;


/**
 * Implementation of the Keccak family of hash functions.
 */
class Keccak
{
public:
    /**
     * State of the sponge construction.
     * The word size is currently fixed at 64 bit.
     */
    class State
    {
    public:
        /**
         * Type of permutation functions.
         */
        using permutation_fun = std::function<void(State&)>;
        /**
         * Construct State initialized with zeros.
         */
        State(permutation_fun permutation, size_t capacity_bytes);
        /**
         * Construct State initialized with data.
         */
        State(permutation_fun permutation, size_t capacity_bytes, const bytes_t &initial);
        State(permutation_fun permutation, size_t capacity_bytes, const std::vector<uint64_t> &initial);
        State(permutation_fun permutation, size_t capacity_bytes, const std::array<uint64_t, 25> &initial);

        /**
         * Copy constructor.
         */
        State(const State &other);
        /**
         * Move constructor.
         */
        State(State &&other);
        /**
         * Copy assignment.
         */
        State& operator=(const State &other);
        /**
         * Move assignment.
         */
        State& operator=(State &&other);

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
        uint64_t& operator()(size_t x, size_t y);
        const uint64_t& operator()(size_t x, size_t y) const;

        /**
         * Chosen permutation.
         */
        permutation_fun permutation;
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
        std::array<uint64_t, 25> data;
    };

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
    static void permutation(State &state);
    /**
     * Single round of the Keccak-f permutation.
     */
    static void round(State &state, uint64_t round);

    /**
     * theta part of the Keccac-f permutation.
     */
    static void theta(State &state);
    /**
     * rho and pi part of the Keccac-f permutation.
     */
    static void rho_pi(State &state);
    /**
     * chi part of the Keccac-f permutation.
     */
    static void chi(State &state);
    /**
     * iota part of the Keccac-f permutation.
     */
    static void iota(State &state, size_t round);

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
    State state_;
};


#endif // KECCAK_HPP
