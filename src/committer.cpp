#include "committer.hpp"
#include "commitment_generated.h"

bytes_t Committer::recv_message_verified()
{
    bytes_t buffer(conn_->recv_message());
    if (!verify_message(buffer))
    {
        throw std::runtime_error("received malformed buffer");
    }
    return buffer;
}

bool Committer::verify_message(bytes_t buffer)
{
    return verify_message(buffer.data(), buffer.size());
}

bool Committer::verify_message(uint8_t *buffer, size_t size)
{
    flatbuffers::Verifier verifier(buffer, size);
    return comm_fbs::VerifyRootBuffer(verifier);
}
