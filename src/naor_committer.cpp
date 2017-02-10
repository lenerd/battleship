#include "naor_committer.hpp"


Comm_p NaorCommitter::send_commitment(bytes_t value)
{
    throw std::logic_error("not implemented");
}

void NaorCommitter::send_decommitment(const Comm_p &comm)
{
    throw std::logic_error("not implemented");
}

Comm_p NaorCommitter::recv_commitment()
{
    throw std::logic_error("not implemented");
}

bool NaorCommitter::recv_decommitment(Comm_p &comm)
{
    throw std::logic_error("not implemented");
}
