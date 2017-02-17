#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include "naor_committer.hpp"
#include "commitment_generated.h"


Comm_p NaorCommitter::send_commitment(bool value)
{
    bytes_t rand;
   
    // recv from bob
    {
        bytes_t buffer(recv_message_verified());
        auto root{comm_fbs::GetRoot(buffer.data())};
        auto commitment{root->commitment()};
        if (commitment->commitment_type() != comm_fbs::CommitmentImpl::NaorCommitment)
        {
            throw std::runtime_error("expected NaorCommitment");
        }
        auto naor_commitment{static_cast<const comm_fbs::NaorCommitment*>(commitment->commitment())};
        auto rand_buf{naor_commitment->rand()};
        rand = bytes_t(rand_buf->data(), rand_buf->data() + rand_buf->size());
    }

    auto comm(make_commitment(value, rand));

    flatbuffers::FlatBufferBuilder builder;
    {
        auto commitment_vec{builder.CreateVector(comm->commitment.data(), comm->commitment.size())};
        auto naor_comm_builder{comm_fbs::NaorCommitmentBuilder(builder)};
        naor_comm_builder.add_commitment(commitment_vec);
        auto naor_comm{naor_comm_builder.Finish()};
        auto comm_builder{comm_fbs::CommitmentBuilder(builder)};
        comm_builder.add_commitment(naor_comm.Union());
        comm_builder.add_commitment_type(comm_fbs::CommitmentImpl::NaorCommitment);
        auto comm_buf{comm_builder.Finish()};
        auto root{comm_fbs::CreateRoot(builder, comm_buf)};
        builder.Finish(root);
    }

    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());

    return comm;
}


void NaorCommitter::send_decommitment(const Comm_p &comm)
{
    auto naor_comm{std::dynamic_pointer_cast<NaorCommitment>(comm)};
    // send seed
    flatbuffers::FlatBufferBuilder builder;
    {
        auto seed_vec{builder.CreateVector(naor_comm->seed.data(), naor_comm->seed.size())};
        auto naor_comm_builder{comm_fbs::NaorCommitmentBuilder(builder)};
        naor_comm_builder.add_seed(seed_vec);
        auto naor_comm_buf{naor_comm_builder.Finish()};
        auto comm_builder{comm_fbs::CommitmentBuilder(builder)};
        comm_builder.add_value(naor_comm->value);
        comm_builder.add_commitment(naor_comm_buf.Union());
        comm_builder.add_commitment_type(comm_fbs::CommitmentImpl::NaorCommitment);
        auto comm_buf{comm_builder.Finish()};
        auto root{comm_fbs::CreateRoot(builder, comm_buf)};
        builder.Finish(root);
    }

    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());
}

Comm_p NaorCommitter::recv_commitment()
{
    auto comm{std::make_shared<NaorCommitment>()};
    comm->rand = random_bytes(48);


    // send rand
    flatbuffers::FlatBufferBuilder builder;
    {
        auto rand_vec{builder.CreateVector(comm->rand.data(), comm->rand.size())};
        auto naor_comm_builder{comm_fbs::NaorCommitmentBuilder(builder)};
        naor_comm_builder.add_rand(rand_vec);
        auto naor_comm{naor_comm_builder.Finish()};
        auto comm_builder{comm_fbs::CommitmentBuilder(builder)};
        comm_builder.add_commitment(naor_comm.Union());
        comm_builder.add_commitment_type(comm_fbs::CommitmentImpl::NaorCommitment);
        auto comm_buf{comm_builder.Finish()};
        auto root{comm_fbs::CreateRoot(builder, comm_buf)};
        builder.Finish(root);
    }

    assert(verify_message(builder.GetBufferPointer(), builder.GetSize()));
    conn_->send_message(builder.GetBufferPointer(), builder.GetSize());

    // recv commitment
    {
        bytes_t buffer(recv_message_verified());
        auto root{comm_fbs::GetRoot(buffer.data())};
        auto commitment{root->commitment()};
        if (commitment->commitment_type() != comm_fbs::CommitmentImpl::NaorCommitment)
        {
            throw std::runtime_error("expected NaorCommitment");
        }
        auto naor_commitment{static_cast<const comm_fbs::NaorCommitment*>(commitment->commitment())};
        auto comm_buf{naor_commitment->commitment()};
        comm->commitment = bytes_t(comm_buf->data(), comm_buf->data() + comm_buf->size());
    }


    return comm;
}

bool NaorCommitter::recv_decommitment(Comm_p &comm)
{
    auto naor_comm{std::dynamic_pointer_cast<NaorCommitment>(comm)};
    // recv from alice
    {
        bytes_t buffer(recv_message_verified());
        auto root{comm_fbs::GetRoot(buffer.data())};
        auto commitment{root->commitment()};
        if (commitment->commitment_type() != comm_fbs::CommitmentImpl::NaorCommitment)
        {
            throw std::runtime_error("expected NaorCommitment");
        }
        naor_comm->value = commitment->value();
        auto naor_commitment{static_cast<const comm_fbs::NaorCommitment*>(commitment->commitment())};
        auto seed_buf{naor_commitment->seed()};
        naor_comm->seed = bytes_t(seed_buf->data(), seed_buf->data() + seed_buf->size());
    }
    return verify_commitment(comm);
}

NaorComm_p NaorCommitter::make_commitment(bool value, bytes_t rand) const
{
    auto comm{std::make_shared<NaorCommitment>()};
    comm->value = value;
    comm->rand = rand;
    comm->seed = random_bytes(16);
    comm->commitment = bytes_t(48);
    bytes_t pseudo_seq{pseudo_gen(comm->seed, 48)};
    auto bit_vec{value ? uint8_t{0xff} : uint8_t{0x00}};
    for (size_t i{0}; i < 48; ++i)
    {
        comm->commitment[i] = pseudo_seq[i] ^ (bit_vec & rand[i]);
    }
    return comm;
}

bool NaorCommitter::verify_commitment(const Comm_p &comm) const
{
    auto naor_comm{std::dynamic_pointer_cast<NaorCommitment>(comm)};
    bytes_t comm_cmp(48);
    bytes_t pseudo_seq{pseudo_gen(naor_comm->seed, 48)};
    auto bit_vec{comm->value ? uint8_t{0xff} : uint8_t{0x00}};
    for (size_t i{0}; i < 48; ++i)
    {
        comm_cmp[i] = pseudo_seq[i] ^ (bit_vec & naor_comm->rand[i]);
    }
    return std::equal(naor_comm->commitment.cbegin(), naor_comm->commitment.cend(),
                      comm_cmp.cbegin(), comm_cmp.cend());
}

bytes_t CTR_NaorCommitter::pseudo_gen(const bytes_t &seed, size_t length) const
{
    bytes_t pseudo_seq(length);
    bytes_t zero_bytes(16);
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption prng;
    prng.SetKeyWithIV(seed.data(), seed.size(), zero_bytes.data(), zero_bytes.size());
    prng.CryptoPP::RandomNumberGenerator::GenerateBlock(pseudo_seq.data(), pseudo_seq.size());
    return pseudo_seq;
}
