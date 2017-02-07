#include <iostream>
#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <beast/core/to_string.hpp>
#include <beast/websocket.hpp>
#include "websocket_connection.hpp"

using boost::asio::ip::tcp;
namespace websocket = beast::websocket;
using websocket_t = beast::websocket::stream<tcp::socket>;

struct WebSocketConnection::WebSocketConnectionImpl
{
    WebSocketConnectionImpl(uint16_t port);
    ~WebSocketConnectionImpl() = default;
    boost::asio::io_service io_service;
    websocket_t ws;

    static websocket_t init_websocket(boost::asio::io_service &io_service, uint16_t port);
};

websocket_t WebSocketConnection::WebSocketConnectionImpl::init_websocket(
        boost::asio::io_service &io_service, uint16_t port)
{
    tcp::socket socket{io_service};
    tcp::endpoint endpoint{ boost::asio::ip::address::from_string("127.0.0.1"), port};
    tcp::acceptor acceptor{socket.get_io_service(), endpoint};
    acceptor.accept(socket);
    websocket_t ws{std::move(socket)};;
    ws.accept();
    std::cerr << "Accepted connection on websocket\n";
    return ws;
}

WebSocketConnection::WebSocketConnectionImpl::WebSocketConnectionImpl(uint16_t port)
    : ws(init_websocket(io_service, port))
{
}


WebSocketConnection::WebSocketConnection(uint16_t port)
    : pImpl(std::make_unique<WebSocketConnectionImpl>(port))
{
}

WebSocketConnection::~WebSocketConnection() = default;

void WebSocketConnection::send_message(std::string data)
{
    auto buffer{boost::asio::buffer(data)};
    pImpl->ws.write(buffer);
}

std::string WebSocketConnection::recv_message()
{
    // throw std::logic_error("WebSocketConnection::recv_message not implemented");
    beast::streambuf buffer;
    websocket::opcode op;
    pImpl->ws.read(op, buffer);
    return beast::to_string(buffer.data());
}
