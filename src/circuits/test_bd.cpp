#include <iostream>
#include <sstream>
#include <string>
#include "circuits/aby_party_connection.hpp"
#include "circuits/battleship_decider.hpp"
#include "misc/options.hpp"
#include "misc/util.hpp"

int main(int argc, char* argv[])
{
    Role role;
    if (argc < 2)
    {
        std::cerr << "usage: ./test_bd.exe <role>\n";
        return 1;
    }
    std::stringstream ss(argv[1]);
    ss >> role;
    std::cout << "Role: " << role << std::endl;


    auto aby{std::make_shared<ABYPartyConnection>(role, "127.0.0.1", 6677)};
    BattleshipDecider decider{role, aby};

    bool res;
    if (role == Role::server)
    {
        res = decider.decide();
    }
    else
    {
        res = decider.decide(string_to_bits<100>(
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
                ));
    }
    std::cout << "Result: " << res << std::endl;

    return 0;
}
