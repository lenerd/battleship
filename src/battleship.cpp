#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "tcp_connection.hpp"
#include "game.hpp"
#include "user_interface.hpp"
#include "options.hpp"


namespace po = boost::program_options;


int main(int argc, char* argv[])
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
        return EXIT_FAILURE;
    }

    if (vm.count("help"))
    {
        std::cerr << desc << "\n";
        return EXIT_FAILURE;
    }

    boost::asio::io_service io_service;
    try
    {
        auto connection(TCPConnection::from_role(vm["role"].as<Role>(),
                                                 io_service,
                                                 vm["address"].as<std::string>(),
                                                 vm["game-port"].as<uint16_t>()));
        UIFactory ui_factory{vm["http-port"].as<uint16_t>(),
                             vm["ws-port"].as<uint16_t>()};
        Game game(vm["role"].as<Role>(),
                  ui_factory,
                  vm["ui"].as<UIType>(),
                  connection);

        game.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Caught exception: " << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
