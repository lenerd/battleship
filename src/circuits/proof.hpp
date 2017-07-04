#ifndef PROOF_HPP
#define PROOF_HPP

#include <bitset>
#include "crypto/hash_committer.hpp"
#include "misc/options.hpp"

bool proof_prover(std::vector<Comm_p> commitments, const Options& options, bool inv_aby_role=false, const std::string& inv_aby_address = std::string());
bool proof_verifier(std::vector<Comm_p> commitments, const Options& options, bool inv_aby_role=false, const std::string& inv_aby_address = std::string());


#endif // PROOF_HPP
