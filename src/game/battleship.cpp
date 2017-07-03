#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "network/tcp_connection.hpp"
#include "game/game.hpp"
#include "ui/user_interface.hpp"
#include "misc/options.hpp"


namespace po = boost::program_options;


Options parse_arguments(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("role,r", po::value<Role>()->required(), "role")
        ("ui,u", po::value<UIType>()->default_value(UIType::ncurses), "interface type")
        ("address", po::value<std::string>()->default_value("127.0.0.1"), "address")
        ("game-port", po::value<uint16_t>()->default_value(1337), "game port")
        ("aby-port", po::value<uint16_t>()->default_value(1338), "aby port")
        ("http-port", po::value<uint16_t>()->default_value(1339), "http port")
        ("ws-port", po::value<uint16_t>()->default_value(1340), "ws port")
    ;
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (po::error &e)
    {
        std::cerr << "Error parsing arguments: " << e.what() << "\n";
        std::cerr << "\n" << desc << "\n";
        exit(EXIT_FAILURE);
    }

    if (vm.count("help"))
    {
        std::cerr << desc << "\n";
        exit(EXIT_FAILURE);
    }
    Options options;
    options.role = vm["role"].as<Role>();
    options.ui_type = vm["ui"].as<UIType>();
    options.address = vm["address"].as<std::string>();
    options.game_port = vm["game-port"].as<uint16_t>();
    options.aby_port = vm["aby-port"].as<uint16_t>();
    options.http_port = vm["http-port"].as<uint16_t>();
    options.ws_port = vm["ws-port"].as<uint16_t>();
    return options;
}


int main(int argc, char* argv[])
{
    auto options{parse_arguments(argc, argv)};

    boost::asio::io_service io_service;
    try
    {
        auto connection(TCPConnection::from_role(options.role,
                                                 io_service,
                                                 options.address,
                                                 options.game_port));
        UIFactory ui_factory{options};
        Game game(options,
                  ui_factory,
                  options.ui_type,
                  connection);

        game.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Caught exception: " << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
