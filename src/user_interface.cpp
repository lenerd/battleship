#include <memory>
#include <stdexcept>
#include "user_interface.hpp"
#include "ncurses_interface.hpp"

UserInterface::UserInterface(Board &board_local, Board &board_remote)
    : board_local_(board_local), board_remote_(board_remote)
{}

UserInterface::~UserInterface() {}


std::unique_ptr<UserInterface> make_userinterface(UserInterface::Type type, Board &board_local, Board &board_remote)
{
    switch (type)
    {
        case UserInterface::Type::ncurses:
            return std::make_unique<NCursesInterface>(board_local, board_remote);
        case UserInterface::Type::web:
            throw std::logic_error("web interface not implemented");
    }
}
