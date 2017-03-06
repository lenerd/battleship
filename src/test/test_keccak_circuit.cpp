#include <gtest/gtest.h>
#include "circuits/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"
#include "test_keccak_helper.hpp"


TEST(KeccakCircuitTest, ThetaTest)
{
    auto test_vectors{theta_vectors()};
    for (auto &tuple : test_vectors)
    {
        auto input{std::get<0>(tuple)};
        auto round{std::get<2>(tuple)};
        auto output{eval_permutation(build_theta, input, round)};
        ASSERT_EQ(output, std::get<1>(tuple));
    }
}

TEST(KeccakCircuitTest, RhoPiTest)
{
    auto test_vectors{rho_pi_vectors()};
    for (auto &tuple : test_vectors)
    {
        auto input{std::get<0>(tuple)};
        auto round{std::get<2>(tuple)};
        auto output{eval_permutation(build_rho_pi, input, round)};
        ASSERT_EQ(output, std::get<1>(tuple));
    }
}

TEST(KeccakCircuitTest, ChiTest)
{
    auto test_vectors{chi_vectors()};
    for (auto &tuple : test_vectors)
    {
        auto input{std::get<0>(tuple)};
        auto round{std::get<2>(tuple)};
        auto output{eval_permutation(build_chi, input, round)};
        ASSERT_EQ(output, std::get<1>(tuple));
    }
}

TEST(KeccakCircuitTest, IotaTest)
{
    auto test_vectors{iota_vectors()};
    for (auto &tuple : test_vectors)
    {
        auto input{std::get<0>(tuple)};
        auto round{std::get<2>(tuple)};
        auto output{eval_permutation(build_iota, input, round)};
        ASSERT_EQ(output, std::get<1>(tuple));
    }
}

TEST(KeccakCircuitTest, PermutationTest)
{
    auto test_vectors{permutation_vectors()};
    for (auto &tuple : test_vectors)
    {
        auto input{std::get<0>(tuple)};
        auto output{eval_permutation(build_permutation, input)};
        ASSERT_EQ(output, std::get<1>(tuple));
    }
}
