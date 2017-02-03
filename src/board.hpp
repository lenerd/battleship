#ifndef BOARD_HPP
#define BOARD_HPP

#include <bitset>
#include <vector>
#include "commitment.hpp"
#include "util.hpp"


class Board
{
public:
    static const size_t size = 10;
    bool query(coords_t coords);
    void mark_queried(coords_t coords);
    bool is_queried(coords_t coords);
    bool get(coords_t coords);
    void set(coords_t coords);
    void reset(coords_t coords);
    void flip(coords_t coords);
    void commit();
    void debug();
    void draw(std::ostream& os);
private:
    size_t coords_to_index(coords_t coords);
    coords_t index_to_coords(size_t n);
    std::bitset<size*size> ships_;
    std::bitset<size*size> queried_;
    // bool committed_;
    std::vector<Commitment> commitments;
};

#endif // BOARD_HPP
