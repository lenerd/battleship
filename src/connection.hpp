#ifndef CONNECTION_HPP
#define CONNECTION_HPP

class Connection
{
public:
    Connection() {};
    virtual ~Connection() {};

    virtual void send_query() = 0;
    virtual void send_answer() = 0;
    virtual void recv_query() = 0;
    virtual void recv_answer() = 0;
};


# endif // CONNECTION_HPP
