#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "web_interface.hpp"
#include "websocket_connection.hpp"


struct WebInterface::WebInterfaceImpl
{
    std::unique_ptr<WebSocketConnection> ws_conn;
    WebInterfaceImpl() {}
    ~WebInterfaceImpl() {}
};

WebInterface::WebInterface(Board &board_local, Board &board_remote,
        uint16_t ws_port)
        : UserInterface(board_local, board_remote),
        pImpl(std::make_unique<WebInterfaceImpl>())
{
    // start webserver
    start_webserver();
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
    throw std::logic_error("WebInterface::place_ships not implemented");
}

coords_t WebInterface::select_position()
{
    throw std::logic_error("WebInterface::select_position not implemented");
}

void WebInterface::post_message(std::string)
{
    throw std::logic_error("WebInterface::post_message not implemented");
}

void WebInterface::wait_for_quit()
{
    throw std::logic_error("WebInterface::wait_for_quit not implemented");
}

void WebInterface::init_websocket()
{
}

void WebInterface::start_webserver()
{
    kill_webserver();
    auto child_pid = fork();
    if (child_pid == 0) // child
    {
        chdir("./web");
        const char* const args[] = {"/usr/bin/python", "-m", "http.server", "-b", "127.0.0.1", "1339", nullptr};
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
    system("python -m webbrowser -t \"http://localhost:1339\"");
}
