#include <future>
#include <memory>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <gtest/gtest.h>
#include "dummy_connection.hpp"
#include "game_communicator.hpp"

class GameCommunicatorTest : public ::testing::Test
{
protected:
    GameCommunicatorTest()
    {
        auto conn_pair{DummyConnection::make_dummies()};
        comm1 = std::make_unique<GameCommunicator>(conn_pair.first);
        comm2 = std::make_unique<GameCommunicator>(conn_pair.second);

        gen_values();
    }
    virtual ~GameCommunicatorTest() = default;

    void gen_values()
    {
        values.emplace(std::make_pair(0, 0), true);
        values.emplace(std::make_pair(4, 2), false);
        values.emplace(std::make_pair(4, 7), true);
        values.emplace(std::make_pair(1, 3), false);
        values.emplace(std::make_pair(3, 7), true);
        values.emplace(std::make_pair(9, 9), false);
    }

    std::unique_ptr<GameCommunicator> comm1;
    std::unique_ptr<GameCommunicator> comm2;
    std::unordered_map<coords_t, bool, boost::hash<coords_t>> values;
};


TEST_F(GameCommunicatorTest, QueryTest)
{
    for (auto& pair : values)
    {
        auto original{pair.first};
        comm1->send_query(original);
        auto received{comm2->recv_query()};
        ASSERT_EQ(original, received);
    }
}


TEST_F(GameCommunicatorTest, AnswerTest)
{
    for (auto& pair : values)
    {
        auto original{pair.second};
        comm1->send_answer(original);
        auto received{comm2->recv_answer()};
        ASSERT_EQ(original, received);
    }
}


TEST_F(GameCommunicatorTest, QueryPositionTest)
{
    for (auto& pair : values)
    {
        auto original_pos{pair.first};
        auto original_val{pair.second};
        auto fut{std::async(std::launch::async,
            [this, original_pos]
            { return this->comm2->query_position(original_pos); })};
        auto query{comm1->recv_query()};
        comm1->send_answer(original_val);
        auto answer{fut.get()};
        ASSERT_EQ(original_pos, query);
        ASSERT_EQ(original_val, answer);
    }
}


TEST_F(GameCommunicatorTest, AnswerQueryTest)
{
    for (auto& pair : values)
    {
        auto original_pos{pair.first};
        auto original_val{pair.second};
        auto answer_fun{[this] (auto query)
            { return this->values.at(query); }};
        auto fut{std::async(std::launch::async,
            [this, answer_fun]
            { this->comm2->answer_query(answer_fun); })};
        comm1->send_query(original_pos);
        auto answer{comm1->recv_answer()};
        fut.get();
        ASSERT_EQ(original_val, answer);
    }
}

TEST_F(GameCommunicatorTest, QueryAnswerTest)
{
    for (auto& pair : values)
    {
        auto original_pos{pair.first};
        auto original_val{pair.second};

        auto fut_q{std::async(std::launch::async,
            [this, original_pos]
            { return this->comm1->query_position(original_pos); })};

        auto answer_fun{[this] (auto query)
            { return this->values.at(query); }};
        auto fut_a{std::async(std::launch::async,
            [this, answer_fun]
            { this->comm2->answer_query(answer_fun); })};

        auto answer{fut_q.get()};
        fut_a.get();

        ASSERT_EQ(original_val, answer);
    }
}

TEST_F(GameCommunicatorTest, WrongMessages)
{
    comm1->send_query({4, 7});
    ASSERT_ANY_THROW(comm2->recv_answer());
    comm1->send_answer(true);
    ASSERT_ANY_THROW(comm2->recv_query());
}
