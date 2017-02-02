#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <memory>
#include <string>

class Board;

class UserInterface
{
public:
    enum class Type
    {
        ncurses,
        web,
    };
    UserInterface(Board &board_local, Board &board_remote);
    virtual ~UserInterface();
    virtual void show() = 0;
    virtual void place_ships() = 0;
    virtual void select_position() = 0;
    virtual void post_message(std::string message) = 0;
protected:
    Board &board_local_;
    Board &board_remote_;
};


std::unique_ptr<UserInterface> make_userinterface(UserInterface::Type type, Board &board_local, Board &board_remote);

#endif // USER_INTERFACE_HPP
