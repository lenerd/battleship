#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "tcp_connection.hpp"

using boost::asio::ip::tcp;


TCPConnection::TCPConnection(TCPConnection &&conn)
    : socket_(std::move(conn.socket_))
{
}
TCPConnection::TCPConnection(tcp::socket socket)
    : socket_(std::move(socket))
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
    tcp::endpoint endpoint(boost::asio::ip::address::from_string(address),
            static_cast<unsigned short>(std::stoi(port)));
    tcp::acceptor acceptor(socket.get_io_service(), endpoint);
    acceptor.accept(socket);
    return TCPConnection(std::move(socket));
}


void TCPConnection::send_length(size_t length)
{
    if (length > std::numeric_limits<uint32_t>::max())
        throw std::out_of_range("message too long");
    uint32_t header{htonl(static_cast<uint32_t>(length))};
    static_assert(sizeof(header) == header_size, "header size mismatch");
    boost::asio::write(socket_, boost::asio::buffer(&header, sizeof(header)));
}

size_t TCPConnection::recv_length()
{
    uint32_t header{0};
    static_assert(sizeof(header) == header_size, "header size mismatch");
    boost::asio::read(socket_, boost::asio::buffer(&header, sizeof(header)));
    return static_cast<size_t>(ntohl(header));
}

void TCPConnection::send_message(uint8_t* buffer, size_t length)
{
    send_length(length);
    boost::asio::write(socket_, boost::asio::buffer(buffer, length));
}

bytes_t TCPConnection::recv_message()
{
    auto length{recv_length()};
    bytes_t buffer(length);
    boost::asio::read(socket_, boost::asio::buffer(buffer));
    return buffer;
}
