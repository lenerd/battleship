#include <gtest/gtest.h>
#include "circuits/trivial_decider.hpp"
#include "test_decider.hpp"

class TrivialDeciderTest : public ::testing::Test
{
protected:
    TrivialDeciderTest() = default;
    virtual ~TrivialDeciderTest() = default;
};



TEST_F(TrivialDeciderTest, PositiveTest)
{
    std::bitset<25> input;

    auto fut_s{decide_async_s<TrivialDecider>()};
    auto fut_c{decide_async_c<TrivialDecider>(input)};
    ASSERT_TRUE(fut_s.get());
    ASSERT_TRUE(fut_c.get());
}
