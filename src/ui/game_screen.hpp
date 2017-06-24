#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include <memory>
#include <string>
#include "game/board.hpp"

class BoardWindow;
class MessageWindow;

/**
 * Screen of the ncurses interface during the game
 */
class GameScreen
{
public:
    GameScreen();
    GameScreen(Board_p board_local, Board_p board_remote);
    ~GameScreen();
    void handle();
    void post_message(std::string message);
    void place_ships();
    coords_t select_position();
private:
    Board_p board_local_;
    Board_p board_remote_;
    std::unique_ptr<BoardWindow> bw_local_;
    std::unique_ptr<BoardWindow> bw_remote_;
    std::unique_ptr<MessageWindow> mw_;
};

#endif // GAME_SCREEN_HPP
