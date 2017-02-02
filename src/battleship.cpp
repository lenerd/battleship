#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "tcp_connection.hpp"
#include "game.hpp"
#include "user_interface.hpp"
#include "util.hpp"


using boost::asio::ip::tcp;

void usage()
{
    std::cout << "usage: ./battleships server|client address port" << std::endl;
}


tcp::socket open_socket(Role role, boost::asio::io_service &io_service,
                        std::string host, std::string port)
{
    tcp::socket socket(io_service);

    switch (role)
    {
        case Role::server:
            {
                tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), std::stoi(port));
                tcp::acceptor acceptor(socket.get_io_service(), endpoint);
                acceptor.accept(socket);
            }
            break;

        case Role::client:
            {
                tcp::resolver resolver(socket.get_io_service());
                boost::asio::connect(socket, resolver.resolve({host, port}));
            }
            break;
    }

    return socket;
}



int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        usage();
        return EXIT_FAILURE;
    }
    Role role;
    std::string role_s(argv[1]);
    std::string address(argv[2]);
    std::string port(argv[3]);
    boost::asio::io_service io_service;
    if (role_s == "server")
    {
        role = Role::server;
    }
    else if (role_s == "client")
    {
        role = Role::client;
    }
    else
    {
        usage();
        return EXIT_FAILURE;
    }

    try
    {
        tcp::socket socket(open_socket(role, io_service, address, port));
        TCPConnection connection(std::move(socket));
        Game game(role, UserInterface::Type::ncurses, connection);

        game.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Caught exception: " << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
