#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <boost/asio/ip/tcp.hpp>
#include "connection.hpp"

enum class Role;

class TCPConnection : public Connection
{
public:
    TCPConnection(boost::asio::ip::tcp::socket socket);
    ~TCPConnection() = default;

    TCPConnection(TCPConnection&&) = default;
    TCPConnection& operator=(TCPConnection&&) = default;

    static Conn_p from_role(Role role, boost::asio::io_service &io_service,
        std::string address, uint16_t port);
    static Conn_p connect(boost::asio::io_service& io_service,
            std::string address, uint16_t port);
    static Conn_p listen(boost::asio::io_service& io_service,
            std::string address, uint16_t port);

    virtual void send_message(const uint8_t * buffer, size_t length) override;
    virtual bytes_t recv_message() override;
private:

    void send_length(size_t length);
    size_t recv_length();

    const static size_t header_size = 4;
    boost::asio::ip::tcp::socket socket_;
};

#endif // TCP_CONNECTION_HPP
