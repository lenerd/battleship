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
    using permutation_builder = std::function<void(CircuitW_p, StateShare&, size_t)>;
    StateShare(CircuitW_p circ, permutation_builder, size_t capacity_bytes);

    StateShare(CircuitW_p circ, permutation_builder, size_t capacity_bytes, std::vector<share_p> initial);
    StateShare(const StateShare &other);
    StateShare& operator=(const StateShare &other);
    share_p& operator()(size_t x, size_t y);
    const share_p& operator()(size_t x, size_t y) const;

    void absorb(const std::vector<share_p> &input);
    void absorb(std::vector<share_p>::const_iterator first);

    std::vector<share_p> read();
    void read(std::vector<share_p>::iterator out_first);
    std::vector<share_p> squeeze();
    void squeeze(std::vector<share_p>::iterator out_first);


    CircuitW_p circ;
    permutation_builder permutation;
    size_t capacity_bytes;
    size_t rate_bytes;
    std::array<share_p, 25> data;
};

std::vector<share_p> build_keccak(CircuitW_p circ, std::vector<share_p> input_shares, size_t capacity_bits, uint8_t d, size_t output_bits);
std::vector<share_p> build_sha3_256(CircuitW_p circ, std::vector<share_p> input_shares);
std::vector<share_p> build_sha3_512(CircuitW_p circ, std::vector<share_p> input_shares);
std::vector<share_p> build_padding(CircuitW_p circ, std::vector<share_p> input_shares);

void build_theta(CircuitW_p circ, StateShare &state, size_t);
void build_rho_pi(CircuitW_p circ, StateShare &state, size_t);
void build_chi(CircuitW_p circ, StateShare &state, size_t);
void build_iota(CircuitW_p circ, StateShare &state, size_t round);
void build_round(CircuitW_p circ, StateShare &state, size_t round);
void build_permutation(CircuitW_p circ, StateShare &state, size_t = 0);

void build_absorb(CircuitW_p circ, StateShare &state, std::vector<share_p> input);
void build_squeeze(CircuitW_p circ, StateShare &state);


#endif // CIRCUIT_KECCAK_HPP
