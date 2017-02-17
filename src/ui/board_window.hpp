#ifndef BOARD_WINDOW_HPP
#define BOARD_WINDOW_HPP

#include <memory>
#include <ncurses.h>
#include "game/board.hpp"
#include "misc/util.hpp"
#include "ncurses_window.hpp"


class BoardWindow : public NCursesWindow
{
public:
    BoardWindow(int starty, int startx, Board_p board);
    ~BoardWindow();
    void draw_grid();
    std::pair<size_t, size_t> index_to_win(size_t row, size_t col) const;
    int get_height() const;
    int get_width() const;

    char get_char(size_t row, size_t col) const;

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();

    bool cursor_toggle();
    void cursor_update(size_t row, size_t col);
    void cursor_show();
    void cursor_hide();
    void update_position(size_t row, size_t col);
    void toggle_position(size_t row, size_t col);

    coords_t select_position();
    void place_ships();

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
