#ifndef KECCAK_HPP
#define KECCAK_HPP

#include <array>
#include <functional>
#include <vector>

using bytes_t = std::vector<uint8_t>;


class Keccak
{
public:
    class State
    {
    public:
        using permutation_fun = std::function<void(State&)>;
        State(permutation_fun permutation, size_t capacity_bytes);
        State(permutation_fun permutation, size_t capacity_bytes, const bytes_t &initial);
        State(permutation_fun permutation, size_t capacity_bytes, const std::vector<uint64_t> &initial);
        State(permutation_fun permutation, size_t capacity_bytes, const std::array<uint64_t, 25> &initial);

        State(const State &other);
        State(State &&other);
        State& operator=(const State &other);
        State& operator=(State &&other);

        void absorb(const bytes_t &bytes);
        void absorb(bytes_t::const_iterator first);
        bytes_t squeeze();
        void squeeze(bytes_t::iterator out_first);

        uint64_t& operator()(size_t x, size_t y);
        const uint64_t& operator()(size_t x, size_t y) const;

        permutation_fun permutation;
        size_t capacity_bytes;
        size_t rate_bytes;
        std::array<uint64_t, 25> data;
    };

    Keccak(size_t capacity_bits=576, uint8_t d=0x01, size_t output_bits=0);
    static Keccak SHA3_256();
    static Keccak SHA3_512();

    void update(const bytes_t& input);
    bytes_t finalize();

    static bytes_t pad(const bytes_t &message, size_t blocksize, uint8_t d=0x01);

    static void permutation(State &state);
    static void round(State &state, uint64_t round);

    static void theta(State &state);
    static void rho_pi(State &state);
    static void chi(State &state);
    static void iota(State &state, size_t round);

    size_t output_bytes = 32;
    size_t capacity_bytes = 72;
    size_t rate_bytes = 128;
    uint8_t d = 0x01;

    bool finalized = false;

    const static std::array<uint64_t, 24> round_constants;
    const static std::array<std::array<size_t, 5>, 5> rotation_offsets;

private:
    bytes_t input_buffer;
    State state_;
};


#endif // KECCAK_HPP
