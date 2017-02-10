#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <memory>
#include "util.hpp"

class Connection
{
public:
    Connection() = default;
    virtual ~Connection() = default;
    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

    virtual void send_message(const bytes_t &buffer)
    {
        send_message(buffer.data(), buffer.size());
    }
    virtual void send_message(const uint8_t*, size_t) = 0;
    virtual bytes_t recv_message() = 0;
};

using Conn_p = std::shared_ptr<Connection>;

#endif // CONNECTION_HPP
