#include <iostream>
#include <sstream>
#include <string>
#include "circuits/aby_party_connection.hpp"
#include "circuits/trivial_decider.hpp"
#include "misc/options.hpp"

int main(int argc, char* argv[])
{
    Role role;
    if (argc < 2)
    {
        std::cerr << "usage: ./test_ed.exe <role>\n";
        return 1;
    }
    std::stringstream ss(argv[1]);
    ss >> role;
    std::cout << "Role: " << role << std::endl;


    auto aby{std::make_shared<ABYPartyConnection>(role, "127.0.0.1", 6677)};
    TrivialDecider decider{role, aby};

    bool res;
    if (role == Role::server)
    {
        res = decider.decide();
    }
    else
    {
        res = decider.decide(std::bitset<25>());
    }
    std::cout << "Result: " << res << std::endl;

    return 0;
}
