#ifndef NAOR_COMMITTER_HPP
#define NAOR_COMMITTER_HPP

#include <memory>
#include "committer.hpp"


/**
 * Represents a commitment by naor's scheme.
 */
struct NaorCommitment : public Commitment
{
    bytes_t seed;
    bytes_t rand;
    bytes_t commitment;
};

using NaorComm_p = std::shared_ptr<NaorCommitment>;


/**
 * Implementation of Naors commitment scheme without a specifig prg.
 */
class NaorCommitter : public Committer
{
public:
    NaorCommitter(Conn_p conn) : Committer(conn) {}
    virtual ~NaorCommitter() = default;

    // sender of commitment
    virtual Comm_p send_commitment(bool) override;
    virtual void send_decommitment(const Comm_p&) override;

    // receiver of commitment
    virtual Comm_p recv_commitment() override;
    virtual bool recv_decommitment(Comm_p&) override;

    /**
     * Verify a commitment.
     */
    bool verify_commitment(const Comm_p &comm) const;

    /**
     * Create a commitment string given randomness.
     */
    NaorComm_p make_commitment(bool value, bytes_t rand) const;

private:
    virtual bytes_t pseudo_gen(const bytes_t &seed, size_t length) const = 0;
};


/**
 * Implementation of Naors commitment scheme with AES in CTR mode as prg.
 */
class CTR_NaorCommitter : public NaorCommitter
{
public:
    CTR_NaorCommitter(Conn_p conn) : NaorCommitter(conn) {}
    virtual ~CTR_NaorCommitter() = default;
private:
    virtual bytes_t pseudo_gen(const bytes_t &seed, size_t length) const final override;
};

#endif // NAOR_COMMITTER_HPP
