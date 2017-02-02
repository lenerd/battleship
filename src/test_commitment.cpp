#include <gtest/gtest.h>
#include "commitment.hpp"
#include "util.hpp"


TEST(CommitmentTest, CommitmentOpen)
{
    bytes_t val0(random_bytes(16));

    Commitment com0_s = Commitment::create_open(val0);
    ASSERT_EQ(val0, com0_s.get_value());
    ASSERT_TRUE(com0_s.is_open());
    ASSERT_FALSE(com0_s.is_committed());

    com0_s.commit();
    ASSERT_EQ(val0, com0_s.get_value());
    ASSERT_TRUE(com0_s.is_open());
    ASSERT_TRUE(com0_s.is_committed());
    ASSERT_FALSE(com0_s.get_padding().empty());
    ASSERT_FALSE(com0_s.get_commitment().empty());
    ASSERT_TRUE(com0_s.verify());
}

TEST(CommitmentTest, CommitmentHidden)
{
    bytes_t val0(random_bytes(16));
    Commitment com0_s = Commitment::create_open(val0);
    com0_s.commit();

    Commitment com0_r = Commitment::create_hidden(com0_s.send_commitment());
    ASSERT_EQ(com0_s.get_commitment(), com0_r.get_commitment());
    ASSERT_TRUE(com0_r.is_committed());
    ASSERT_FALSE(com0_r.is_open());

    com0_r.recv_decommitment(com0_s.decommit());
    ASSERT_EQ(com0_s.get_commitment(), com0_r.get_commitment());
    ASSERT_EQ(com0_r.get_value(), val0);
    ASSERT_EQ(com0_s.get_padding(), com0_r.get_padding());
    ASSERT_TRUE(com0_r.is_committed());
    ASSERT_TRUE(com0_r.is_open());
    ASSERT_TRUE(com0_r.verify());
}

TEST(CommitmentTest, CommTest1)
{
    bytes_t val0({0, 0, 0, 0});
    Commitment com0_s = Commitment::create_open(val0);
    com0_s.commit();
    Commitment com0_r = Commitment::create_hidden(com0_s.send_commitment());

    com0_r.recv_decommitment(com0_s.decommit());

    ASSERT_TRUE(com0_r.verify());
}
