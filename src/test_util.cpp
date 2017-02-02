#include <gtest/gtest.h>
#include "util.hpp"

TEST(RandomBytesTest, UtilTest)
{
    bytes_t val(random_bytes(42));
    ASSERT_EQ(val.size(), 42);
}
