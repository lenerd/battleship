#include <cassert>
#include <iostream>
#include <stdexcept>
#include <boost/asio/ip/tcp.hpp>
#include "game.hpp"
#include "user_interface.hpp"
#include "util.hpp"

using boost::asio::ip::tcp;

Game::Game(Role role, UserInterface::Type ui_type, tcp::socket &socket)
    : role_(role), state_(State::initial),
      board_local_(std::make_unique<Board>()),
      board_remote_(std::make_unique<Board>()),
      ui_(make_userinterface(ui_type, *board_local_, *board_remote_)),
      socket_(socket)
{
    ui_->show();
}

Game::~Game()
{
}

void Game::run()
{
    // return;
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
            case State::commitments_sent:
                handle_commitments_sent();
                break;
            case State::commitments_received:
                handle_commitments_received();
                break;
            case State::ready:
                handle_ready();
                break;
            case State::turn:
                handle_turn();
                break;
            case State::wait_for_query:
                handle_wait_for_query();
                break;
            case State::wait_for_answer:
                handle_wait_for_answer();
                break;
            default:
                throw std::logic_error("unknown state");
        }
    }
}

void Game::handle_initial()
{
    assert(state_ == State::initial);
    std::cerr << "state: initial\n";
    // place ships
    board_local_->debug();
    ui_->place_ships();
    board_local_->debug();
    state_ = State::ships_placed;
}

void Game::handle_ships_placed()
{
    assert(state_ == State::ships_placed);
    std::cerr << "state: ships_placed\n";
    // TODO: make and exchange commitments
    // state_ = State::ships_committed;
    state_ = State::ready;
}

void Game::handle_ships_committed()
{
    assert(state_ == State::ships_committed);
    std::cerr << "state: ships_committed\n";
    if (role_ == Role::server)
    {
        // send commitments
        state_ = State::commitments_sent;
    }
    else
    {
        // recv commitments
        state_ = State::commitments_received;
    }
}

void Game::handle_commitments_sent()
{
    assert(state_ == State::commitments_sent);
    assert(role_ == Role::server);
    // recv commitments
    state_ = State::ready;
}


void Game::handle_commitments_received()
{
    assert(state_ == State::commitments_received);
    assert(role_ == Role::client);
    // send commitments
    state_ = State::ready;
}

void Game::handle_ready()
{
    assert(state_ == State::ready);
    std::cerr << "state: ready\n";
    state_ = State::end;
}
void Game::handle_turn()
{
    // ask ui for query
    // send query
    state_ = State::wait_for_answer;
}

void Game::handle_wait_for_query()
{
    assert(state_ == State::wait_for_query);
    // recv query
    // send answer
    // check for end of game
    state_ = State::turn;
}

void Game::handle_wait_for_answer()
{
    assert(state_ == State::wait_for_answer);
    // recv answer
    // display answer
    // check for end of game
    state_ = State::wait_for_query;
}
