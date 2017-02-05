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
    coords_t select_position();
    void post_message(std::string message);
    void wait_for_quit();
private:
};

#endif // WEB_INTERFACE_HPP
