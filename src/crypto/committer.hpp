#ifndef COMMITTER_HPP
#define COMMITTER_HPP

#include <memory>
#include "network/connection.hpp"
#include "misc/util.hpp"


enum class CommitmentAlgorithm
{
    SHA1,
    NAOR,
};

struct Commitment
{
    virtual ~Commitment() = default;
    bool value;

    bool committed;
    bool decommitted;
};

using Comm_p = std::shared_ptr<Commitment>;

class Committer
{
public:
    Committer(Conn_p conn) : conn_(conn) {}
    virtual ~Committer() = default;

    virtual Comm_p send_commitment(bool) = 0;
    virtual void send_decommitment(const Comm_p&) = 0;

    virtual Comm_p recv_commitment() = 0;
    virtual bool recv_decommitment(Comm_p&) = 0;


protected:
    bytes_t recv_message_verified();
    bool verify_message(bytes_t buffer);
    bool verify_message(uint8_t *buffer, size_t size);

    Conn_p conn_;
};

#endif // COMMITTER_HPP
