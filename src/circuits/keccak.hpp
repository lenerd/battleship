#ifndef CIRCUIT_KECCAK_HPP
#define CIRCUIT_KECCAK_HPP

#include <memory>
#include <vector>

class CircuitWrapper;
class share;
using CircuitW_p = std::shared_ptr<CircuitWrapper>;
using share_p = std::shared_ptr<share>;


class StateShare
{
public:
    StateShare(std::vector<share_p> input);
    StateShare(const StateShare &other);
    StateShare& operator=(const StateShare &other);
    share_p& operator()(size_t x, size_t y);
    const share_p& operator()(size_t x, size_t y) const;
    std::array<share_p, 25> data;
};


void build_theta(CircuitW_p circ, StateShare &state, size_t);
void build_rho_pi(CircuitW_p circ, StateShare &state, size_t);
void build_chi(CircuitW_p circ, StateShare &state, size_t);
void build_iota(CircuitW_p circ, StateShare &state, size_t round);
void build_round(CircuitW_p circ, StateShare &state, size_t round);
void build_permutation(CircuitW_p circ, StateShare &state, size_t = 0);


#endif // CIRCUIT_KECCAK_HPP
