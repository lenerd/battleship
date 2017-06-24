#ifndef WEBSOCKET_CONNECTION_HPP
#define WEBSOCKET_CONNECTION_HPP

#include <memory>
#include <string>

/**
 * Wrapper around a web socket
 */
class WebSocketConnection
{
public:
    /**
     * Constructor: Listen on a Port
     */
    WebSocketConnection(uint16_t port);
    /**
     * Destructor
     */
    ~WebSocketConnection();

    /**
     * Receive a message
     */
    std::string recv_message();
    /**
     * Send a message
     */
    void send_message(std::string data);
    /**
     * Accept an incoming connection
     */
    void accept();
private:
    uint16_t port_;
    struct WebSocketConnectionImpl;
    std::unique_ptr<WebSocketConnectionImpl> pImpl;
};

#endif // WEBSOCKET_CONNECTION_HPP
