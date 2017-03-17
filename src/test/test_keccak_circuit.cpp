#include <gtest/gtest.h>
#include "circuits/keccak.hpp"
#include "crypto/keccak_test_vectors.hpp"
#include "test_keccak_helper.hpp"


TEST(KeccakCircuitTest, Theta)
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

TEST(KeccakCircuitTest, RhoPi)
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

TEST(KeccakCircuitTest, Chi)
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

TEST(KeccakCircuitTest, Iota)
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

TEST(KeccakCircuitTest, Permutation)
{
    auto test_vectors{permutation_vectors()};
    for (auto &tuple : test_vectors)
    {
        auto input{std::get<0>(tuple)};
        auto output{eval_permutation(build_permutation, input)};
        ASSERT_EQ(output, std::get<1>(tuple));
    }
}

TEST(KeccakCircuitTest, SHA3_256)
{
    auto test_vectors{sha3_256_vectors()};
    size_t i{0};
    for (auto &pair : test_vectors)
    {
        auto input{pair.first};
        std::cerr << "test vector " << i << ", input size " << input.size() << "\n";

        if (input.size() > 10000)
            continue;

        auto digest{eval_hash(build_sha3_256, input)};
        ASSERT_EQ(digest, pair.second);
        ++i;
    }
}

TEST(KeccakCircuitTest, SHA3_512)
{
    auto test_vectors{sha3_512_vectors()};
    size_t i{0};
    for (auto &pair : test_vectors)
    {
        auto input{pair.first};

        if (input.size() > 10000)
            continue;
        std::cerr << "test vector " << i << ", input size " << input.size() << "\n";

        auto digest{eval_hash(build_sha3_512, input)};
        ASSERT_EQ(digest, pair.second);
        ++i;
    }
}
