#include <iostream>
#include "board.hpp"

bool Board::query(coords_t coords)
{
    mark_queried(coords);
    return get(coords);
}

void Board::mark_queried(coords_t coords)
{
    queried_.set(foo(coords));
}

bool Board::is_queried(coords_t coords)
{
    return queried_.test(foo(coords));
}

bool Board::get(coords_t coords)
{
    return ships_.test(foo(coords));
}

void Board::set(coords_t coords)
{
    ships_.set(foo(coords));
}

void Board::reset(coords_t coords)
{
    Board::ships_.reset(foo(coords));
}

void Board::flip(coords_t coords)
{
    ships_.flip(foo(coords));
}

void Board::commit()
{
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
            os << ships_[foo({row, col})];
        }
        os << "\n";
    }
}

int Board::foo(coords_t coords)
{
    return size*coords.first + coords.second;
}

coords_t Board::bar(int n)
{
    return {n / size, n % size};
}
