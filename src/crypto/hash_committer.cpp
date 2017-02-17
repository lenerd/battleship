#include <algorithm>
#include <memory>
#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include "hash_committer.hpp"
#include "commitment_generated.h"


Comm_p HashCommitter::send_commitment(bool value)
{
    auto comm{make_commitment(value)};

    flatbuffers::FlatBufferBuilder builder;
    auto hash_vec{builder.CreateVector(comm->hash.data(), comm->hash.size())};
    auto hash_comm_builder{comm_fbs::HashCommitmentBuilder(builder)};
    hash_comm_builder.add_hash(hash_vec);
    auto hash_comm{hash_comm_builder.Finish()};
    auto comm_builder{comm_fbs::CommitmentBuilder(builder)};
    comm_builder.add_commitment(hash_comm.Union());
    comm_builder.add_commitment_type(comm_fbs::CommitmentImpl::HashCommitment);
    auto comm_buf{comm_builder.Finish()};
    auto root{comm_fbs::CreateRoot(builder, comm_buf)};
    builder.Finish(root);

    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());

    // serialize and send hash
    return comm;
}

void HashCommitter::send_decommitment(const Comm_p &comm)
{
    auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(comm)};

    flatbuffers::FlatBufferBuilder builder;
    auto padding_vec{builder.CreateVector(hash_comm->padding.data(), hash_comm->padding.size())};
    auto hash_comm_builder{comm_fbs::HashCommitmentBuilder(builder)};
    hash_comm_builder.add_padding(padding_vec);
    auto hash_comm_buf{hash_comm_builder.Finish()};
    auto comm_buf{comm_fbs::CreateCommitment(builder, comm->value, comm_fbs::CommitmentImpl::HashCommitment, hash_comm_buf.Union())};
    auto root{comm_fbs::CreateRoot(builder, comm_buf)};
    builder.Finish(root);
    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));

    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());
}

Comm_p HashCommitter::recv_commitment()
{
    bytes_t buffer(recv_message_verified());
    auto root{comm_fbs::GetRoot(buffer.data())};
    auto commitment{root->commitment()};
    if (commitment->commitment_type() != comm_fbs::CommitmentImpl::HashCommitment)
    {
        throw std::runtime_error("expected HashCommitment");
    }
    auto hash_commitment{static_cast<const comm_fbs::HashCommitment*>(commitment->commitment())};
    auto hash_comm{std::make_shared<HashCommitment>()};
    auto hash_buf = hash_commitment->hash();
    hash_comm->hash = bytes_t(hash_buf->data(), hash_buf->data() + hash_buf->size());
    return hash_comm;
}

bool HashCommitter::recv_decommitment(Comm_p &comm)
{
    auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(comm)};

    bytes_t buffer(recv_message_verified());
    auto root{comm_fbs::GetRoot(buffer.data())};
    auto commitment{root->commitment()};
    if (commitment->commitment_type() != comm_fbs::CommitmentImpl::HashCommitment)
    {
        throw std::runtime_error("expected HashCommitment");
    }
    hash_comm->value = commitment->value();
    auto hash_commitment{static_cast<const comm_fbs::HashCommitment*>(commitment->commitment())};
    auto padding_buf = hash_commitment->padding();
    hash_comm->padding = bytes_t(padding_buf->data(), padding_buf->data() + padding_buf->size());

    return verify_commitment(comm);
}


HashComm_p HashCommitter::make_commitment(bool value) const
{
    auto comm{std::make_shared<HashCommitment>()};
    comm->padding = random_bytes(16);
    bytes_t byte_value{value ? uint8_t{0xff} : uint8_t{0x00}};
    comm->hash = hash_function(byte_value, comm->padding);
    comm->committed = true;
    comm->value = value;
    return comm;
}

bool HashCommitter::verify_commitment(const Comm_p &comm) const
{
    auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(comm)};
    assert(!hash_comm->hash.empty());
    assert(!hash_comm->padding.empty());

    bytes_t byte_value{comm->value ? uint8_t{0xff} : uint8_t{0x00}};
    auto comp{hash_function(byte_value, hash_comm->padding)};
    return std::equal(hash_comm->hash.cbegin(), hash_comm->hash.cend(),
                      comp.cbegin(), comp.cend());
}

bytes_t SHA1_HashCommitter::hash_function(const bytes_t &data, const bytes_t &padding) const
{
    CryptoPP::SHA1 sha1;
    bytes_t digest(sha1.DigestSize());
    sha1.Update(data.data(), data.size());
    sha1.Update(padding.data(), padding.size());
    sha1.Final(digest.data());
    return digest;
}

bytes_t SHA3_HashCommitter::hash_function(const bytes_t &data, const bytes_t &padding) const
{
    CryptoPP::SHA3_256 sha3;
    bytes_t digest(sha3.DigestSize());
    sha3.Update(data.data(), data.size());
    sha3.Update(padding.data(), padding.size());
    sha3.Final(digest.data());
    return digest;
}
