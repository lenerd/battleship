#ifndef WEBSOCKET_CONNECTION_HPP
#define WEBSOCKET_CONNECTION_HPP

#include <memory>
#include <string>

class WebSocketConnection
{
public:
    WebSocketConnection(uint16_t port);
    ~WebSocketConnection();

    std::string recv_message();
    void send_message(std::string data);
    void accept();
private:
    uint16_t port_;
    struct WebSocketConnectionImpl;
    std::unique_ptr<WebSocketConnectionImpl> pImpl;
};

#endif // WEBSOCKET_CONNECTION_HPP
