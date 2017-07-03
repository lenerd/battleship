#include <algorithm>
#include <numeric>
#include "circuit_wrapper.hpp"
#include "commitment.hpp"
#include "keccak.hpp"


std::vector<share_p> make_commitment(CircuitW_p circ, share_p value, std::vector<share_p> padding)
{
    assert(padding.size() == 16);
    std::vector<share_p> input_shares;
    input_shares.reserve(17);
    input_shares.push_back(value);
    input_shares.insert(input_shares.cend(), padding.cbegin(), padding.cend());
    auto digest_shares{build_sha3_256(circ, input_shares)};
    return digest_shares;
}


share_p verify_commitment(CircuitW_p circ, share_p value, std::vector<share_p> padding,
                          std::vector<share_p> digest)
{
    auto cmp{make_commitment(circ, value, padding)};
    assert(cmp.size() == digest.size());
    std::vector<share_p> bytes_eq(digest.size());
    std::transform(cmp.cbegin(), cmp.cend(), digest.cbegin(), bytes_eq.begin(),
            [circ] (share_p ina, share_p inb) -> share_p
            { return circ->PutEQGate(ina, inb); });
    assert(bytes_eq.size() == cmp.size());
    return circ->PutReduceANDGates(bytes_eq);
}
