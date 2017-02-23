#include <gtest/gtest.h>
#include "circuits/example_decider.hpp"
#include "test_decider.hpp"

class ExampleDeciderTest : public ::testing::Test
{
protected:
    ExampleDeciderTest() = default;
    virtual ~ExampleDeciderTest() = default;

    static void SetUpTestCase()
    {
        // positive examples
        positive_examples.emplace_back("00000"
                                       "00000"
                                       "00000"
                                       "00000"
                                       "00000");
        positive_examples.emplace_back("10000"
                                       "00000"
                                       "00000"
                                       "00000"
                                       "00000");
        positive_examples.emplace_back("10100"
                                       "00000"
                                       "00000"
                                       "00000"
                                       "00000");
        positive_examples.emplace_back("10100"
                                       "00000"
                                       "10000"
                                       "00000"
                                       "00000");
        positive_examples.emplace_back("10101"
                                       "00000"
                                       "01010"
                                       "00000"
                                       "00101");
        // negative examples
        negative_examples.emplace_back("11000"
                                       "00000"
                                       "00000"
                                       "00000"
                                       "00000");
        negative_examples.emplace_back("10000"
                                       "01000"
                                       "00000"
                                       "00000"
                                       "00000");
        negative_examples.emplace_back("10000"
                                       "10000"
                                       "00000"
                                       "00000"
                                       "00000");
        negative_examples.emplace_back("10101"
                                       "00000"
                                       "01010"
                                       "10000"
                                       "00101");
    }

    static std::vector<std::bitset<25>> positive_examples;
    static std::vector<std::bitset<25>> negative_examples;
};

std::vector<std::bitset<25>> ExampleDeciderTest::positive_examples;
std::vector<std::bitset<25>> ExampleDeciderTest::negative_examples;


TEST_F(ExampleDeciderTest, PositiveTest)
{
    for (auto &input : positive_examples)
    {
        auto fut_s{decide_async_s<ExampleDecider>()};
        auto fut_c{decide_async_c<ExampleDecider>(input)};
        ASSERT_TRUE(fut_s.get());
        ASSERT_TRUE(fut_c.get());
    }
}


TEST_F(ExampleDeciderTest, NegativeTest)
{
    for (auto &input : negative_examples)
    {
        auto fut_s{decide_async_s<ExampleDecider>()};
        auto fut_c{decide_async_c<ExampleDecider>(input)};
        ASSERT_FALSE(fut_s.get());
        ASSERT_FALSE(fut_c.get());
    }
}
