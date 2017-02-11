#include <cassert>
#include <stdexcept>
#include "game_generated.h"
#include "game_communicator.hpp"
#include "util.hpp"

GameCommunicator::GameCommunicator(Conn_p conn)
    : conn_(conn)
{
}


void GameCommunicator::send_query(coords_t coords)
{
    flatbuffers::FlatBufferBuilder builder;
    auto query{game_fbs::CreateQuery(builder, coords.first, coords.second)};
    auto root{game_fbs::CreateRoot(builder, game_fbs::Message::Query, query.Union())};
    builder.Finish(root);
    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());
}

void GameCommunicator::send_answer(bool value)
{
    flatbuffers::FlatBufferBuilder builder;
    auto answer{game_fbs::CreateAnswer(builder, value)};
    auto root{game_fbs::CreateRoot(builder, game_fbs::Message::Answer, answer.Union())};
    builder.Finish(root);
    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());
}

coords_t GameCommunicator::recv_query()
{
    bytes_t buffer(recv_message_verified());
    auto root{game_fbs::GetRoot(buffer.data())};
    if (root->message_type() != game_fbs::Message::Query)
    {
        throw std::runtime_error("expected query");
    }
    auto query{static_cast<const game_fbs::Query*>(root->message())};
    return {query->row(), query->col()};
}

bool GameCommunicator::recv_answer()
{
    bytes_t buffer(recv_message_verified());
    auto root{game_fbs::GetRoot(buffer.data())};
    if (root->message_type() != game_fbs::Message::Answer)
    {
        throw std::runtime_error("expected answer");
    }
    auto answer{static_cast<const game_fbs::Answer*>(root->message())};
    return answer->value();
}

void GameCommunicator::answer_query(std::function<bool(coords_t)> fun)
{
    auto coords{recv_query()};
    send_answer(fun(coords));
}

bool GameCommunicator::query_position(coords_t coords)
{
    send_query(coords);
    return recv_answer();
}


bytes_t GameCommunicator::recv_message_verified()
{
    bytes_t buffer(conn_->recv_message());
    if (!verify_message(buffer))
    {
        throw std::runtime_error("received malformed buffer");
    }
    return buffer;
}

bool GameCommunicator::verify_message(bytes_t buffer)
{
    return verify_message(buffer.data(), buffer.size());
}

bool GameCommunicator::verify_message(uint8_t *buffer, size_t size)
{
    flatbuffers::Verifier verifier(buffer, size);
    return game_fbs::VerifyRootBuffer(verifier);
}

