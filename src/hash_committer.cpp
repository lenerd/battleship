#include <algorithm>
#include <memory>
#include <cryptopp/sha.h>
#include "hash_committer.hpp"
#include <util.hpp>


Comm_p HashCommitter::send_commitment(bytes_t value)
{
    auto comm{make_commitment(value)};
    throw std::logic_error("not implemented");
    // serialize and send hash
    return comm;
}

void HashCommitter::send_decommitment(const Comm_p &comm)
{
    throw std::logic_error("not implemented");
    // serialize and send padding, value
}

Comm_p HashCommitter::recv_commitment()
{
    throw std::logic_error("not implemented");
    // recv and deserialize hash
}

bool HashCommitter::recv_decommitment(Comm_p &comm)
{
    throw std::logic_error("not implemented");
    // recv and deserialize padding, value
    return verify_commitment(comm);
}


Comm_p HashCommitter::make_commitment(const bytes_t value) const
{
    auto comm{std::make_shared<HashCommitment>()};
    comm->padding = random_bytes(16);
    comm->hash = hash_function(value, comm->padding);
    comm->committed = true;
    comm->value = std::move(value);
    return comm;
}

bool HashCommitter::verify_commitment(const Comm_p &comm) const
{
    auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(comm)};
    auto comp{hash_function(hash_comm->value, hash_comm->padding)};
    return std::equal(hash_comm->hash.cbegin(), hash_comm->hash.cend(),
                      comp.cbegin(), comp.cend());
}

bytes_t SHA1Committer::hash_function(const bytes_t &data, const bytes_t &padding) const
{
    CryptoPP::SHA1 sha1;
    bytes_t digest(sha1.DigestSize());
    sha1.Update(data.data(), data.size());
    sha1.Update(padding.data(), padding.size());
    sha1.Final(digest.data());
    return digest;
}
