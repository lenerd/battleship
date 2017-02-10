#ifndef DUMMY_CONNECTION_HPP
#define DUMMY_CONNECTION_HPP

#include "connection.hpp"
#include "queue.hpp"
#include "util.hpp"

class DummyConnection : public Connection
{
public:
    using message_queue_t = std::shared_ptr<Queue<bytes_t>>;

    static std::pair<Conn_p, Conn_p> make_dummies();

    DummyConnection(message_queue_t send_queue, message_queue_t recv_queue);
    virtual ~DummyConnection();

    DummyConnection(DummyConnection&&) = default;
    DummyConnection& operator=(DummyConnection&&) = default;

    virtual void send_message(const uint8_t*, size_t) override;
    virtual bytes_t recv_message() override;

// private:
    message_queue_t send_queue_;
    message_queue_t recv_queue_;
};

#endif // DUMMY_CONNECTION_HPP
