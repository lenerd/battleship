#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <functional>
#include "util.hpp"

class Connection
{
public:
    Connection() = default;
    virtual ~Connection() = default;

    virtual void send_query(coords_t coords) = 0;
    virtual void send_answer(bool answer) = 0;
    virtual coords_t recv_query() = 0;
    virtual bool recv_answer() = 0;
    virtual void answer_query(std::function<bool(coords_t)> fun) = 0;
    virtual bool query_position(coords_t coords) = 0;
};


# endif // CONNECTION_HPP
