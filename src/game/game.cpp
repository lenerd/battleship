#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "game.hpp"
#include "except.hpp"
#include "crypto/hash_committer.hpp"
#include "circuits/proof.hpp"
#include "ui/user_interface.hpp"
#include "misc/util.hpp"


Game::Game(const Options& options, const UIFactory &ui_factory, UIType ui_type, Conn_p conn, bool prove_board)
    : options_(options), role_(options.role), state_(State::initial),
      committer_(std::make_shared<SHA3_256_HashCommitter>(conn)),
      board_local_(std::make_shared<Board>(committer_)),
      board_remote_(std::make_shared<Board>(committer_)),
      ui_(ui_factory.make(ui_type, board_local_, board_remote_)),
      game_comm_(conn),
      prove_board_(prove_board)
{
    ui_->show();
}

Game::~Game()
{
}

void Game::run()
{
    while (state_ != State::end)
    {
        switch (state_)
        {
            case State::initial:
                handle_initial();
                break;
            case State::ships_placed:
                handle_ships_placed();
                break;
            case State::ships_committed:
                handle_ships_committed();
                break;
            case State::ready:
                handle_ready();
                break;
            case State::query_phase:
                handle_query_phase();
                break;
            case State::answer_phase:
                handle_answer_phase();
                break;
            default:
                throw std::logic_error("unknown state");
        }
    }
    handle_end();
}

void Game::handle_initial()
{
    assert(state_ == State::initial);
    std::cerr << "state: initial\n";
    // place ships
    board_local_->debug();
    ui_->post_message("Place your ships!");
    ui_->place_ships();
    board_local_->debug();
    state_ = State::ships_placed;
}

void Game::handle_ships_placed()
{
    assert(state_ == State::ships_placed);
    std::cerr << "state: ships_placed\n";

    if (role_ == Role::server)
    {
        ui_->post_message("Committing board");
        board_local_->send_commitments();
        std::cerr << "commitments sent\n";
        ui_->post_message("Receiving commitments");
        board_remote_->recv_commitments();
        std::cerr << "commitments received\n";
    }
    else
    {
        ui_->post_message("Receiving commitments");
        board_remote_->recv_commitments();
        std::cerr << "commitments received\n";
        ui_->post_message("Committing board");
        board_local_->send_commitments();
        std::cerr << "commitments sent\n";
    }
    state_ = State::ships_committed;
}

void Game::handle_ships_committed()
{
    assert(state_ == State::ships_committed);
    std::cerr << "state: ships_committed\n";

    if (prove_board_)
    {
        bool result;
        if (role_ == Role::server)
        {
            // prove
            ui_->post_message("Proving validity of the board ...");
            std::cerr << "Proving validity of the board ...\n";
            result = proof_sender(board_local_->get_commitments(), options_);
            if (result)
            {
                ui_->post_message("... Success");
                std::cerr << "Success\n";
            }
            else
            {
                ui_->post_message("... Failure, this looks like cheating");
                std::cerr << "Failure\n";
            }

            // verify
            ui_->post_message("Verifying validity of the opponent's board");
            std::cerr << "Verifying validity of the opponent's board ...\n";
            result = proof_receiver(board_remote_->get_commitments(), options_);
            if (result)
            {
                ui_->post_message("... Success");
                std::cerr << "Success\n";
            }
            else
            {
                ui_->post_message("... Failure, Cheating Detected!");
                std::cerr << "Failure\n";
            }
        }
        else
        {
            // verify
            ui_->post_message("Verifying validity of the opponent's board");
            std::cerr << "Verifying validity of the opponent's board ...\n";
            result = proof_receiver(board_remote_->get_commitments(), options_);
            if (result)
            {
                ui_->post_message("... Success");
                std::cerr << "Success\n";
            }
            else
            {
                ui_->post_message("... Failure, Cheating Detected!");
                std::cerr << "Failure\n";
            }

            // prove
            ui_->post_message("Proving validity of the board ...");
            std::cerr << "Proving validity of the board ...\n";
            result = proof_sender(board_local_->get_commitments(), options_);
            if (result)
            {
                ui_->post_message("... Success");
                std::cerr << "Success\n";
            }
            else
            {
                ui_->post_message("... Failure, this looks like cheating");
                std::cerr << "Failure\n";
            }
        }
    }

    state_ = State::ready;
}

void Game::handle_ready()
{
    assert(state_ == State::ready);
    std::cerr << "state: ready\n";
    ui_->post_message("Game starts");
    if (role_ == Role::server)
        state_ = State::query_phase;
    else
        state_ = State::answer_phase;
}

void Game::handle_query_phase()
{
    assert(state_ == State::query_phase);
    std::cerr << "state: query_phase\n";

    ui_->post_message("select position");
    auto coords{ui_->select_position()};
    auto answer{game_comm_.query_position(coords)};
    board_remote_->set_queried(coords, answer);
    // verify commitment
    try
    {
        board_remote_->verify_answer(coords, answer);
        ui_->post_message("Answer verified with commitment");
    }
    catch (CheatingException &e)
    {
        ui_->post_message("Opponent was caught cheating:");
        ui_->post_message(e.what());
        std::cerr << "Opponent was caught cheating: " << e.what() << "\n";
    }

    if (board_remote_->num_ships_hit() < 3)
    {
        state_ = State::answer_phase;
    }
    else
    {
        won_ = true;
        state_ = State::end;
    }
}

void Game::handle_answer_phase()
{
    assert(state_ == State::answer_phase);
    std::cerr << "state: answer_phase\n";

    ui_->post_message("answering position");
    auto coords{game_comm_.answer_query([this] (coords_t coords)
        {
            return this->board_local_->query(coords);
        })};
    // decommit
    board_local_->prove_answer(coords);
    ui_->post_message("Answer proved with commitment");
    if (board_local_->ships_alive())
    {
        state_ = State::query_phase;
    }
    else
    {
        won_ = false;
        state_ = State::end;
    }
}

void Game::handle_end()
{
    assert(state_ == State::end);
    std::cerr << "state: end\n";

    if (won_)
    {
        ui_->post_message("You won: you destroyed all enemy ships");
    }
    else
    {
        ui_->post_message("You lost: all your ships are destroyed");
    }
    ui_->wait_for_quit();
}
