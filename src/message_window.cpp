#include <iterator>
#include "message_window.hpp"


MessageWindow::MessageWindow(int starty, int startx, int height, int width)
    : NCursesWindow(starty, startx, height, width)
{
}

MessageWindow::~MessageWindow()
{
}


void MessageWindow::post_message(std::string message)
{
    if (message.size() > width_)
    {
        message.replace(std::next(message.begin(), width_ - 3), message.end(), "...");
    }
    else
    {
        message.append(width_ - message.size(), ' ');
    }

    messages_.push_back(message);
    if (messages_.size() > height_)
        messages_.pop_front();
    update();
}

void MessageWindow::update()
{
    auto end = std::next(messages_.begin(), std::min(static_cast<size_t>(height_), messages_.size()));
    for (auto it = messages_.begin(); it != end; ++it)
    {
        mvwaddstr(window_, it - messages_.begin(), 0, it->c_str());
    }
    wrefresh(window_);
}
