#ifndef GAME_COMMUNICATOR_HPP
#define GAME_COMMUNICATOR_HPP

#include <functional>
#include "network/connection.hpp"
#include "misc/util.hpp"


/**
 * Handler for the games communication
 */
class GameCommunicator
{
public:
    /**
     * Constructor
     */
    GameCommunicator(Conn_p conn);
    /**
     * Destructor
     */
    virtual ~GameCommunicator() = default;

    /**
     * Send a query
     */
    virtual void send_query(coords_t coords);
    /**
     * Send the answer to a query
     */
    virtual void send_answer(bool answer);
    /**
     * Receive a query
     */
    virtual coords_t recv_query();
    /**
     * Receive the answer to a query
     */
    virtual bool recv_answer();
    /**
     * Answer a query with a callback function
     */
    virtual coords_t answer_query(std::function<bool(coords_t)> fun);
    /**
     * Query a position and get the answer
     */
    virtual bool query_position(coords_t coords);
private:
    bool verify_message(bytes_t buffer);
    bool verify_message(uint8_t *buffer, size_t size);
    bytes_t recv_message_verified();

    Conn_p conn_;
};


# endif // GAME_COMMUNICATOR_HPP
