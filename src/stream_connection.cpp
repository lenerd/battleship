#include <cassert>
#include <stdexcept>
#include "message_generated.h"
#include "stream_connection.hpp"

#include <iostream>


void StreamConnection::send_query(coords_t coords)
{
    flatbuffers::FlatBufferBuilder builder;
    auto query{fbs::CreateQuery(builder, coords.first, coords.second)};
    auto root{fbs::CreateRoot(builder, fbs::Message::Query, query.Union())};
    builder.Finish(root);
    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    send_message(builder.GetBufferPointer(), builder.GetSize());
}

void StreamConnection::send_answer(bool value)
{
    flatbuffers::FlatBufferBuilder builder;
    auto answer{fbs::CreateAnswer(builder, value)};
    auto root{fbs::CreateRoot(builder, fbs::Message::Answer, answer.Union())};
    builder.Finish(root);
    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    send_message(builder.GetBufferPointer(), builder.GetSize());
}

coords_t StreamConnection::recv_query()
{
    bytes_t buffer(recv_message_verified());
    auto root{fbs::GetRoot(buffer.data())};
    if (root->message_type() != fbs::Message::Query)
    {
        throw std::runtime_error("expected query");
    }
    auto query{static_cast<const fbs::Query*>(root->message())};
    return {query->row(), query->col()};
}

bool StreamConnection::recv_answer()
{
    bytes_t buffer(recv_message_verified());
    auto root{fbs::GetRoot(buffer.data())};
    if (root->message_type() != fbs::Message::Answer)
    {
        throw std::runtime_error("expected answer");
    }
    auto answer{static_cast<const fbs::Answer*>(root->message())};
    return answer->value();
}

void StreamConnection::answer_query(std::function<bool(coords_t)> fun)
{
    auto coords{recv_query()};
    send_answer(fun(coords));
}

bool StreamConnection::query_position(coords_t coords)
{
    send_query(coords);
    return recv_answer();
}


bytes_t StreamConnection::recv_message_verified()
{
    bytes_t buffer(recv_message());
    if (!verify_message(buffer))
    {
        throw std::runtime_error("received malformed buffer");
    }
    return buffer;
}

bool StreamConnection::verify_message(bytes_t buffer)
{
    return verify_message(buffer.data(), buffer.size());
}

bool StreamConnection::verify_message(uint8_t *buffer, size_t size)
{
    flatbuffers::Verifier verifier(buffer, size);
    return fbs::VerifyRootBuffer(verifier);
}

