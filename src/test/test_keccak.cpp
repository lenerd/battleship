#include <sstream>
#include <gtest/gtest.h>
#include "crypto/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"
#include "misc/util.hpp"


TEST(KeccakTest, Theta)
{
    auto test_vectors{theta_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::theta(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, RhoPi)
{
    auto test_vectors{rho_pi_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::rho_pi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Chi)
{
    auto test_vectors{chi_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::chi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Iota)
{
    auto test_vectors{iota_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::iota(state, std::get<2>(tuple));
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Permutation)
{
    auto test_vectors{permutation_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::permutation(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Padding)
{
    auto input{random_bytes(23)};
    bytes_t expected_padding{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80};
    auto padded{Keccak::pad(input, 32)};
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 32);
    ASSERT_EQ(bytes_t(padded.cbegin() + 23, padded.cend()), expected_padding);

    input = random_bytes(47);
    expected_padding = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    padded = Keccak::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 64);
    ASSERT_EQ(bytes_t(padded.cbegin() + 47, padded.cend()), expected_padding);

    input = random_bytes(32);
    expected_padding = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    padded = Keccak::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 64);
    ASSERT_EQ(bytes_t(padded.cbegin() + 32, padded.cend()), expected_padding);

    input = random_bytes(31);
    expected_padding = {0x81};
    padded = Keccak::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
    ASSERT_EQ(padded.size(), 32);
    ASSERT_EQ(bytes_t(padded.cbegin() + 31, padded.cend()), expected_padding);
}

TEST(KeccakTest, SHA3_256)
{
    auto test_vectors{sha3_256_vectors()};
    for (auto &pair : test_vectors)
    {
        auto keccak{Keccak::SHA3_256()};
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
        auto keccak{Keccak::SHA3_512()};
        keccak.update(pair.first);
        auto digest{keccak.finalize()};
        ASSERT_EQ(digest, pair.second);
    }
}
