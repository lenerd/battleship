#ifndef ABY_PARTY_CONNECTION_HPP
#define ABY_PARTY_CONNECTION_HPP

#include <memory>
#include <string>

enum class Role;
class BooleanCircuit;
class share;
class ABYPartyConnection;
class CircuitWrapper;

using APC_p = std::shared_ptr<ABYPartyConnection>;

class ABYPartyConnection
{
public:
    ABYPartyConnection(Role role, std::string address, uint16_t port);
    ~ABYPartyConnection();
    static std::pair<APC_p, APC_p> make_pair();

    std::shared_ptr<CircuitWrapper> get_circuit();
    void execute();
    void reset();

private:
    struct APC_Impl;
    std::unique_ptr<APC_Impl> pImpl;
};


#endif // ABY_PARTY_CONNECTION_HPP
