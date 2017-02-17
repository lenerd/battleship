#include <sstream>
#include <gtest/gtest.h>
#include "misc/options.hpp"
#include "misc/util.hpp"

TEST(MiscTest, RandomBytes)
{
    bytes_t val(random_bytes(42));
    ASSERT_EQ(val.size(), 42);
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
