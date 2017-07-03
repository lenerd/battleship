#ifndef PROOF_HPP
#define PROOF_HPP

#include <bitset>
#include "crypto/hash_committer.hpp"
#include "misc/options.hpp"

bool proof_sender(std::vector<Comm_p> commitments, const Options& options);
bool proof_receiver(std::vector<Comm_p> commitments, const Options& options);


#endif // PROOF_HPP
