#ifndef BATTLESHIP_DECIDER_HPP
#define BATTLESHIP_DECIDER_HPP

#include <bitset>
#include <vector>
#include "aby_party_connection.hpp"
#include "trivial_decider.hpp"

enum class Role;
class CircuitWrapper;
using CircuitW_p = std::shared_ptr<CircuitWrapper>;
class share;
using share_p = std::shared_ptr<share>;


class BattleshipDecider
{
public:
    BattleshipDecider(Role role, APC_p aby);
    bool decide();
    bool decide(std::bitset<100>);
private:
    share_p build_circuit(std::vector<share_p> input);

    Role role_;
    APC_p aby_;
};


share_p build_circuit(CircuitW_p circ, std::vector<share_p> sv_input);


#endif // BATTLESHIP_DECIDER_HPP
