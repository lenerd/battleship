#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <memory>
#include <string>
#include "game/board.hpp"
#include "misc/options.hpp"
#include "misc/util.hpp"

class UserInterface
{
public:
    UserInterface(Board_p board_local, Board_p board_remote);
    virtual ~UserInterface();
    virtual void show() = 0;
    virtual void place_ships() = 0;
    virtual coords_t select_position() = 0;
    virtual void post_message(std::string message) = 0;
    virtual void wait_for_quit() = 0;
protected:
    Board_p board_local_;
    Board_p board_remote_;
};

using UI_p = std::unique_ptr<UserInterface>;


class UIFactory
{
public:
    UIFactory(const Options &options);
    ~UIFactory() = default;
    UI_p make(UIType type, Board_p board_local, Board_p board_remote) const;
private:
    const Options& options_;
};

#endif // USER_INTERFACE_HPP
