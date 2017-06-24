#ifndef HASH_COMMITTER_HPP
#define HASH_COMMITTER_HPP

#include <memory>
#include "committer.hpp"


/**
 * Represents a commitment by hash function based scheme
 */
struct HashCommitment : public Commitment
{
    bytes_t padding;
    bytes_t hash;
};

using HashComm_p = std::shared_ptr<HashCommitment>;

/**
 * Implementation of a hash function based commitment scheme
 */
class HashCommitter : public Committer
{
public:
    HashCommitter(Conn_p conn) : Committer(conn) {}
    virtual ~HashCommitter() = default;

    virtual Comm_p send_commitment(bool) override;
    virtual void send_decommitment(const Comm_p&) override;

    virtual Comm_p recv_commitment() override;
    virtual bool recv_decommitment(Comm_p&) override;

    /**
     * Create a commitment string
     */
    HashComm_p make_commitment(bool value) const;
    /**
     * Verify a commitment.
     */
    bool verify_commitment(const Comm_p &comm) const;

private:
    virtual bytes_t hash_function(const bytes_t&, const bytes_t&) const = 0;
};


/**
 * Implementation of a commitment scheme using SHA1 as hash function
 */
class SHA1_HashCommitter : public HashCommitter
{
public:
    SHA1_HashCommitter(Conn_p conn) : HashCommitter(conn) {}
    virtual ~SHA1_HashCommitter() = default;
private:
    virtual bytes_t hash_function(const bytes_t&, const bytes_t&) const final override;
};


/**
 * Implementation of a commitment scheme using SHA3-256 as hash function
 */
class SHA3_256_HashCommitter : public HashCommitter
{
public:
    SHA3_256_HashCommitter(Conn_p conn) : HashCommitter(conn) {}
    virtual ~SHA3_256_HashCommitter() = default;
private:
    virtual bytes_t hash_function(const bytes_t&, const bytes_t&) const final override;
};

#endif // HASH_COMMITTER_HPP
