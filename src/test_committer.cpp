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
    bytes_t value(16);

    auto comm_s{this->comm_sender->send_commitment(value)};
    auto comm_r{this->comm_receiver->recv_commitment()};

    this->comm_sender->send_decommitment(comm_s);
    ASSERT_TRUE(this->comm_receiver->recv_decommitment(comm_r));
}


REGISTER_TYPED_TEST_CASE_P(
        CommitterTest,
        SendRecv);


typedef ::testing::Types<SHA1Committer, NaorCommitter> CommitterImplementations;

INSTANTIATE_TYPED_TEST_CASE_P(CommitterTestInstance, CommitterTest, CommitterImplementations);
