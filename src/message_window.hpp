#ifndef MESSAGE_WINDOW_HPP
#define MESSAGE_WINDOW_HPP

#include <deque>
#include <string>
#include "ncurses_window.hpp"

class MessageWindow : public NCursesWindow
{
public:
    MessageWindow(int starty, int startx, int height, int width);
    ~MessageWindow();
    void post_message(std::string message);
    void update();
private:
    std::deque<std::string> messages_;
};


#endif // MESSAGE_WINDOW_HPP
