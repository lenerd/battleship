#ifndef GAME_COMMUNICATOR_HPP
#define GAME_COMMUNICATOR_HPP

#include <functional>
#include "connection.hpp"
#include "util.hpp"

class GameCommunicator
{
public:
    GameCommunicator(Conn_p conn);
    virtual ~GameCommunicator() = default;

    virtual void send_query(coords_t coords);
    virtual void send_answer(bool answer);
    virtual coords_t recv_query();
    virtual bool recv_answer();
    virtual void answer_query(std::function<bool(coords_t)> fun);
    virtual bool query_position(coords_t coords);
private:
    bool verify_message(bytes_t buffer);
    bool verify_message(uint8_t *buffer, size_t size);
    bytes_t recv_message_verified();

    Conn_p conn_;
};


# endif // GAME_COMMUNICATOR_HPP
