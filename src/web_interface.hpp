#ifndef WEB_INTERFACE_HPP
#define WEB_INTERFACE_HPP

#include <stdexcept>
#include <string>
#include <memory>
#include "user_interface.hpp"

class Board;

class WebInterface : public UserInterface
{
public:
    WebInterface(Board &board_local, Board &board_remote)
        : UserInterface(board_local, board_remote)
    {
        throw std::logic_error("WebInterface not implemented");
    }
    ~WebInterface();
    void show();
    void place_ships();
    void select_position();
    void post_message(std::string message);
private:
};

#endif // WEB_INTERFACE_HPP
