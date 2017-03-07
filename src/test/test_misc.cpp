#include <sstream>
#include <gtest/gtest.h>
#include "misc/options.hpp"
#include "misc/util.hpp"

TEST(MiscTest, RandomBytes)
{
    bytes_t val(random_bytes(42));
    ASSERT_EQ(val.size(), 42);
}

TEST(MiscTest, HexlifyTest)
{
    bytes_t input{0x1d, 0x0d, 0x35, 0x3f, 0x65, 0xf3, 0x9e, 0xfe, 0xbc, 0x62,
        0x15, 0x7f, 0x99, 0x7e, 0x3e, 0x71};
    std::string output("1d0d353f65f39efebc62157f997e3e71");
    std::string output_upper("1D0D353F65F39EFEBC62157F997E3E71");
    ASSERT_EQ(hexlify(input), output);
    ASSERT_EQ(hexlify(input, true), output_upper);

    input = {0xd7, 0x78, 0x0a, 0x66, 0x6e, 0x36, 0xc4, 0x8e, 0x2d, 0xea, 0xd0,
        0xf6, 0x0a, 0x4d, 0x46, 0xa8};
    output = "d7780a666e36c48e2dead0f60a4d46a8";
    output_upper = "D7780A666E36C48E2DEAD0F60A4D46A8";
    ASSERT_EQ(hexlify(input), output);
    ASSERT_EQ(hexlify(input, true), output_upper);
}

TEST(MiscTest, UnhexlifyTest)
{
    bytes_t output{0x1d, 0x0d, 0x35, 0x3f, 0x65, 0xf3, 0x9e, 0xfe, 0xbc, 0x62,
        0x15, 0x7f, 0x99, 0x7e, 0x3e, 0x71};
    std::string input("1d0d353f65f39efebc62157f997e3e71");
    std::string input_upper("1D0D353F65F39EFEBC62157F997E3E71");
    std::string input_mixed("1d0d353f65f39efeBC62157F997E3E71");
    ASSERT_EQ(unhexlify(input), output);
    ASSERT_EQ(unhexlify(input_upper), output);
    ASSERT_EQ(unhexlify(input_mixed), output);

    output = {0xd7, 0x78, 0x0a, 0x66, 0x6e, 0x36, 0xc4, 0x8e, 0x2d, 0xea, 0xd0,
        0xf6, 0x0a, 0x4d, 0x46, 0xa8};
    input = "d7780a666e36c48e2dead0f60a4d46a8";
    input_upper = "D7780A666E36C48E2DEAD0F60A4D46A8";
    input_mixed = "d7780a666e36c48e2DEAD0F60A4D46A8";
    ASSERT_EQ(unhexlify(input), output);
    ASSERT_EQ(unhexlify(input_upper), output);
    ASSERT_EQ(unhexlify(input_mixed), output);
}

TEST(MiscTest, StringToBits)
{
    std::string bitstr("10001011""11101111");
    auto bits{string_to_bits<16>(bitstr)};
    for (size_t i{0}; i < bitstr.size(); ++i)
    {
        ASSERT_EQ(bits[i], bitstr[i] - '0');
    }
}

TEST(MiscTest, BitsToBytes)
{
    auto bits{string_to_bits<32>("01111011"
                                 "10110101"
                                 "01111101"
                                 "11110111")};
    bytes_t bytes{bits_to_bytes(bits)};
    bytes_t correct_bytes{0xde, 0xad, 0xbe, 0xef};
    ASSERT_EQ(bytes, correct_bytes);
}

TEST(MiscTest, RoleEnum)
{
    std::stringstream ss;
    Role role;
    ss << Role::client;
    ASSERT_EQ(ss.str(), "client");
    ss >> role;
    ASSERT_EQ(role, Role::client);

    ss = std::stringstream();
    ss << Role::server;
    ASSERT_EQ(ss.str(), "server");
    ss >> role;
    ASSERT_EQ(role, Role::server);

    ss = std::stringstream();
    ss << "foobar";
    ASSERT_EQ(ss.str(), "foobar");
    ASSERT_ANY_THROW(ss >> role);
}

TEST(MiscTest, UITypeEnum)
{
    std::stringstream ss;
    UIType role;
    ss << UIType::ncurses;
    ASSERT_EQ(ss.str(), "ncurses");
    ss >> role;
    ASSERT_EQ(role, UIType::ncurses);

    ss = std::stringstream();
    ss << UIType::web;
    ASSERT_EQ(ss.str(), "web");
    ss >> role;
    ASSERT_EQ(role, UIType::web);

    ss = std::stringstream();
    ss << "foobar";
    ASSERT_EQ(ss.str(), "foobar");
    ASSERT_ANY_THROW(ss >> role);
}
