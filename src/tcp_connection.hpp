#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <boost/asio/ip/tcp.hpp>
#include "connection.hpp"

enum class Role;

class TCPConnection : public Connection
{
public:
    TCPConnection(TCPConnection &&conn);
    ~TCPConnection();

    static TCPConnection from_role(Role role, boost::asio::io_service &io_service,
        std::string address, std::string port);
    static TCPConnection connect(boost::asio::io_service& io_service,
            std::string address, std::string port);
    static TCPConnection listen(boost::asio::io_service& io_service,
            std::string address, std::string port);

    void send_query();
    void send_answer();
    void recv_query();
    void recv_answer();
private:
    TCPConnection(boost::asio::ip::tcp::socket socket);
    boost::asio::ip::tcp::socket socket_;
};

#endif // TCP_CONNECTION_HPP
