#include <sstream>
#include <gtest/gtest.h>
#include "crypto/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"
#include "misc/util.hpp"

TEST(KeccakTest, Rotate)
{
    ASSERT_EQ(rotate(uint64_t{0x0123456789abcdef}, 4), uint64_t{0x123456789abcdef0});
    ASSERT_EQ(rotate(uint32_t{0x01234567}, 4), uint32_t{0x12345670});
    ASSERT_EQ(rotate(uint16_t{0x0123}, 4), uint16_t{0x1230});
    ASSERT_EQ(rotate(uint8_t{0x01}, 4), uint8_t{0x10});
    ASSERT_EQ(rotate(uint16_t{0xf564}, 2), uint16_t{0xd593});
    ASSERT_EQ(rotate(uint16_t{0x8f88}, 1), uint16_t{0x1f11});
    ASSERT_EQ(rotate(uint16_t{0xbfa8}, 12), uint16_t{0x8bfa});
    ASSERT_EQ(rotate(uint16_t{0x9d88}, 10), uint16_t{0x2276});
    ASSERT_EQ(rotate(uint16_t{0x9f88}, 13), uint16_t{0x13f1});
    ASSERT_EQ(rotate(uint16_t{0x9f88}, 2), uint16_t{0x7e22});
    ASSERT_EQ(rotate(uint16_t{0xd626}, 14), uint16_t{0xb589});
    ASSERT_EQ(rotate(uint16_t{0x5626}, 6), uint16_t{0x8995});
    ASSERT_EQ(rotate(uint16_t{0x5626}, 11), uint16_t{0x32b1});
    ASSERT_EQ(rotate(uint16_t{0x5226}, 15), uint16_t{0x2913});
    ASSERT_EQ(rotate(uint16_t{0x5722}, 13), uint16_t{0x4ae4});
    ASSERT_EQ(rotate(uint16_t{0xa54c}, 12), uint16_t{0xca54});
    ASSERT_EQ(rotate(uint16_t{0x854d}, 7), uint16_t{0xa6c2});

}

TEST(KeccakTest, RoundConstants)
{
    ASSERT_EQ(Keccak<uint64_t>::round_constants, round_constants_64);
    ASSERT_EQ(Keccak<uint32_t>::round_constants, round_constants_32);
    ASSERT_EQ(Keccak<uint16_t>::round_constants, round_constants_16);
    // ASSERT_EQ(Keccak<uint8_t>::round_constants, round_constants_8);
}

TEST(KeccakTest, Theta64)
{
    auto test_vectors{theta_vectors_64()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::theta(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Theta16)
{
    auto test_vectors{theta_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::theta(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, RhoPi64)
{
    auto test_vectors{rho_pi_vectors_64()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::rho_pi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, RhoPi16)
{
    auto test_vectors{rho_pi_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::rho_pi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Rho16)
{
    auto test_vectors{rho_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::rho(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Pi16)
{
    auto test_vectors{pi_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::pi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Chi64)
{
    auto test_vectors{chi_vectors_64()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::chi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Chi16)
{
    auto test_vectors{chi_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::chi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Iota64)
{
    auto test_vectors{iota_vectors_64()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::iota(state, std::get<2>(tuple));
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Iota16)
{
    auto test_vectors{iota_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::iota(state, std::get<2>(tuple));
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Permutation64)
{
    auto test_vectors{permutation_vectors_64()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint64_t> state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak<uint64_t>::permutation(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, Permutation16)
{
    auto test_vectors{permutation_vectors_16()};
    for (auto &tuple : test_vectors)
    {
        KeccakState<uint16_t> state{[] (auto) {}, 25, std::get<0>(tuple)};
        Keccak<uint16_t>::permutation(state);
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
