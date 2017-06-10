#include <unordered_map>
#include <vector>
#include <abycore/circuit/booleancircuits.h>
#include "aby_party_connection.hpp"
#include "battleship_decider.hpp"
#include "circuit_wrapper.hpp"
#include "misc/options.hpp"

using share_grid = std::vector<std::vector<share_p>>;
void debug_board(CircuitW_p circ, share_grid &board);

BattleshipDecider::BattleshipDecider(Role role, APC_p aby)
    : role_(role), aby_(aby)
{
}

bool BattleshipDecider::decide()
{
    assert(role_ == Role::server);
    return decide(std::bitset<100>());
}

bool BattleshipDecider::decide(std::bitset<100> input)
{
    auto circ{aby_->get_circuit()};

    std::vector<share_p> sv_in;

    if (role_ == Role::server)
    {
        sv_in = circ->PutDummyINGates(input.size(), 1);
    }
    else
    {
        sv_in = circ->PutINGates(input, CLIENT);
    }

    auto s_out{build_circuit(sv_in)};
    s_out = circ->PutOUTGate(s_out, ALL);
    aby_->execute();
    auto output{s_out->get_clear_value<uint32_t>()};
    aby_->reset();
    return output != 0;
}

share_grid prepare_board(CircuitW_p circ, std::vector<share_p> sv_input)
{
    assert(sv_input.size() == 100);
    auto coords2idx = [] (size_t row, size_t col)
    {
        assert(row < 10 && col < 10);
        return static_cast<uint32_t>(row * 10 + col);
    };
    share_p s_zero = circ->PutCONSGate(0U, 1);;
    std::vector<std::vector<share_p>> board(12);
    for (size_t col{0}; col < 12; ++col)
    {
        // top padding
        board.at(0).push_back(s_zero);
        // bottom padding
        board.at(11).push_back(s_zero);
    }
    for (size_t row{1}; row < 11; ++row)
    {
        // left padding
        board.at(row).push_back(s_zero);
        for (size_t col{1}; col < 11; ++col)
        {
            board.at(row).push_back(sv_input.at(coords2idx(row - 1, col - 1)));
        }
        // right padding
        board.at(row).push_back(s_zero);
    }
    return board;
}

share_p reduce_and(CircuitW_p circ, std::vector<share_p> shares)
{
    // TODO: reduce depth
    share_p result{circ->PutCONSGate(1U, 1)};
    for (auto& share : shares)
    {
        result = circ->PutANDGate(result, share);
    }
    return result;
}

share_p reduce_add(CircuitW_p circ, std::vector<share_p> shares)
{
    // TODO: reduce depth
    share_p result{circ->PutCONSGate(0U, 1)};
    for (auto& share : shares)
    {
        result = circ->PutADDGate(result, share);
    }
    return result;
}

share_p check_ship_v(CircuitW_p circ, share_grid &board, size_t length, size_t row, size_t col)
{
    assert(0 < row && row + length - 1 < 11);
    assert(0 < col && col < 11);
    std::vector<share_p> shares;
    shares.push_back(circ->PutINVGate(board.at(row - 1).at(col - 1)));
    shares.push_back(circ->PutINVGate(board.at(row - 1).at(col)));
    shares.push_back(circ->PutINVGate(board.at(row - 1).at(col + 1)));

    for (size_t i{0}; i < length; ++i)
    {
        shares.push_back(circ->PutINVGate(board.at(row + i).at(col - 1)));
        shares.push_back(board.at(row + i).at(col));
        shares.push_back(circ->PutINVGate(board.at(row + i).at(col + 1)));
    }

    shares.push_back(circ->PutINVGate(board.at(row + length).at(col - 1)));
    shares.push_back(circ->PutINVGate(board.at(row + length).at(col)));
    shares.push_back(circ->PutINVGate(board.at(row + length).at(col + 1)));

    return reduce_and(circ, shares);
}

share_p check_ship_h(CircuitW_p circ, share_grid &board, size_t length, size_t row, size_t col)
{
    assert(0 < row && row < 11);
    assert(0 < col && col + length - 1 < 11);
    std::vector<share_p> shares;
    shares.push_back(circ->PutINVGate(board.at(row - 1).at(col - 1)));
    shares.push_back(circ->PutINVGate(board.at(row).at(col - 1)));
    shares.push_back(circ->PutINVGate(board.at(row + 1).at(col - 1)));

    for (size_t i{0}; i < length; ++i)
    {
        shares.push_back(circ->PutINVGate(board.at(row - 1).at(col + i)));
        shares.push_back(board.at(row).at(col + i));
        shares.push_back(circ->PutINVGate(board.at(row + 1).at(col + i)));
    }

    shares.push_back(circ->PutINVGate(board.at(row - 1).at(col + length)));
    shares.push_back(circ->PutINVGate(board.at(row).at(col + length)));
    shares.push_back(circ->PutINVGate(board.at(row + 1).at(col + length)));

    return reduce_and(circ, shares);
}

share_p place_ship_mask_v(CircuitW_p circ, share_grid &board, size_t length, size_t row, size_t col)
{
    assert(0 < row && row + length - 1 < 11);
    assert(0 < col && col < 11);
    share_p s_zero{circ->PutCONSGate(0U, 1)};
    auto ship_found{check_ship_v(circ, board, length, row, col)};
    for (size_t i{0}; i < length; ++i)
    {
        board.at(row + i).at(col) = circ->PutMUXGate(s_zero, board.at(row + i).at(col), ship_found);
    }
    return ship_found;
}

share_p place_ship_mask_h(CircuitW_p circ, share_grid &board, size_t length, size_t row, size_t col)
{
    assert(0 < row && row < 11);
    assert(0 < col && col + length - 1 < 11);
    share_p s_zero{circ->PutCONSGate(0U, 1)};
    auto ship_found{check_ship_h(circ, board, length, row, col)};
    for (size_t i{0}; i < length; ++i)
    {
        board.at(row).at(col + i) = circ->PutMUXGate(s_zero, board.at(row).at(col + i), ship_found);
    }
    return ship_found;
}

share_p count_ships(CircuitW_p circ, share_grid &board, size_t length)
{
    share_p count;
    std::vector<share_p> ships;
    // vertical ships
    for (size_t row{1}; row < 11 - length + 1; ++row)
    {
        for (size_t col{1}; col < 11; ++col)
        {
            ships.push_back(place_ship_mask_v(circ, board, length, row, col));
        }
    }
    // horizontal ships
    for (size_t row{1}; row < 11; ++row)
    {
        for (size_t col{1}; col < 11 - length + 1; ++col)
        {
            ships.push_back(place_ship_mask_h(circ, board, length, row, col));
        }
    }
    return reduce_add(circ, ships);
}

void debug_board(CircuitW_p circ, share_grid &board)
{
    for (size_t row{0}; row < 12; ++row)
    {
        for (size_t col{0}; col < 12; ++col)
        {
            std::stringstream ss;
            ss << "[" << std::setfill('0') << std::setw(2) << row << ":" << std::setw(2) << col << "]";
            circ->PutPrintValueGate(board.at(row).at(col), ss.str());
        }
    }
}

share_p board_empty(CircuitW_p circ, share_grid &board)
{
    std::vector<share_p> cells_empty;
    for (size_t row{1}; row < 11; ++row)
    {
        for (size_t col{1}; col < 11; ++col)
        {
            cells_empty.push_back(circ->PutINVGate(board.at(row).at(col)));
        }
    }
    return reduce_and(circ, cells_empty);
}

share_p build_circuit(CircuitW_p circ, std::vector<share_p> sv_input)
{
    assert(sv_input.size() == 100);
    std::unordered_map<uint8_t, uint8_t> target_nums{
        {2, 4}, {3, 3}, {4, 2}, {5, 1}
    };
    auto board{prepare_board(circ, sv_input)};
    // debug_board(circ, board);

    std::vector<share_p> nums_correct;
    for (auto &pair : target_nums)
    {
        auto length{pair.first};
        auto num{pair.second};
        auto s_count{count_ships(circ, board, length)};
        auto s_target{circ->PutCONSGate(num, 8)};
        // circ->PutPrintValueGate(s_count, std::to_string(length));
        nums_correct.push_back(circ->PutEQGate(s_count, s_target));
    }
    // debug_board(circ, board);

    auto all_nums_correct{reduce_and(circ, nums_correct)};
    auto all_empty{board_empty(circ, board)};

    // return circ->PutCONSGate(0U, 1);
    return circ->PutANDGate(all_nums_correct, all_empty);
}


share_p BattleshipDecider::build_circuit(std::vector<share_p> sv_input)
{
    return ::build_circuit(aby_->get_circuit(), sv_input);
}
