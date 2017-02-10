#ifndef NAOR_COMMITTER_HPP
#define NAOR_COMMITTER_HPP

#include <memory>
#include <connection.hpp>
#include <committer.hpp>
#include "util.hpp"


struct NaorCommitment : public Commitment
{
    // TODO
};


class NaorCommitter : public Committer
{
public:
    NaorCommitter(Conn_p conn) : Committer(conn) {}
    virtual ~NaorCommitter() = default;

    // sender of commitment
    virtual Comm_p send_commitment(bytes_t) override;
    virtual void send_decommitment(const Comm_p&) override;

    // receiver of commitment
    virtual Comm_p recv_commitment() override;
    virtual bool recv_decommitment(Comm_p&) override;
private:
    Conn_p conn_;
};

#endif // NAOR_COMMITTER_HPP
