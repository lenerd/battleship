#include <gtest/gtest.h>
#include "circuits/battleship_decider.hpp"
#include "test_decider.hpp"

class BattleshipDeciderTest : public ::testing::Test
{
protected:
    BattleshipDeciderTest() = default;
    virtual ~BattleshipDeciderTest() = default;

    static void SetUpTestCase()
    {
        // positive examples
        positive_examples.emplace_back("1010101010"
                                       "1010101010"
                                       "0000000010"
                                       "0000000000"
                                       "1010101010"
                                       "1010101010"
                                       "1010101010"
                                       "0000101010"
                                       "0000000010"
                                       "0000000000");
        positive_examples.emplace_back("1010101000"
                                       "1010101000"
                                       "0000000000"
                                       "1110101000"
                                       "0000101000"
                                       "1110101000"
                                       "0000101000"
                                       "1110000000"
                                       "0000000000"
                                       "1111100000");
        // negative examples
        // * extremely wrong boards
        negative_examples.emplace_back("0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000"
                                       "0000000000");
        negative_examples.emplace_back("1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111"
                                       "1111111111");
        // additional garbage
        // * 1x ship
        additional_garbage_examples.emplace_back("1010101000"
                                                 "1010101000"
                                                 "0000000000"
                                                 "1110101000"
                                                 "0000101000"
                                                 "1110101000"
                                                 "0000101000"
                                                 "1110000000"
                                                 "0000000000"
                                                 "1111100100");
        // * 6x ship
        additional_garbage_examples.emplace_back("1010101000"
                                                 "1010101000"
                                                 "0000000000"
                                                 "1110101010"
                                                 "0000101010"
                                                 "1110101010"
                                                 "0000101010"
                                                 "1110000010"
                                                 "0000000010"
                                                 "1111100000");
        // * L ship
        additional_garbage_examples.emplace_back("1010101000"
                                                 "1010101000"
                                                 "0000000000"
                                                 "1110101000"
                                                 "0000101000"
                                                 "1110101000"
                                                 "0000101000"
                                                 "1110000010"
                                                 "0000001110"
                                                 "1111100000");
        // * 2x2 ship
        additional_garbage_examples.emplace_back("1010101000"
                                                 "1010101000"
                                                 "0000000000"
                                                 "1110101000"
                                                 "0000101000"
                                                 "1110101000"
                                                 "0000101000"
                                                 "1110000011"
                                                 "0000000011"
                                                 "1111100000");
        // * additional 2x ship
        additional_ships_examples.emplace_back("1010101000"
                                               "1010101000"
                                               "0000000000"
                                               "1110101010"
                                               "0000101010"
                                               "1110101000"
                                               "0000101000"
                                               "1110000000"
                                               "0000000000"
                                               "1111100000");
        // * additional 3x ship
        additional_ships_examples.emplace_back("1010101000"
                                               "1010101000"
                                               "0000000000"
                                               "1110101010"
                                               "0000101010"
                                               "1110101010"
                                               "0000101000"
                                               "1110000000"
                                               "0000000000"
                                               "1111100000");
        // * additional 4x ship
        additional_ships_examples.emplace_back("1010101000"
                                               "1010101010"
                                               "0000000010"
                                               "1110101010"
                                               "0000101010"
                                               "1110101000"
                                               "0000101000"
                                               "1110000000"
                                               "0000000000"
                                               "1111100000");
        // * additional 5x ship
        additional_ships_examples.emplace_back("1010101000"
                                               "1010101000"
                                               "0000000000"
                                               "1110101000"
                                               "0000101010"
                                               "1110101010"
                                               "0000101010"
                                               "1110000010"
                                               "0000000010"
                                               "1111100000");
        // * missing 2x ship
        missing_ships_examples.emplace_back("0010101000"
                                            "0010101000"
                                            "0000000000"
                                            "1110101000"
                                            "0000101000"
                                            "1110101000"
                                            "0000101000"
                                            "1110000000"
                                            "0000000000"
                                            "1111100000");
        // * missing 3x ship
        missing_ships_examples.emplace_back("1010101000"
                                            "1010101000"
                                            "0000000000"
                                            "0000101000"
                                            "0000101000"
                                            "1110101000"
                                            "0000101000"
                                            "1110000000"
                                            "0000000000"
                                            "1111100000");
        // * missing 4x ship
        missing_ships_examples.emplace_back("1010101000"
                                            "1010101000"
                                            "0000000000"
                                            "1110001000"
                                            "0000001000"
                                            "1110001000"
                                            "0000001000"
                                            "1110000000"
                                            "0000000000"
                                            "1111100000");
        // * missing 5x ship
        missing_ships_examples.emplace_back("1010101000"
                                            "1010101000"
                                            "0000000000"
                                            "1110101000"
                                            "0000101000"
                                            "1110101000"
                                            "0000101000"
                                            "1110000000"
                                            "0000000000"
                                            "0000000000");
    }

    // positive examples
    static std::vector<std::bitset<100>> positive_examples;
    // negative examples
    static std::vector<std::bitset<100>> negative_examples;
    static std::vector<std::bitset<100>> missing_ships_examples;
    static std::vector<std::bitset<100>> additional_ships_examples;
    static std::vector<std::bitset<100>> additional_garbage_examples;
};

std::vector<std::bitset<100>> BattleshipDeciderTest::positive_examples;
std::vector<std::bitset<100>> BattleshipDeciderTest::negative_examples;
std::vector<std::bitset<100>> BattleshipDeciderTest::missing_ships_examples;
std::vector<std::bitset<100>> BattleshipDeciderTest::additional_ships_examples;
std::vector<std::bitset<100>> BattleshipDeciderTest::additional_garbage_examples;


TEST_F(BattleshipDeciderTest, PositiveTest)
{
    for (auto &input : positive_examples)
    {
        auto fut_s{decide_async_s<BattleshipDecider>()};
        auto fut_c{decide_async_c<BattleshipDecider>(input)};
        ASSERT_TRUE(fut_s.get());
        ASSERT_TRUE(fut_c.get());
    }
}


TEST_F(BattleshipDeciderTest, NegativeTest)
{
    for (auto &input : negative_examples)
    {
        auto fut_s{decide_async_s<BattleshipDecider>()};
        auto fut_c{decide_async_c<BattleshipDecider>(input)};
        ASSERT_FALSE(fut_s.get());
        ASSERT_FALSE(fut_c.get());
    }
}


TEST_F(BattleshipDeciderTest, MissingShipsTest)
{
    for (auto &input : missing_ships_examples)
    {
        auto fut_s{decide_async_s<BattleshipDecider>()};
        auto fut_c{decide_async_c<BattleshipDecider>(input)};
        ASSERT_FALSE(fut_s.get());
        ASSERT_FALSE(fut_c.get());
    }
}


TEST_F(BattleshipDeciderTest, AdditionalShipsTest)
{
    for (auto &input : additional_ships_examples)
    {
        auto fut_s{decide_async_s<BattleshipDecider>()};
        auto fut_c{decide_async_c<BattleshipDecider>(input)};
        ASSERT_FALSE(fut_s.get());
        ASSERT_FALSE(fut_c.get());
    }
}


TEST_F(BattleshipDeciderTest, AdditionalGarbageTest)
{
    for (auto &input : additional_garbage_examples)
    {
        auto fut_s{decide_async_s<BattleshipDecider>()};
        auto fut_c{decide_async_c<BattleshipDecider>(input)};
        ASSERT_FALSE(fut_s.get());
        ASSERT_FALSE(fut_c.get());
    }
}
