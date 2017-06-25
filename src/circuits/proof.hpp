#ifndef PROOF_HPP
#define PROOF_HPP

#include <bitset>
#include "crypto/hash_committer.hpp"
#include "misc/options.hpp"

bool proof_sender(std::vector<Comm_p> commitments, Role role=Role::server);
bool proof_receiver(std::vector<Comm_p> commitments, Role role=Role::client);


#endif // PROOF_HPP
