#include <sstream>
#include <gtest/gtest.h>
#include "crypto/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"

// TEST(KeccakTest, KeccakTest)
// {
//     FAIL();
// }

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
