#include <iostream>
#include <sstream>
#include <utility>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "proof.hpp"
#include "misc/options.hpp"
#include "network/tcp_connection.hpp"


namespace po = boost::program_options;

struct TestOptions : public Options
{
    bool inv_aby_role;
    std::string inv_aby_address;
};


TestOptions parse_arguments(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("role,r", po::value<Role>()->required(), "role")
        ("address", po::value<std::string>()->default_value("127.0.0.1"), "address")
        ("game-port", po::value<uint16_t>()->default_value(6666), "game port")
        ("aby-port", po::value<uint16_t>()->default_value(6677), "aby port")
        ("inv-aby-role", po::value<bool>()->default_value(false), "invert aby role")
        ("inv-aby-address", po::value<std::string>()->default_value("127.0.0.1"), "invert aby address")
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
    TestOptions options;
    options.role = vm["role"].as<Role>();
    options.address = vm["address"].as<std::string>();
    options.game_port = vm["game-port"].as<uint16_t>();
    options.aby_port = vm["aby-port"].as<uint16_t>();
    options.inv_aby_role = vm["inv-aby-role"].as<bool>();
    options.inv_aby_address = vm["inv-aby-address"].as<std::string>();
    return options;
}

int main(int argc, char* argv[])
{
    auto options{parse_arguments(argc, argv)};

    boost::asio::io_service io_service;
    auto conn{TCPConnection::from_role(options.role, io_service, options.address, options.game_port)};
    SHA3_256_HashCommitter committer{conn};
    std::vector<Comm_p> commitments(100);

    bool res;
    if (options.role == Role::server)
    {
        std::cout << "Prover with PID " << ::getpid() << " (waiting for input)" << std::endl;
        getchar();
        auto board{string_to_bits<100>(
                    "1010101000"
                    "1010101000"
                    "0000000000"
                    "1110101000"
                    "0000101000"
                    "1110101000"
                    "0000101000"
                    "1110000000"
                    "0000000000"
                    "1111100000"
                )};
        for (size_t i{0}; i < 100; ++i)
        {
            commitments[i] = committer.send_commitment(board[i]);
        }
        // auto hash_comm{std::dynamic_pointer_cast<HashCommitment>(commitments[42])};
        // hash_comm->padding[4] ^= 0x42;
        res = proof_prover(commitments, options, options.inv_aby_role, options.inv_aby_address);
    }
    else
    {
        std::cout << "Verifier with PID " << ::getpid() << " (waiting for input)" << std::endl;
        getchar();

        for (size_t i{0}; i < 100; ++i)
        {
            commitments[i] = committer.recv_commitment();
        }
        res = proof_verifier(commitments, options, options.inv_aby_role, options.inv_aby_address);
    }
    std::cout << "Result: " << res << std::endl;

    return 0;
}
