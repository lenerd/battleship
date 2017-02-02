#include "tcp_connection.hpp"


TCPConnection::TCPConnection(boost::asio::ip::tcp::socket&& socket)
    : socket_(std::move(socket))
{
}

TCPConnection::~TCPConnection()
{
}

void TCPConnection::send_query()
{
}

void TCPConnection::send_answer()
{
}

void TCPConnection::recv_query()
{
}

void TCPConnection::recv_answer()
{
}
