#ifndef GAME_H
#define GAME_H

#include <memory>
#include "board.hpp"
#include "game_communicator.hpp"
#include "crypto/committer.hpp"
#include "misc/options.hpp"
#include "ui/user_interface.hpp"

enum class Role;

/**
 * Implements stateemachine of the game
 */
class Game
{
public:
    /**
     * Possible states
     */
    enum class State
    {
        initial,
        ships_placed,
        ships_committed,
        ready,
        turn,
        query_phase,
        answer_phase,
        cheating_detected,
        end_of_game,
        abort,
        end,
    };
    /**
     * Constructor
     */
    Game(Role role, const UIFactory &ui_factory, UIType ui_type, Conn_p conn, bool prove_board=true);
    /**
     * Destructor
     */
    ~Game();

    /**
     * Run the game
     */
    void run();

    // game preparation
    /**
     * Handle phase: initial
     */
    void handle_initial();
    /**
     * Handle phase: ships are placed
     */
    void handle_ships_placed();
    /**
     * Handle phase: ships are committed
     */
    void handle_ships_committed();
    /**
     * Handle phase: commitments are sent
     */
    void handle_commitments_sent();
    /**
     * Handle phase: ready to play
     */
    void handle_commitments_received();
    /**
     * Handle phase: ready to play
     */
    void handle_ready();

    // game
    /**
     * Handle query phase
     */
    void handle_query_phase();
    /**
     * Handle answer phase
     */
    void handle_answer_phase();

    /**
     * end of game
     */
    void handle_end();
private:
    Role role_;
    State state_;
    Committer_p committer_;
    Board_p board_local_;
    Board_p board_remote_;
    UI_p ui_;
    GameCommunicator game_comm_;
    bool won_;
    bool prove_board_;
};

#endif // GAME_H
