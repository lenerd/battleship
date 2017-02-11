#include <iostream>
#include "ncurses.h"
#include "ncurses_interface.hpp"

NCursesInterface::NCursesInterface(Board_p board_local, Board_p board_remote, const Options&)
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

coords_t NCursesInterface::select_position()
{
    return game_screen_->select_position();
}

void NCursesInterface::post_message(std::string message)
{
    game_screen_->post_message(message);
}

void NCursesInterface::wait_for_quit()
{
    post_message("press 'q' to quit");
    int ch;
    while ((ch = std::cin.get()) != 'q');
}
