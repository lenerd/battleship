#ifndef NCURSES_WINDOW_HPP
#define NCURSES_WINDOW_HPP

#include <ncurses.h>

class NCursesWindow
{
public:
    NCursesWindow(int starty, int startx, int height, int width);
    ~NCursesWindow();
    WINDOW* get_window();

protected:
    WINDOW* window_;
    int starty_;
    int startx_;
    int height_;
    int width_;
private:
};

#endif // NCURSES_WINDOW_HPP
