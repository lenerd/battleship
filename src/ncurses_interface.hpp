#ifndef NCURSES_INTERFACE_HPP
#define NCURSES_INTERFACE_HPP

#include <string>
#include <memory>
#include "board.hpp"
#include "game_screen.hpp"
#include "user_interface.hpp"


class NCursesInterface : public UserInterface
{
public:
    NCursesInterface(Board_p board_local, Board_p board_remote, const Options&);
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
