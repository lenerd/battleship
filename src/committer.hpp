#ifndef COMMITTER_HPP
#define COMMITTER_HPP

#include <memory>
#include <connection.hpp>
#include "util.hpp"


enum class CommitmentAlgorithm
{
    SHA1,
    NAOR,
};

struct Commitment
{
    virtual ~Commitment() = default;
    bool committed;
    bytes_t value;
};

using Comm_p = std::shared_ptr<Commitment>;

class Committer
{
public:
    Committer(Conn_p conn) : conn_(conn) {}
    virtual ~Committer() = default;

    virtual Comm_p send_commitment(bytes_t) = 0;
    virtual void send_decommitment(const Comm_p&) = 0;

    virtual Comm_p recv_commitment() = 0;
    virtual bool recv_decommitment(Comm_p&) = 0;
private:
    Conn_p conn_;
};

#endif // COMMITTER_HPP
