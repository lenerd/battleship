#include <stdexcept>
#include <ncurses.h>
#include "ncurses_window.hpp"

NCursesWindow::NCursesWindow(int starty, int startx, int height, int width)
    : starty_(starty), startx_(startx), height_(height), width_(width)
{
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
