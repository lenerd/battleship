#ifndef BOARD_WINDOW_HPP
#define BOARD_WINDOW_HPP

#include <memory>
#include <ncurses.h>
#include "game/board.hpp"
#include "misc/util.hpp"
#include "ncurses_window.hpp"


/**
 * Display a board in an ncurses ui
 */
class BoardWindow : public NCursesWindow
{
public:
    /**
     * Constructor with given coordinates and board
     */
    BoardWindow(int starty, int startx, Board_p board);
    /**
     * Destructor
     */
    ~BoardWindow();
    void draw_grid();
    /**
     * Calculates window coordinates from board coordinates
     */
    std::pair<size_t, size_t> index_to_win(size_t row, size_t col) const;
    /**
     * Return height of the window
     */
    int get_height() const;
    /**
     * Return width of the window
     */
    int get_width() const;

    char get_char(size_t row, size_t col) const;

    /**
     * Cursor movements
     */
    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();

    /**
     * Toggle cursor display
     */
    bool cursor_toggle();
    /**
     * Update the position of the cursor
     */
    void cursor_update(size_t row, size_t col);
    /**
     * Show the cursor
     */
    void cursor_show();
    /**
     * Hide the cursor
     */
    void cursor_hide();
    /**
     * Change entry at position
     */
    void update_position(size_t row, size_t col);
    /**
     * Invert entry at position
     */
    void toggle_position(size_t row, size_t col);

    /**
     * Ask the user to select a position
     */
    coords_t select_position();
    /**
     * Ask the user to place its ships
     */
    void place_ships();

    /**
     * Some constants regarding the cell layout
     */
    static const int offset_left = 2;
    static const int offset_top = 1;
    static const int cell_width = 4;
    static const int cell_height = 2;
private:
    Board_p board_;
    size_t cursor_y_;
    size_t cursor_x_;
    bool display_cursor_;
};

#endif // BOARD_WINDOW_HPP
