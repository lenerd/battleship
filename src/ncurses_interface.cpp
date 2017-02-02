#include <iostream>
#include "ncurses.h"
#include "ncurses_interface.hpp"

NCursesInterface::NCursesInterface(Board &board_local, Board &board_remote)
    : UserInterface(board_local, board_remote)
{
    // init ncurses
    initscr();
    cbreak();
    curs_set(0);
    noecho();
    game_screen_ = std::make_unique<GameScreen>(board_local, board_remote);
}


NCursesInterface::~NCursesInterface()
{
    endwin();
}

void NCursesInterface::show()
{
    std::cerr << "shows ui\n";
}

void NCursesInterface::place_ships()
{
    game_screen_->place_ships();
}

void NCursesInterface::select_position()
{
}

void NCursesInterface::post_message(std::string message)
{
    game_screen_->post_message(message);
}
