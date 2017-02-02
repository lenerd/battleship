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
        TCPConnection connection(TCPConnection::from_role(role, io_service, address, port));
        Game game(role, UserInterface::Type::ncurses, connection);

        game.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Caught exception: " << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
