#include <sstream>
#include <gtest/gtest.h>
#include "crypto/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"
#include "misc/util.hpp"


TEST(KeccakTest, ThetaTest)
{
    auto test_vectors{theta_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::theta(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, RhoPiTest)
{
    auto test_vectors{rho_pi_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::rho_pi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, ChiTest)
{
    auto test_vectors{chi_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::chi(state);
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, IotaTest)
{
    auto test_vectors{iota_vectors()};
    for (auto &tuple : test_vectors)
    {
        Keccak::State state{[] (auto) {}, 128, std::get<0>(tuple)};
        Keccak::iota(state, std::get<2>(tuple));
        ASSERT_EQ(state.data, std::get<1>(tuple));
    }
}

TEST(KeccakTest, PermutationTest)
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
    auto padded{Keccak::pad(input, 32)};
    ASSERT_EQ(padded.size() % 32, 0);

    input = random_bytes(47);
    padded = Keccak::pad(input, 32);
    ASSERT_EQ(padded.size() % 32, 0);
}

TEST(KeccakTest, SHA3_256_Test)
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

TEST(KeccakTest, SHA3_512_Test)
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
