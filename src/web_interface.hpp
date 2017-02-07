#ifndef WEB_INTERFACE_HPP
#define WEB_INTERFACE_HPP

#include <stdexcept>
#include <string>
#include <memory>
#include <unistd.h>
#include "user_interface.hpp"

class Board;

class WebInterface : public UserInterface
{
public:
    WebInterface(Board &board_local, Board &board_remote,
            uint16_t ws_port = 1338);
    ~WebInterface();
    void show();
    void place_ships();
    coords_t select_position();
    void post_message(std::string message);
    void wait_for_quit();

    void start_webserver();
    void kill_webserver();
    void start_webbrowser();
    static const uint16_t http_port = 1339;
private:
    struct WebInterfaceImpl;
    std::unique_ptr<WebInterfaceImpl> pImpl;

    void init_websocket();
    pid_t webserver_pid;
};

#endif // WEB_INTERFACE_HPP
