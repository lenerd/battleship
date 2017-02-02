#ifndef BOARD_WINDOW_HPP
#define BOARD_WINDOW_HPP

#include <memory>
#include <ncurses.h>
#include "board.hpp"
#include "util.hpp"
#include "ncurses_window.hpp"


class BoardWindow : public NCursesWindow
{
public:
    BoardWindow(int starty, int startx, Board& board);
    ~BoardWindow();
    void draw_grid();
    std::pair<int, int> idx2win(int row, int col) const;
    int get_height() const;
    int get_width() const;

    char get_char(int row, int col) const;

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();

    bool cursor_toggle();
    void cursor_update(int row, int col);
    void cursor_draw(int row, int col);
    void cursor_remove(int row, int col);
    void cursor_show();
    void cursor_hide();
    void update_position(int row, int col);
    void toggle_position(int row, int col);

    coords_t select_position();
    void place_ships();

    static const int offset_left = 2;
    static const int offset_top = 1;
    static const int cell_width = 4;
    static const int cell_height = 2;
private:
    Board& board_;
    int cursor_y_;
    int cursor_x_;
    bool display_cursor_;
};

#endif // BOARD_WINDOW_HPP
