#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <memory>
#include <string>
#include "options.hpp"
#include "util.hpp"

class Board;

class UserInterface
{
public:
    UserInterface(Board &board_local, Board &board_remote);
    virtual ~UserInterface();
    virtual void show() = 0;
    virtual void place_ships() = 0;
    virtual coords_t select_position() = 0;
    virtual void post_message(std::string message) = 0;
    virtual void wait_for_quit() = 0;
protected:
    Board &board_local_;
    Board &board_remote_;
};


class UIFactory
{
public:
    UIFactory(uint16_t http_port, uint16_t ws_port);
    ~UIFactory() = default;
    std::unique_ptr<UserInterface> make(UIType type,
            Board &board_local, Board &board_remote) const;
private:
    uint16_t http_port_;
    uint16_t ws_port_;
};

#endif // USER_INTERFACE_HPP
