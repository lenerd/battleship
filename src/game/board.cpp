#include <iostream>
#include "board.hpp"
#include "except.hpp"

Board::Board(Committer_p committer)
    : committer_(committer)
{
}

bool Board::query(coords_t coords)
{
    mark_queried(coords);
    auto value{get(coords)};
    signal_updated_position(coords);
    return value;
}

void Board::set_queried(coords_t coords, bool value)
{
    set(coords, value);
    mark_queried(coords);
    signal_updated_position(coords);
}

void Board::mark_queried(coords_t coords)
{
    queried_.set(coords_to_index(coords));
    signal_updated_position(coords);
}


bool Board::is_queried(coords_t coords)
{
    return queried_.test(coords_to_index(coords));
}

bool Board::get(coords_t coords)
{
    return ships_.test(coords_to_index(coords));
}

void Board::set(coords_t coords, bool value)
{
    ships_.set(coords_to_index(coords), value);
    signal_updated_position(coords);
}

void Board::reset(coords_t coords)
{
    Board::ships_.reset(coords_to_index(coords));
    signal_updated_position(coords);
}

void Board::flip(coords_t coords)
{
    ships_.flip(coords_to_index(coords));
    signal_updated_position(coords);
}

void Board::send_commitments()
{
    if (committer_ == nullptr)
        return;

    commitments_.resize(100);
    for (size_t i{0}; i < 100; ++i)
    {
        commitments_[i] = committer_->send_commitment(ships_.test(i));
    }
    committed_ = true;
}

void Board::recv_commitments()
{
    if (committer_ == nullptr)
        return;

    commitments_.resize(100);
    for (size_t i{0}; i < 100; ++i)
    {
        commitments_[i] = committer_->recv_commitment();
    }
    committed_ = true;
}

void Board::debug()
{
    draw(std::cerr);
}

void Board::draw(std::ostream& os)
{
    for (size_t row = 0; row < size; ++row)
    {
        for (size_t col = 0; col < size; ++col)
        {
            os << ships_[coords_to_index({row, col})];
        }
        os << "\n";
    }
}

size_t Board::coords_to_index(coords_t coords)
{
    return size*coords.first + coords.second;
}

coords_t Board::index_to_coords(size_t n)
{
    return {n / size, n % size};
}

bool Board::ships_alive()
{
    return (ships_ & ~queried_).any();
}

size_t Board::num_ships_hit()
{
    return (ships_ & queried_).count();
}
