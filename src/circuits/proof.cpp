#include "aby_party_connection.hpp"
#include "battleship_decider.hpp"
#include "circuit_wrapper.hpp"
#include "commitment.hpp"
#include "proof.hpp"
#include "keccak.hpp"
#include "battleship_decider.hpp"
#include "misc/options.hpp"


bool proof_sender(std::vector<Comm_p> commitments, Role role)
{
    auto aby_role{role == Role::server ? SERVER : CLIENT};
    auto aby{std::make_shared<ABYPartyConnection>(role, "127.0.0.1", 6677)};
    auto circ{aby->get_circuit()};
    std::vector<share_p> comms_valid;
    comms_valid.reserve(commitments.size());
    std::vector<uint8_t> bit_values;
    bit_values.reserve(commitments.size());
    size_t i{1};
    for (auto &comm : commitments)
    {
        auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(comm)};
        auto digest_shares{circ->bytes_to_shares(hash_comm->hash)};
        auto padding_shares{circ->PutINGates(hash_comm->padding, aby_role)};
        assert(hash_comm->padding.size() == 16);
        uint8_t value{hash_comm->value ? uint8_t{0x01} : uint8_t{0x00}};
        auto value_share{circ->PutINGate(value, 8, aby_role)};

        comms_valid.push_back(verify_commitment(circ, value_share, padding_shares, digest_shares));
        bit_values.push_back(value);
        // std::cerr << "Comm " << i << ": " << circ->circ_->m_cCircuit->GetGateHead() << "\n";
        std::cerr << "Comm " << i << "\n";
        ++i;
    }
    auto all_comms_valid{circ->PutReduceANDGates(comms_valid)};
    auto bit_shares{circ->PutINGates(bit_values, 1, aby_role)};
    auto board_valid{build_circuit(circ, bit_shares)};
    auto result{circ->PutANDGate(all_comms_valid, board_valid)};
    auto output{circ->PutOUTGate(result, ALL)};

    std::cerr << "Executing ...\n";
    aby->execute();
    std::cerr << "Done\n";
    auto final_result{output->get_clear_value<uint8_t>()};
    assert(final_result == 0x00 || final_result == 0x01);
    return final_result == 0x01;
}


bool proof_receiver(std::vector<Comm_p> commitments, Role role)
{
    auto aby_role{role == Role::server ? SERVER : CLIENT};
    auto aby{std::make_shared<ABYPartyConnection>(role, "127.0.0.1", 6677)};
    auto circ{aby->get_circuit()};
    std::vector<share_p> comms_valid;
    comms_valid.reserve(commitments.size());
    size_t i{1};
    for (auto &comm : commitments)
    {
        auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(comm)};
        auto digest_shares{circ->bytes_to_shares(hash_comm->hash)};
        auto padding_shares{circ->PutDummyINGates(16, 8)};
        auto value_share{circ->PutDummyINGate(8)};

        comms_valid.push_back(verify_commitment(circ, value_share, padding_shares, digest_shares));
        // std::cerr << "Comm " << i << ": " << circ->circ_->m_cCircuit->GetGateHead() << "\n";
        std::cerr << "Comm " << i << "\n";
        ++i;
    }
    auto all_comms_valid{circ->PutReduceANDGates(comms_valid)};
    auto bit_shares{circ->PutDummyINGates(commitments.size(), 1)};
    auto board_valid{build_circuit(circ, bit_shares)};
    auto result{circ->PutANDGate(all_comms_valid, board_valid)};
    auto output{circ->PutOUTGate(result, ALL)};

    // std::cerr << "Complete: " << circ->circ_->m_cCircuit->GetGateHead() << "\n";
    std::cerr << "Complete\n";

    std::cerr << "Executing ...\n";
    aby->execute();
    std::cerr << "Done\n";
    auto final_result{output->get_clear_value<uint8_t>()};
    assert(final_result == 0x00 || final_result == 0x01);
    return final_result == 0x01;
}
