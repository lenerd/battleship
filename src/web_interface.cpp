#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <signal.h>
#include <json.hpp>
#include "board.hpp"
#include "web_interface.hpp"
#include "websocket_connection.hpp"

using nlohmann::json;


struct WebInterface::WebInterfaceImpl
{
    std::unique_ptr<WebSocketConnection> ws_conn;
    WebInterfaceImpl() {}
    ~WebInterfaceImpl() {}
};

WebInterface::WebInterface(Board_p board_local, Board_p board_remote,
        const Options &options)
        : UserInterface(board_local, board_remote),
        http_port(options.http_port), ws_port(options.ws_port),
        pImpl(std::make_unique<WebInterfaceImpl>())
{
    // start webserver
    start_webserver();
    std::cout << "Please open your webbrowser on http://localhost:"
        << http_port << "/?" << ws_port << std::endl;
    try
    {
        // open websocket
        pImpl->ws_conn = std::make_unique<WebSocketConnection>(ws_port);
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        kill_webserver();
    }
    board_local_->signal_updated_position.connect([this] (coords_t coords)
        {
            this->update_position("local", coords.first, coords.second);
        });
    board_remote_->signal_updated_position.connect([this] (coords_t coords)
        {
            this->update_position("remote", coords.first, coords.second);
        });
}


WebInterface::~WebInterface()
{
    // shutdown webserver
    kill_webserver();
}


void WebInterface::show()
{
    // open browser
}

void WebInterface::place_ships()
{
    try
    {
        board_local_->debug();
        while (true)
        {
            auto data{pImpl->ws_conn->recv_message()};
            auto message{json::parse(data)};
            if (message["type"] == "toggle_position")
            {
                coords_t coords{message["row"], message["col"]};
                board_local_->flip(coords);
            }
            else if (message["type"] == "submit_board")
            {
                break;
            }
            else
            {
                std::cerr << "ignoring message: " << data << "\n";
            }
            board_local_->debug();
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "WebInterface::place_ships: " << e.what() << "\n";
    }
}

coords_t WebInterface::select_position()
{
    try
    {
        switch_state("query_phase");
        auto data{pImpl->ws_conn->recv_message()};
        auto message{json::parse(data)};
        if (message["type"] == "query")
        {
            return {message["row"], message["col"]};
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "WebInterface::place_ships: " << e.what() << "\n";
        throw "foobar";
    }
    throw std::logic_error("do something");
}

void WebInterface::switch_state(std::string state)
{
    json j;
    j["type"] = "switch_state";
    j["state"] = state;
    pImpl->ws_conn->send_message(j.dump());
}

void WebInterface::post_message(std::string message)
{
    json j;
    j["type"] = "message";
    j["message"] = message;
    pImpl->ws_conn->send_message(j.dump());
}

void WebInterface::wait_for_quit()
{
    return;
}

void WebInterface::update_position(std::string board, size_t row, size_t col)
{
    assert(board == "local" || board == "remote");
    json j;
    j["type"] = "update_position";
    j["board"] = board;
    j["row"] = row;
    j["col"] = col;
    if (board == "local")
    {
        j["ship"] = board_local_->get({row, col});
        j["queried"] = board_local_->is_queried({row, col});
    }
    else if (board == "remote")
    {
        j["ship"] = board_remote_->get({row, col});
        j["queried"] = board_remote_->is_queried({row, col});
    }
    pImpl->ws_conn->send_message(j.dump());
}


void WebInterface::start_webserver()
{
    kill_webserver();
    auto child_pid = fork();
    if (child_pid == 0) // child
    {
        auto port{std::to_string(http_port)};
        chdir("./web");
        const char* const args[] = {"/usr/bin/python", "-m", "http.server", "-b", "127.0.0.1", port.c_str(), nullptr};
        execv("/usr/bin/python", const_cast<char**const>(args));
        throw std::runtime_error("shouldn't happen");
    }
    else // parent
    {
        webserver_pid = child_pid;
    }
}

void WebInterface::kill_webserver()
{
    if (webserver_pid > 0)
    {
        kill(webserver_pid, SIGTERM);
        webserver_pid = 0;
    }
}

void WebInterface::start_webbrowser()
{
    throw std::logic_error("FIXME: start_webbrowser");
    std::stringstream ss;
    ss << "python -m webbrowser -t \"http://localhost:"
       << http_port
       << "?"
       << ws_port
       << "\"";
    auto cmd{ss.str()};
    std::cerr << "system(" << cmd << ");" << "\n";
    system(cmd.c_str());
}
