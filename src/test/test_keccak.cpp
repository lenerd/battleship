#include <sstream>
#include <gtest/gtest.h>
#include "crypto/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"
#include "misc/util.hpp"

TEST(KeccakTest, RoundConstants)
{
    ASSERT_EQ(Keccak<uint64_t>::round_constants, round_constants_64);
    ASSERT_EQ(Keccak<uint32_t>::round_constants, round_constants_32);
    ASSERT_EQ(Keccak<uint16_t>::round_constants, round_constants_16);
    // ASSERT_EQ(Keccak<uint8_t>::round_constants, round_constants_8);
}

TEST(KeccakTest, Theta)
{
    auto test_vectors{theta_vectors()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::theta(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, RhoPi)
{
    auto test_vectors{rho_pi_vectors()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::rho_pi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Chi)
{
    auto test_vectors{chi_vectors()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::chi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Iota)
{
    auto test_vectors{iota_vectors()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::iota(state, std::get<2>(tuple));
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Permutation)
{
    auto test_vectors{permutation_vectors()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::permutation(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Padding)
{
    auto input{random_bytes(23)};
    bytes_t expected_padding{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80};
    auto padded{Keccak<uint64_t>::pad(input, 32)};
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 32);
    ASSERT_EQ(bytes_t(padded.cbegin() + 23, padded.cend()), expected_padding);

    input = random_bytes(47);
    expected_padding = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    padded = Keccak<uint64_t>::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 64);
    ASSERT_EQ(bytes_t(padded.cbegin() + 47, padded.cend()), expected_padding);

    input = random_bytes(32);
    expected_padding = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    padded = Keccak<uint64_t>::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 64);
    ASSERT_EQ(bytes_t(padded.cbegin() + 32, padded.cend()), expected_padding);

    input = random_bytes(31);
    expected_padding = {0x81};
    padded = Keccak<uint64_t>::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 32);
    ASSERT_EQ(bytes_t(padded.cbegin() + 31, padded.cend()), expected_padding);
}

TEST(KeccakTest, SHA3_256)
{
    auto test_vectors{sha3_256_vectors()};
    for (auto &pair : test_vectors)
    {
        auto keccak{SHA3_256()};
        keccak.update(pair.first);
        auto digest{keccak.finalize()};
        ASSERT_EQ(digest, pair.second);
    }
}

TEST(KeccakTest, SHA3_512)
{
    auto test_vectors{sha3_512_vectors()};
    for (auto &pair : test_vectors)
    {
        auto keccak{SHA3_512()};
        keccak.update(pair.first);
        auto digest{keccak.finalize()};
        ASSERT_EQ(digest, pair.second);
    }
}

TEST(KeccakTest, Keccak400)
{
    auto keccak{Keccak400()};
    keccak.update(random_bytes(16));
    auto digest{keccak.finalize()};
}
