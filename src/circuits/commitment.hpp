#ifndef CIRCUIT_COMMITMENT_HPP
#define CIRCUIT_COMMITMENT_HPP

#include <memory>
#include <vector>

class CircuitWrapper;
using CircuitW_p = std::shared_ptr<CircuitWrapper>;
class share;
using share_p = std::shared_ptr<share>;

std::vector<share_p> make_commitment(CircuitW_p circ, share_p value, std::vector<share_p> padding);
share_p verify_commitment(CircuitW_p circ, share_p value, std::vector<share_p> padding, std::vector<share_p> digest);


#endif // CIRCUIT_COMMITMENT_HPP
