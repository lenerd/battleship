#ifndef WEB_INTERFACE_HPP
#define WEB_INTERFACE_HPP

#include <stdexcept>
#include <string>
#include <memory>
#include <unistd.h>
#include "user_interface.hpp"

class Board;

/**
 * User interface implementation via web browser
 */
class WebInterface : public UserInterface
{
public:
    WebInterface(Board_p board_local, Board_p board_remote,
            const Options &options);
    ~WebInterface();
    void show();
    void place_ships();
    coords_t select_position();
    void post_message(std::string message);
    void wait_for_quit();

    /**
     * Send message to update a boards representation
     */
    void update_position(std::string board, size_t row, size_t col);
    /**
     * Send message to switch states in the client state machine
     */
    void switch_state(std::string state);

    /**
     * Start/stop the webserver
     */
    void start_webserver();
    void kill_webserver();
    /**
     * Start a webbrowser
     */
    void start_webbrowser();

    /**
     * Used ports
     */
    uint16_t http_port;
    uint16_t ws_port;
private:
    struct WebInterfaceImpl;
    std::unique_ptr<WebInterfaceImpl> pImpl;

    pid_t webserver_pid;
};

#endif // WEB_INTERFACE_HPP
