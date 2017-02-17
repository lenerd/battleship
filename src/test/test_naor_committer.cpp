#include <future>
#include <gtest/gtest.h>
#include "crypto/committer.hpp"
#include "crypto/naor_committer.hpp"
#include "misc/util.hpp"
#include "network/dummy_connection.hpp"

template <typename T>
class NaorCommitterTest : public ::testing::Test
{
protected:
    NaorCommitterTest()
    {
        auto conn_pair{DummyConnection::make_dummies()};
        comm_sender = std::make_unique<T>(conn_pair.first);
        comm_receiver = std::make_unique<T>(conn_pair.second);
    }
    virtual ~NaorCommitterTest() = default;

    std::unique_ptr<NaorCommitter> comm_sender;
    std::unique_ptr<NaorCommitter> comm_receiver;
};


TYPED_TEST_CASE_P(NaorCommitterTest);


TYPED_TEST_P(NaorCommitterTest, CommitDecommit)
{
    auto value{true};
    auto fut_s_1{std::async(std::launch::async,
        [this, value]
        {
            try
            {
                return this->comm_sender->send_commitment(value);
            }
            catch (std::exception &e)
            {
                std::cerr << "async sender: " << e.what() << "\n";
            }
            return std::make_shared<Commitment>();
        })};
    auto fut_r_1{std::async(std::launch::async,
        [this]
        {
            try
            {
                return this->comm_receiver->recv_commitment();
            }
            catch (std::exception &e)
            {
                std::cerr << "async receiver: " << e.what() << "\n";
                return std::make_shared<Commitment>();
            }
        })};
    auto comm_s{fut_s_1.get()};
    auto comm_r{fut_r_1.get()};
    auto ncomm_s{std::dynamic_pointer_cast<NaorCommitment>(comm_s)};
    auto ncomm_r{std::dynamic_pointer_cast<NaorCommitment>(comm_r)};

    ASSERT_EQ(value, comm_s->value);
    ASSERT_EQ(ncomm_s->rand, ncomm_r->rand);
    ASSERT_EQ(ncomm_s->commitment, ncomm_r->commitment);

    this->comm_sender->send_decommitment(comm_s);
    auto valid{this->comm_receiver->recv_decommitment(comm_r)};

    ASSERT_TRUE(valid);
    ASSERT_EQ(comm_s->value, comm_r->value);
    ASSERT_EQ(ncomm_s->seed, ncomm_r->seed);
}


REGISTER_TYPED_TEST_CASE_P(
        NaorCommitterTest,
        CommitDecommit);


typedef ::testing::Types<CTR_NaorCommitter> NaorCommitterImplementations;

INSTANTIATE_TYPED_TEST_CASE_P(NaorCommitterTestInstance, NaorCommitterTest, NaorCommitterImplementations);
