#include <iostream>
#include <memory>
#include <ncurses.h>
#include "game_screen.hpp"
#include "board_window.hpp"
#include "message_window.hpp"


GameScreen::GameScreen(Board_p board_local, Board_p board_remote)
    : board_local_(board_local), board_remote_(board_remote)
{
    bw_local_ = std::make_unique<BoardWindow>(1, 2, board_local);
    bw_remote_ = std::make_unique<BoardWindow>(1, 6 + bw_local_->get_width(), board_remote);
    bw_local_->draw_grid();
    bw_remote_->draw_grid();
    mw_ = std::make_unique<MessageWindow>(1 + bw_local_->get_height() + 2, 2, 8, 64);
}

GameScreen::~GameScreen()
{
}

void GameScreen::handle()
{
    bw_local_->place_ships();
    wgetch(bw_local_->get_window());
}

void GameScreen::post_message(std::string message)
{
    mw_->post_message(message);
}

void GameScreen::place_ships()
{
    bw_local_->place_ships();
}

coords_t GameScreen::select_position()
{
    return bw_remote_->select_position();
}
