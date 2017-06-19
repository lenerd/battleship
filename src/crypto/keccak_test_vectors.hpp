#ifndef KECCAK_TEST_VECTORS_HPP
#define KECCAK_TEST_VECTORS_HPP

#include <array>
#include <cstdint>
#include <tuple>
#include <vector>

using bytes_t = std::vector<uint8_t>;

using perm_vector = std::tuple<std::array<uint64_t, 25>, std::array<uint64_t, 25>, size_t>;
using test_vector = std::pair<bytes_t, bytes_t>;

std::vector<perm_vector> theta_vectors();
std::vector<perm_vector> rho_pi_vectors();
std::vector<perm_vector> chi_vectors();
std::vector<perm_vector> iota_vectors();
std::vector<perm_vector> permutation_vectors();

std::vector<test_vector> sha3_256_vectors();
std::vector<test_vector> sha3_512_vectors();

extern std::array<uint64_t, 24> round_constants_64;
extern std::array<uint32_t, 24> round_constants_32;
extern std::array<uint16_t, 24> round_constants_16;
extern std::array<uint8_t, 24> round_constants_8;

#endif // KECCAK_TEST_VECTORS_HPP
