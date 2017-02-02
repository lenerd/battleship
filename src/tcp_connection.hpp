#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <boost/asio/ip/tcp.hpp>
#include "connection.hpp"

class TCPConnection : public Connection
{
public:
    TCPConnection(boost::asio::ip::tcp::socket&& socket);
    ~TCPConnection();

    void send_query();
    void send_answer();
    void recv_query();
    void recv_answer();
private:
    boost::asio::ip::tcp::socket socket_;
};

#endif // TCP_CONNECTION_HPP
