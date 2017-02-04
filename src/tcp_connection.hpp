#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <boost/asio/ip/tcp.hpp>
#include "stream_connection.hpp"

class TCPConnection : public StreamConnection
{
public:
    TCPConnection(TCPConnection &&conn);
    ~TCPConnection() = default;

    static TCPConnection from_role(Role role, boost::asio::io_service &io_service,
        std::string address, std::string port);
    static TCPConnection connect(boost::asio::io_service& io_service,
            std::string address, std::string port);
    static TCPConnection listen(boost::asio::io_service& io_service,
            std::string address, std::string port);

protected:
    void send_message(uint8_t* buffer, size_t length) override;
    bytes_t recv_message() override;
private:

    void send_length(size_t length);
    size_t recv_length();

    const static size_t header_size = 4;
    TCPConnection(boost::asio::ip::tcp::socket socket);
    boost::asio::ip::tcp::socket socket_;
};

#endif // TCP_CONNECTION_HPP
