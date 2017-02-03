#include <limits>
#include <stdexcept>
#include <ncurses.h>
#include "ncurses_window.hpp"

NCursesWindow::NCursesWindow(int starty, int startx, int height, int width)
    : starty_(starty), startx_(startx), height_(height), width_(width)
{
    if (starty < 0 || startx < 0 || height < 0 || width < 0)
        throw std::out_of_range("invalid value for window position / dimension");

    window_ = newwin(height, width, starty, startx);
    if (window_ == NULL)
        throw std::runtime_error("cannot initialize ncurses window");
}


NCursesWindow::~NCursesWindow()
{
    delwin(window_);
}


WINDOW* NCursesWindow::get_window()
{
    return window_;
}

int NCursesWindow::addstring(size_t row, size_t col, const char *str)
{
    if (row > std::numeric_limits<int>::max() || col > std::numeric_limits<int>::max())
    {
        throw std::out_of_range("printw: index too large for ncurses");
    }
    return mvwaddstr(window_, static_cast<int>(row), static_cast<int>(col), str);
}

int NCursesWindow::addchar(size_t row, size_t col, char ch)
{
    if (row > std::numeric_limits<int>::max() || col > std::numeric_limits<int>::max())
    {
        throw std::out_of_range("printw: index too large for ncurses");
    }
    return mvwaddch(window_, static_cast<int>(row), static_cast<int>(col), ch);
}
