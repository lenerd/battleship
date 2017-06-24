#ifndef MESSAGE_WINDOW_HPP
#define MESSAGE_WINDOW_HPP

#include <deque>
#include <string>
#include "ncurses_window.hpp"

/**
 * Part of the ncurses ui that displays messages
 */
class MessageWindow : public NCursesWindow
{
public:
    /**
     * Constructor with position, and dimensions
     */
    MessageWindow(int starty, int startx, int height, int width);
    /**
     * Destructor
     */
    ~MessageWindow();
    /**
     * Add a new message
     */
    void post_message(std::string message);
    /**
     * Update the window
     */
    void update();
private:
    std::deque<std::string> messages_;
};


#endif // MESSAGE_WINDOW_HPP
