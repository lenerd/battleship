#include <cassert>
#include "dummy_connection.hpp"

DummyConnection::DummyConnection(message_queue_t send_queue, message_queue_t recv_queue)
    : send_queue_(send_queue), recv_queue_(recv_queue) {}

DummyConnection::~DummyConnection() = default;

std::pair<Conn_p, Conn_p> DummyConnection::make_dummies()
{
    auto queue_12{std::make_shared<message_queue_t::element_type>()};
    auto queue_21{std::make_shared<message_queue_t::element_type>()};
    auto conn1{std::make_shared<DummyConnection>(queue_12, queue_21)};
    auto conn2{std::make_shared<DummyConnection>(queue_21, queue_12)};
    assert(conn1->send_queue_ == conn2->recv_queue_);
    assert(conn1->recv_queue_ == conn2->send_queue_);
    return {conn1, conn2};
}


void DummyConnection::send_message(const uint8_t *buffer, size_t size)
{
    send_queue_->enqueue(bytes_t(buffer, buffer + size));
}

bytes_t DummyConnection::recv_message()
{
    return recv_queue_->dequeue();
}
