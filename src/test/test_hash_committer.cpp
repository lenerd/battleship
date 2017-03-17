#include <gtest/gtest.h>
#include "crypto/committer.hpp"
#include "crypto/hash_committer.hpp"
#include "misc/util.hpp"
#include "network/dummy_connection.hpp"

template <typename T>
class HashCommitterTest : public ::testing::Test
{
protected:
    HashCommitterTest()
    {
        auto conn_pair{DummyConnection::make_dummies()};
        comm_sender = std::make_unique<T>(conn_pair.first);
        comm_receiver = std::make_unique<T>(conn_pair.second);
    }
    virtual ~HashCommitterTest() = default;

    std::unique_ptr<HashCommitter> comm_sender;
    std::unique_ptr<HashCommitter> comm_receiver;
};


TYPED_TEST_CASE_P(HashCommitterTest);


TYPED_TEST_P(HashCommitterTest, MakeVerify)
{
    bool value{true};
    auto comm = this->comm_sender->make_commitment(value);
    ASSERT_EQ(value, comm->value);
    ASSERT_TRUE(this->comm_sender->verify_commitment(comm));

    value = false;
    comm = this->comm_sender->make_commitment(value);
    ASSERT_EQ(value, comm->value);
    ASSERT_TRUE(this->comm_sender->verify_commitment(comm));

    value = true;
    comm = this->comm_sender->make_commitment(value);
    comm->value = false;
    ASSERT_NE(value, comm->value);
    ASSERT_FALSE(this->comm_sender->verify_commitment(comm));

    value = false;
    comm = this->comm_sender->make_commitment(value);
    comm->value = true;
    ASSERT_NE(value, comm->value);
    ASSERT_FALSE(this->comm_sender->verify_commitment(comm));
}


REGISTER_TYPED_TEST_CASE_P(
        HashCommitterTest,
        MakeVerify);


typedef ::testing::Types<SHA1_HashCommitter, SHA3_256_HashCommitter> HashCommitterImplementations;

INSTANTIATE_TYPED_TEST_CASE_P(HashCommitterTestInstance, HashCommitterTest, HashCommitterImplementations);
