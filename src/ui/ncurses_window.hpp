#ifndef NCURSES_WINDOW_HPP
#define NCURSES_WINDOW_HPP

#include <stdexcept>
#include <limits>
#include <ncurses.h>

/**
 * Wrapper of an ncurses window object
 */
class NCursesWindow
{
public:
    /**
     * Constructor with position, and dimensions
     */
    NCursesWindow(int starty, int startx, int height, int width);
    /**
     * Destructor
     */
    ~NCursesWindow();
    /**
     * Return the raw window handle
     */
    WINDOW* get_window();

protected:
    template <typename... Values>
    int print(size_t row, size_t col, const char *fmt, Values... values);
    int addstring(size_t row, size_t col, const char *str);
    int addchar(size_t row, size_t col, char ch);

    WINDOW* window_;
    int starty_;
    int startx_;
    int height_;
    int width_;
private:
};

template <typename... Values>
int NCursesWindow::print(size_t row, size_t col, const char *fmt, Values... values)
{
    if (row > std::numeric_limits<int>::max() || col > std::numeric_limits<int>::max())
    {
        throw std::out_of_range("NCursesWindow::print: index too large for ncurses");
    }
    return mvwprintw(window_, static_cast<int>(row), static_cast<int>(col), fmt, values...);
}

#endif // NCURSES_WINDOW_HPP
