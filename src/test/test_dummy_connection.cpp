#include <gtest/gtest.h>
#include "network/dummy_connection.hpp"

class DummyConnectionTest : public ::testing::Test
{
protected:
    DummyConnectionTest()
    {
        auto conn_pair{DummyConnection::make_dummies()};
        conn1 = conn_pair.first;
        conn2 = conn_pair.second;
    }
    virtual ~DummyConnectionTest() = default;

    Conn_p conn1;
    Conn_p conn2;
};

TEST_F(DummyConnectionTest, Init)
{
    auto dconn1{std::dynamic_pointer_cast<DummyConnection>(conn1)};
    auto dconn2{std::dynamic_pointer_cast<DummyConnection>(conn2)};
    ASSERT_EQ(dconn1->send_queue_, dconn2->recv_queue_);
    ASSERT_EQ(dconn1->recv_queue_, dconn2->send_queue_);
}

TEST_F(DummyConnectionTest, SendRecv)
{
    bytes_t original(16, 0x42);
    conn1->send_message(original);
    auto received(conn2->recv_message());
    ASSERT_EQ(original, received);

    original = bytes_t(16, 0x47);
    conn1->send_message(original.data(), original.size());
    received = conn2->recv_message();
    ASSERT_EQ(original, received);
}
