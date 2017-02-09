#ifndef NCURSES_INTERFACE_HPP
#define NCURSES_INTERFACE_HPP

#include <string>
#include <memory>
#include "game_screen.hpp"
#include "user_interface.hpp"

class Board;

class NCursesInterface : public UserInterface
{
public:
    NCursesInterface(Board &board_local, Board &board_remote);
    ~NCursesInterface();
    void show();
    void place_ships();
    coords_t select_position();
    void post_message(std::string message);
    void wait_for_quit();
private:
    std::unique_ptr<GameScreen> game_screen_;
};

#endif // NCURSES_INTERFACE_HPP
