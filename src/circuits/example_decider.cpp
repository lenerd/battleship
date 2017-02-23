#include <vector>
#include <abycore/circuit/booleancircuits.h>
#include "aby_party_connection.hpp"
#include "circuit_wrapper.hpp"
#include "example_decider.hpp"
#include "misc/options.hpp"

ExampleDecider::ExampleDecider(Role role, APC_p aby)
    : role_(role), aby_(aby)
{
}

bool ExampleDecider::decide()
{
    assert(role_ == Role::server);
    return decide(std::bitset<25>());
}

bool ExampleDecider::decide(std::bitset<25> input)
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
    aby_->reset();
    return output != 0;
}

share_p ExampleDecider::build_circuit(share_p s_input)
{
    auto coords2idx = [] (size_t row, size_t col)
    {
        return static_cast<uint32_t>(row * 5 + col);
    };
    auto circ{aby_->get_circuit()};
    share_p s_out;
    std::vector<share_p> cell_shares;
    share_p s_one = circ->PutCONSGate(1U, 1);
    for (size_t row = 0; row < 5; ++row)
    {
        for (size_t col = 0; col < 5; ++col)
        {
            share_p s_cell, s_west, s_north_west, s_north, s_north_east;
            share_p s_tmp;

            s_cell = CircuitWrapper::get_wire_ids_as_share(s_input, coords2idx(row, col));

            s_tmp = s_one;
            if (col > 0)
            {
                s_west = CircuitWrapper::get_wire_ids_as_share(s_input, coords2idx(row, col - 1));
                s_west = circ->PutINVGate(s_west);
                s_tmp = circ->PutANDGate(s_tmp, s_west);
            }
            if (row > 0 && col > 0)
            {
                s_north_west = CircuitWrapper::get_wire_ids_as_share(s_input, coords2idx(row - 1, col - 1));
                s_north_west = circ->PutINVGate(s_north_west);
                s_tmp = circ->PutANDGate(s_tmp, s_north_west);
            }
            if (row > 0)
            {
                s_north = CircuitWrapper::get_wire_ids_as_share(s_input, coords2idx(row - 1, col));
                s_north = circ->PutINVGate(s_north);
                s_tmp = circ->PutANDGate(s_tmp, s_north);
            }
            if (row > 0 && col < 4)
            {
                s_north_east = CircuitWrapper::get_wire_ids_as_share(s_input, coords2idx(row - 1, col + 1));
                s_north_east = circ->PutINVGate(s_north_east);
                s_tmp = circ->PutANDGate(s_tmp, s_north_east);
            }

            s_tmp = circ->PutINVGate(s_tmp);
            s_tmp = circ->PutANDGate(s_cell, s_tmp);
            s_tmp = circ->PutINVGate(s_tmp);
            cell_shares.push_back(s_tmp);
        }
    }
    assert(cell_shares.size() == 25);
    s_out = s_one;
    for (auto s : cell_shares)
    {
        s_out = circ->PutANDGate(s_out, s);
    }
    return s_out;
}
