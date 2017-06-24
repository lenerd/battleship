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

/**
 * Represents a commitment
 */
struct Commitment
{
    virtual ~Commitment() = default;
    bool value;

    bool committed;
    bool decommitted;
};

using Comm_p = std::shared_ptr<Commitment>;


/**
 * Abstraction of a commitment scheme
 */
class Committer
{
public:
    /**
     * Constructor, gets access to a connection object.
     */
    Committer(Conn_p conn) : conn_(conn) {}
    /**
     * Destructor
     */
    virtual ~Committer() = default;

    /**
     * Make a commitment (sender)
     */
    virtual Comm_p send_commitment(bool) = 0;
    /**
     * Send a decommitment (sender)
     */
    virtual void send_decommitment(const Comm_p&) = 0;

    /**
     * Receive a commitment (receiver)
     */
    virtual Comm_p recv_commitment() = 0;
    /**
     * Receive a decommitment (receiver)
     */
    virtual bool recv_decommitment(Comm_p&) = 0;


protected:
    bytes_t recv_message_verified();
    bool verify_message(bytes_t buffer);
    bool verify_message(uint8_t *buffer, size_t size);

    Conn_p conn_;
};


using Committer_p = std::shared_ptr<Committer>;

#endif // COMMITTER_HPP
