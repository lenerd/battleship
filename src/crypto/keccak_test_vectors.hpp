#ifndef KECCAK_TEST_VECTORS_HPP
#define KECCAK_TEST_VECTORS_HPP

#include <array>
#include <cstdint>
#include <tuple>
#include <vector>

using bytes_t = std::vector<uint8_t>;

template <typename Word>
using perm_vector = std::tuple<std::array<Word, 25>, std::array<Word, 25>, size_t>;
using test_vector = std::pair<bytes_t, bytes_t>;

std::vector<perm_vector<uint64_t>> theta_vectors_64();
std::vector<perm_vector<uint64_t>> rho_pi_vectors_64();
std::vector<perm_vector<uint64_t>> chi_vectors_64();
std::vector<perm_vector<uint64_t>> iota_vectors_64();
std::vector<perm_vector<uint64_t>> permutation_vectors_64();
std::vector<perm_vector<uint16_t>> theta_vectors_16();
std::vector<perm_vector<uint16_t>> rho_pi_vectors_16();
std::vector<perm_vector<uint16_t>> rho_vectors_16();
std::vector<perm_vector<uint16_t>> pi_vectors_16();
std::vector<perm_vector<uint16_t>> chi_vectors_16();
std::vector<perm_vector<uint16_t>> iota_vectors_16();
std::vector<perm_vector<uint16_t>> permutation_vectors_16();

std::vector<test_vector> sha3_256_vectors();
std::vector<test_vector> sha3_512_vectors();

extern std::array<uint64_t, 24> round_constants_64;
extern std::array<uint32_t, 24> round_constants_32;
extern std::array<uint16_t, 24> round_constants_16;
extern std::array<uint8_t, 24> round_constants_8;

#endif // KECCAK_TEST_VECTORS_HPP
