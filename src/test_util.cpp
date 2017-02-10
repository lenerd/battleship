#include <gtest/gtest.h>
#include "util.hpp"

TEST(UtilTest, RandomBytesTest)
{
    bytes_t val(random_bytes(42));
    ASSERT_EQ(val.size(), 42);
}
