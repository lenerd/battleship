#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <ncurses.h>
#include "board_window.hpp"

BoardWindow::BoardWindow(int starty, int startx, Board& board)
    : NCursesWindow(starty, startx, 1 + 2 * board.size + 1, 2 + 4 * board.size + 1),
      board_(board), cursor_y_(0), cursor_x_(0)
{
    board_.signal_updated_position.connect([this] (coords_t coords) { this->update_position(coords.first, coords.second); });
}

BoardWindow::~BoardWindow()
{
}


void BoardWindow::draw_grid()
{
    auto size{board_.size};
    for (size_t col = 0; col < size; ++col)
    {
        print(0, offset_left + cell_width*col + 2, "%c", '0' + col);
    }
    for (size_t row = 0; row < size; ++row)
    {
        print(offset_top + cell_height*row+1, 0, "%c", 'A' + row);
        for (size_t col = 0; col < size; ++col)
        {
            addstring(offset_top + cell_height*row, offset_left + cell_width*col, "----");
            addstring(offset_top + cell_height*row+1, 2 + cell_width*col, "|   ");
        }
        addchar(offset_top + cell_height*row, offset_left + cell_width*size, '-');
        addchar(offset_top + cell_height*row+1, offset_left + cell_width*size, '|');
    }
    for (size_t col = 0; col < size; ++col)
    {
        addstring(offset_top + cell_height*size, offset_left + cell_width*col, "----");
    }
    addchar(offset_top + cell_height*size, offset_left + cell_width*size, '-');
    wrefresh(window_);
}

std::pair<size_t, size_t> BoardWindow::index_to_win(size_t row, size_t col) const
{
    size_t win_row{offset_top + cell_height * row + 1};
    size_t win_col{offset_left + cell_width * col + 2};
    return {win_row, win_col};
}

int BoardWindow::get_height() const
{
    return height_;
}

int BoardWindow::get_width() const
{
    return width_;
}

char BoardWindow::get_char(size_t row, size_t col) const
{
    if (board_.get({row, col}))
    {
        if (board_.is_queried({row, col}))
        {
            return 'X';
        }
        else
        {
            return 'O';
        }
    }
    else
    {
        if (board_.is_queried({row, col}))
        {
            return '~';
        }
        else
        {
            return ' ';
        }
    }
}

void BoardWindow::cursor_update(size_t row, size_t col)
{
    std::cerr << board_.size << '\n';
    std::cerr << cursor_y_ << "," << cursor_x_
        << " -> " << row << "," << col << "\n";
    auto old_y{cursor_y_};
    auto old_x{cursor_x_};
    cursor_y_ = row;
    cursor_x_ = col;
    update_position(old_y, old_x);
    update_position(row, col);
}

void BoardWindow::cursor_show()
{
    display_cursor_ = true;
    update_position(cursor_y_, cursor_x_);
}

void BoardWindow::cursor_hide()
{
    display_cursor_ = false;
    update_position(cursor_y_, cursor_x_);
}

bool BoardWindow::cursor_toggle()
{
    if (display_cursor_)
    {
        cursor_hide();
    }
    else
    {
        cursor_show();
    }
    return display_cursor_;
}

void BoardWindow::cursor_up()
{
    cursor_update((cursor_y_ - 1 + board_.size) % board_.size, cursor_x_);
}

void BoardWindow::cursor_down()
{
    cursor_update((cursor_y_ + 1) % board_.size, cursor_x_);
}

void BoardWindow::cursor_left()
{
    cursor_update(cursor_y_, (cursor_x_ - 1 + board_.size) % board_.size);
}

void BoardWindow::cursor_right()
{
    cursor_update(cursor_y_, (cursor_x_ + 1) % board_.size);
}

void BoardWindow::update_position(size_t row, size_t col)
{
    auto window_pos = index_to_win(row, col);
    auto draw_cursor{display_cursor_ && cursor_y_ == row && cursor_x_ == col};

    if (draw_cursor)
        wattron(window_, A_REVERSE);

    addchar(window_pos.first, window_pos.second, get_char(row, col));

    if (draw_cursor)
        wattroff(window_, A_REVERSE);

    wrefresh(window_);
}

void BoardWindow::toggle_position(size_t row, size_t col)
{
    board_.flip({row, col});
}

void BoardWindow::place_ships()
{
    int ch;
    cursor_show();
    // while ((ch = wgetch(get_window())) != '\n')
    while ((ch = std::cin.get()) != '\r')
    {
        switch (ch)
        {
            // case KEY_UP:
            case 'k':
                cursor_up();
                std::cerr << "up\n";
                break;
            // case KEY_DOWN:
            case 'j':
                cursor_down();
                std::cerr << "down\n";
                break;
            // case KEY_RIGHT:
            case 'l':
                cursor_right();
                std::cerr << "right\n";
                break;
            // case KEY_LEFT:
            case 'h':
                cursor_left();
                std::cerr << "left\n";
                break;
            case ' ':
                toggle_position(cursor_y_, cursor_x_);
                std::cerr << "space\n";
                break;
            default:
                std::cerr << "nothing: " << ch << "\n";
        }
    }
    cursor_hide();
}


coords_t BoardWindow::select_position()
{
    int ch;
    cursor_show();
    // while ((ch = wgetch(get_window())) != '\n')
    while ((ch = std::cin.get()) != ' ')
    {
        switch (ch)
        {
            // case KEY_UP:
            case 'k':
                cursor_up();
                std::cerr << "up\n";
                break;
            // case KEY_DOWN:
            case 'j':
                cursor_down();
                std::cerr << "down\n";
                break;
            // case KEY_RIGHT:
            case 'l':
                cursor_right();
                std::cerr << "right\n";
                break;
            // case KEY_LEFT:
            case 'h':
                cursor_left();
                std::cerr << "left\n";
                break;
            default:
                std::cerr << "nothing: " << ch << "\n";
        }
    }
    cursor_hide();
    return {cursor_y_, cursor_x_};
}
