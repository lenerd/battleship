#ifndef BOARD_HPP
#define BOARD_HPP

#include <bitset>
#include <memory>
#include <vector>
#include <boost/signals2.hpp>
#include "crypto/committer.hpp"
#include "misc/util.hpp"

/**
 * Representation of a Battleship board
 */
class Board
{
public:
    /**
     * Constructor with committer
     */
    Board(Committer_p committer=nullptr);
    /**
     * Query a position (it is marked as queried)
     */
    bool query(coords_t coords);
    /**
     * Set a position and mark it as queried
     */
    void set_queried(coords_t, bool value);
    /**
     * Mark a position as queried
     */
    void mark_queried(coords_t coords);
    /**
     * Return whether a position was queried
     */
    bool is_queried(coords_t coords);

    /**
     * Get the value at a position
     */
    bool get(coords_t coords);
    /**
     * Set the value at a position
     */
    void set(coords_t coords, bool value = true);
    /**
     * Reset the value at a position
     */
    void reset(coords_t coords);
    /**
     * Invert the value at a position
     */
    void flip(coords_t coords);

    /**
     * Commit to the board
     */
    void send_commitments();
    void recv_commitments();

    /**
     * Decommit/verify position
     */
    void verify_answer(coords_t coords, bool answer);
    void prove_answer(coords_t coords);

    /**
     * Return vector of commitments
     */
    std::vector<Comm_p> get_commitments();

    /**
     * Draw the board to stderr
     */
    void debug();
    /**
     * Draw the board to a stream
     */
    void draw(std::ostream& os);

    /**
     * Check whether there are ships alive
     */
    bool ships_alive();
    /**
     * Count the number of ship parts that are hit
     */
    size_t num_ships_hit();

    /**
     * Size of the board
     */
    static const size_t size = 10;
    /**
     * Signal that is triggered when a position is updated
     */
    boost::signals2::signal<void(coords_t)> signal_updated_position;

private:
    size_t coords_to_index(coords_t coords);
    coords_t index_to_coords(size_t n);
    std::bitset<size*size> ships_;
    std::bitset<size*size> queried_;
    bool committed_;
    Committer_p committer_;
    std::vector<Comm_p> commitments_;
};

using Board_p = std::shared_ptr<Board>;

#endif // BOARD_HPP
