#ifndef EXAMPLE_DECIDER_HPP
#define EXAMPLE_DECIDER_HPP

#include <bitset>
#include "aby_party_connection.hpp"

enum class Role;
class share;
using share_p = std::shared_ptr<share>;


class ExampleDecider //: public LangDecider<25>
{
public:
    ExampleDecider(Role, APC_p);
    bool decide();
    bool decide(std::bitset<25>);
private:
    share_p build_circuit(share_p input);

    Role role_;
    APC_p aby_;
};


#endif // EXAMPLE_DECIDER_HPP
