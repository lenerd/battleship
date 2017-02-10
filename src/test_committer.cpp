#include <future>
#include <gtest/gtest.h>
#include "committer.hpp"
#include "dummy_connection.hpp"
#include "hash_committer.hpp"
#include "naor_committer.hpp"
#include "util.hpp"

template <typename T>
class CommitterTest : public ::testing::Test
{
protected:
    CommitterTest()
    {
        auto conn_pair{DummyConnection::make_dummies()};
        comm_sender = std::make_unique<T>(conn_pair.first);
        comm_receiver = std::make_unique<T>(conn_pair.second);
    }
    virtual ~CommitterTest() = default;

    std::unique_ptr<Committer> comm_sender;
    std::unique_ptr<Committer> comm_receiver;
};


TYPED_TEST_CASE_P(CommitterTest);


TYPED_TEST_P(CommitterTest, SendRecv)
{
    bool value(true);
    auto fut_s_1{std::async(std::launch::async,
        [this, value]
        {
            return this->comm_sender->send_commitment(value);
        })};
    auto fut_r_1{std::async(std::launch::async,
        [this]
        {
            return this->comm_receiver->recv_commitment();
        })};
    auto comm_s{fut_s_1.get()};
    auto comm_r{fut_r_1.get()};
    this->comm_sender->send_decommitment(comm_s);
    auto valid{this->comm_receiver->recv_decommitment(comm_r)};
    ASSERT_TRUE(valid);
    ASSERT_EQ(value, comm_r->value);

    value = false;
    fut_s_1 = std::async(std::launch::async,
        [this, value]
        {
            return this->comm_sender->send_commitment(value);
        });
    fut_r_1 = std::async(std::launch::async,
        [this]
        {
            return this->comm_receiver->recv_commitment();
        });
    comm_s = fut_s_1.get();
    comm_r = fut_r_1.get();
    this->comm_sender->send_decommitment(comm_s);
    valid = this->comm_receiver->recv_decommitment(comm_r);
    ASSERT_TRUE(valid);
    ASSERT_EQ(value, comm_r->value);

    value = true;
    fut_s_1 = std::async(std::launch::async,
        [this, value]
        {
            return this->comm_sender->send_commitment(value);
        });
    fut_r_1 = std::async(std::launch::async,
        [this]
        {
            return this->comm_receiver->recv_commitment();
        });
    comm_s = fut_s_1.get();
    comm_r = fut_r_1.get();
    comm_s->value = false;
    this->comm_sender->send_decommitment(comm_s);
    valid = this->comm_receiver->recv_decommitment(comm_r);
    ASSERT_FALSE(valid);

    value = false;
    fut_s_1 = std::async(std::launch::async,
        [this, value]
        {
            return this->comm_sender->send_commitment(value);
        });
    fut_r_1 = std::async(std::launch::async,
        [this]
        {
            return this->comm_receiver->recv_commitment();
        });
    comm_s = fut_s_1.get();
    comm_r = fut_r_1.get();
    comm_s->value = true;
    this->comm_sender->send_decommitment(comm_s);
    valid = this->comm_receiver->recv_decommitment(comm_r);
    ASSERT_FALSE(valid);
}


REGISTER_TYPED_TEST_CASE_P(
        CommitterTest,
        SendRecv);


typedef ::testing::Types<SHA1_HashCommitter, CTR_NaorCommitter> CommitterImplementations;

INSTANTIATE_TYPED_TEST_CASE_P(CommitterTestInstance, CommitterTest, CommitterImplementations);
