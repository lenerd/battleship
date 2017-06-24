#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <memory>
#include "misc/util.hpp"

/**
 * Interface for a bidirectional channel
 */
class Connection
{
public:
    /**
     * Constructor
     */
    Connection() = default;
    /**
     * Destructor
     */
    virtual ~Connection() = default;
    /**
     * Move constructor
     */
    Connection(Connection&&) = default;
    /**
     * Move assignment
     */
    Connection& operator=(Connection&&) = default;

    /**
     * Send a message
     */
    virtual void send_message(const bytes_t &buffer)
    {
        send_message(buffer.data(), buffer.size());
    }
    /**
     * Send a message from a raw buffer
     */
    virtual void send_message(const uint8_t*, size_t) = 0;
    /**
     * Receive a message
     */
    virtual bytes_t recv_message() = 0;
};

using Conn_p = std::shared_ptr<Connection>;

#endif // CONNECTION_HPP
