#include <memory>
#include <stdexcept>
#include "user_interface.hpp"
#include "ncurses_interface.hpp"
#include "web_interface.hpp"

UserInterface::UserInterface(Board_p board_local, Board_p board_remote)
    : board_local_(board_local), board_remote_(board_remote)
{}

UserInterface::~UserInterface() {}


UIFactory::UIFactory(uint16_t http_port, uint16_t ws_port)
    : http_port_(http_port), ws_port_(ws_port)
{
}

UI_p UIFactory::make(UIType type, Board_p board_local, Board_p board_remote)
    const
{
    switch (type)
    {
        case UIType::ncurses:
            return std::make_unique<NCursesInterface>(board_local, board_remote);
        case UIType::web:
            return std::make_unique<WebInterface>(board_local, board_remote, http_port_, ws_port_);
    }
}
