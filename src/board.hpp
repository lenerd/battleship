#ifndef BOARD_HPP
#define BOARD_HPP

#include <bitset>
#include <memory>
#include <vector>
#include <boost/signals2.hpp>
#include "util.hpp"


class Board
{
public:
    bool query(coords_t coords);
    void set_queried(coords_t, bool value);
    void mark_queried(coords_t coords);
    bool is_queried(coords_t coords);

    bool get(coords_t coords);
    void set(coords_t coords, bool value = true);
    void reset(coords_t coords);
    void flip(coords_t coords);
    void commit();
    void debug();
    void draw(std::ostream& os);

    bool ships_alive();
    size_t num_ships_hit();

    static const size_t size = 10;
    boost::signals2::signal<void(coords_t)> signal_updated_position;

private:
    size_t coords_to_index(coords_t coords);
    coords_t index_to_coords(size_t n);
    std::bitset<size*size> ships_;
    std::bitset<size*size> queried_;
    // bool committed_;
};

using Board_p = std::shared_ptr<Board>;

#endif // BOARD_HPP
