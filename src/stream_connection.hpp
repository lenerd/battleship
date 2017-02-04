#ifndef STREAM_CONNECTION_HPP
#define STREAM_CONNECTION_HPP
#include "connection.hpp"

class StreamConnection : public Connection
{
public:
    StreamConnection() = default;
    virtual ~StreamConnection() = default;
    void send_query(coords_t coords) override;
    void send_answer(bool answer) override;
    coords_t recv_query() override;
    bool recv_answer() override;
    void answer_query(std::function<bool(coords_t)> fun) override;
    bool query_position(coords_t coords) override;

protected:
    virtual void send_message(uint8_t* buffer, size_t length) = 0;
    virtual bytes_t recv_message() = 0;
    bytes_t recv_message_verified();
private:
    bool verify_message(bytes_t buffer);
    bool verify_message(uint8_t *buffer, size_t size);
};

#endif // STREAM_CONNECTION_HPP
