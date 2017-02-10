#ifndef HASH_COMMITTER_HPP
#define HASH_COMMITTER_HPP

#include <memory>
#include "committer.hpp"

struct HashCommitment : public Commitment
{
    bytes_t padding;
    bytes_t hash;
};

using HashComm_p = std::shared_ptr<HashCommitment>;

class HashCommitter : public Committer
{
public:
    HashCommitter(Conn_p conn) : Committer(conn) {}
    virtual ~HashCommitter() = default;

    virtual Comm_p send_commitment(bytes_t) override;
    virtual void send_decommitment(const Comm_p&) override;

    virtual Comm_p recv_commitment() override;
    virtual bool recv_decommitment(Comm_p&) override;

    Comm_p make_commitment(const bytes_t value) const;
    bool verify_commitment(const Comm_p &comm) const;

private:
    virtual bytes_t hash_function(const bytes_t&, const bytes_t&) const = 0;
};

class SHA1Committer : public HashCommitter
{
public:
    SHA1Committer(Conn_p conn) : HashCommitter(conn) {}
    virtual ~SHA1Committer() = default;
private:
    virtual bytes_t hash_function(const bytes_t&, const bytes_t&) const final override;
};

#endif // HASH_COMMITTER_HPP
