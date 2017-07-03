#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include "proof.hpp"
#include "misc/options.hpp"
#include "network/tcp_connection.hpp"


int main(int argc, char* argv[])
{
    Options options;
    if (argc < 2)
    {
        std::cerr << "usage: ./test_bd.exe <role>\n";
        return 1;
    }
    std::stringstream ss(argv[1]);
    ss >> options.role;
    std::cout << "Role: " << options.role << std::endl;

    options.address = "127.0.0.1";
    options.game_port = 6666;
    options.aby_port = 6677;

    boost::asio::io_service io_service;
    auto conn{TCPConnection::from_role(options.role, io_service, options.address, options.game_port)};
    SHA3_256_HashCommitter committer{conn};
    std::vector<Comm_p> commitments(100);

    bool res;
    if (options.role == Role::server)
    {
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
        res = proof_sender(commitments, options);
    }
    else
    {
        for (size_t i{0}; i < 100; ++i)
        {
            commitments[i] = committer.recv_commitment();
        }
        res = proof_receiver(commitments, options);
    }
    std::cout << "Result: " << res << std::endl;

    return 0;
}
