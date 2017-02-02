#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "tcp_connection.hpp"
#include "util.hpp"

using boost::asio::ip::tcp;


TCPConnection::TCPConnection(TCPConnection &&conn)
    : socket_(std::move(conn.socket_))
{
}
TCPConnection::TCPConnection(tcp::socket socket)
    : socket_(std::move(socket))
{
}

TCPConnection::~TCPConnection()
{
}

TCPConnection TCPConnection::from_role(Role role, boost::asio::io_service &io_service,
        std::string address, std::string port)
{
    switch (role)
    {
        case Role::server:
            return listen(io_service, address, port);
        case Role::client:
            return connect(io_service, address, port);
    }
}

TCPConnection TCPConnection::connect(boost::asio::io_service &io_service,
        std::string address, std::string port)
{
    tcp::socket socket(io_service);
    tcp::resolver resolver(socket.get_io_service());
    boost::asio::connect(socket, resolver.resolve({address, port}));
    return TCPConnection(std::move(socket));
}

TCPConnection TCPConnection::listen(boost::asio::io_service& io_service,
        std::string address, std::string port)
{
    tcp::socket socket(io_service);
    tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), std::stoi(port));
    tcp::acceptor acceptor(socket.get_io_service(), endpoint);
    acceptor.accept(socket);
    return TCPConnection(std::move(socket));
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
