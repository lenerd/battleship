#include <cstddef>
#include <vector>
#include "decider_example_0.hpp"
#include "abycore/circuit/booleancircuits.h"

share* build_bs0_circuit(share *s_board, BooleanCircuit *bc)
{
    share* out;
    std::vector<share*> cell_shares;
    share *s_one = bc->PutCONSGate(1, 1);;
    for (size_t row = 0; row < 5; ++row)
    {
        for (size_t col = 0; col < 5; ++col)
        {
            share *s_cell, *s_west, *s_north_west, *s_north, *s_north_east;
            share *s_tmp;

            s_cell = s_board->get_wire_ids_as_share(row * 5 + col);

            s_tmp = s_one;
            if (col > 0)
            {
                s_west = s_board->get_wire_ids_as_share(row + (col - 1));
                s_west = bc->PutINVGate(s_west);
                s_tmp = bc->PutANDGate(s_tmp, s_west);
            }
            if (row > 0 && col > 0)
            {
                s_north_west = s_board->get_wire_ids_as_share((row - 1) + (col - 1));
                s_north_west = bc->PutINVGate(s_north_west);
                s_tmp = bc->PutANDGate(s_tmp, s_north_west);
            }
            if (row > 0)
            {
                s_north = s_board->get_wire_ids_as_share((row - 1) + col);
                s_north = bc->PutINVGate(s_north);
                s_tmp = bc->PutANDGate(s_tmp, s_north);
            }
            if (row > 0 && col < 4)
            {
                s_north_east = s_board->get_wire_ids_as_share((row - 1) + (col + 1));
                s_north_east = bc->PutINVGate(s_north_east);
                s_tmp = bc->PutANDGate(s_tmp, s_north_east);
            }

            s_tmp = bc->PutINVGate(s_tmp);
            s_tmp = bc->PutANDGate(s_cell, s_tmp);
            s_tmp = bc->PutINVGate(s_tmp);
            cell_shares.push_back(s_tmp);
        }
    }
    assert(cell_shares.size() == 25);
    out = s_one;
    for (auto s : cell_shares)
    {
        out = bc->PutANDGate(out, s);
    }
    return out;
}
