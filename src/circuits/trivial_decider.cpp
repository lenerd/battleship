#include <vector>
#include <abycore/circuit/booleancircuits.h>
#include "aby_party_connection.hpp"
#include "circuit_wrapper.hpp"
#include "trivial_decider.hpp"
#include "misc/options.hpp"

TrivialDecider::TrivialDecider(Role role, APC_p aby)
    : role_(role), aby_(aby)
{
}

bool TrivialDecider::decide()
{
    assert(role_ == Role::server);
    return decide(std::bitset<25>());
}

bool TrivialDecider::decide(std::bitset<25> input)
{
    auto circ{aby_->get_circuit()};

    share_p s_in;

    if (role_ == Role::server)
    {
        s_in = circ->PutDummyINGate(input.size());
    }
    else
    {
        s_in = circ->PutINGate(input.to_ulong(), input.size(), CLIENT);
    }

    auto s_out{build_circuit(s_in)};
    s_out = circ->PutOUTGate(s_out, ALL);
    aby_->execute();
    auto output{s_out->get_clear_value<uint32_t>()};
    // aby_->reset();
    return output != 0;
}

share_p TrivialDecider::build_circuit(share_p s_input)
{
    (void)s_input;
    auto circ{aby_->get_circuit()};
    share_p s_out = circ->PutCONSGate(1U, 1);
    return s_out;
}
