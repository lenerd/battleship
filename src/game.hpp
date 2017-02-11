#ifndef GAME_H
#define GAME_H

#include <memory>
#include "board.hpp"
#include "options.hpp"
#include "game_communicator.hpp"
#include "user_interface.hpp"

enum class Role;

class Game
{
public:
    enum class State
    {
        initial,
        ships_placed,
        ships_committed,
        commitments_sent,
        commitments_received,
        ready,
        turn,
        query_phase,
        answer_phase,
        cheating_detected,
        end_of_game,
        abort,
        end,
    };
    Game(Role role, const UIFactory &ui_factory, UIType ui_type, Conn_p conn);
    ~Game();
    void run();

    // game preparation
    void handle_initial();
    void handle_ships_placed();
    void handle_ships_committed();
    void handle_commitments_sent();
    void handle_commitments_received();
    void handle_ready();

    // game
    void handle_query_phase();
    void handle_answer_phase();

    // end of game
    void handle_end();
private:
    Role role_;
    State state_;
    Board_p board_local_;
    Board_p board_remote_;
    UI_p ui_;
    GameCommunicator game_comm_;
    bool won_;
};

#endif // GAME_H
