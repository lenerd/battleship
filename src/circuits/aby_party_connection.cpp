#include <future>
#include <abycore/aby/abyparty.h>
#include "aby_party_connection.hpp"
#include "circuit_wrapper.hpp"
#include "misc/options.hpp"

struct ABYPartyConnection::APC_Impl
{
    std::unique_ptr<ABYParty> party = nullptr;
    std::string address;
    BooleanCircuit* circuit = nullptr;
};


ABYPartyConnection::ABYPartyConnection(Role role, std::string address, uint16_t port)
    : pImpl(std::make_unique<APC_Impl>())
{
    pImpl->address = address;
    pImpl->party = std::make_unique<ABYParty>(
            (role == Role::server) ? SERVER : CLIENT,
            &*(pImpl->address.begin()),
            port,
            get_sec_lvl(128), // seclvl
            32, // bitlength
            1, // threads
            MT_OT, // multiplication tripe generator
            // 4000000 // maxgates
            40000000 // maxgates
    );

    auto sharings{pImpl->party->GetSharings()};
    pImpl->circuit = static_cast<BooleanCircuit*>(sharings[S_YAO]->GetCircuitBuildRoutine());
}

ABYPartyConnection::~ABYPartyConnection() = default;

std::pair<APC_p, APC_p> ABYPartyConnection::make_pair()
{
    auto fut_s{std::async(std::launch::async, []
        { return std::make_shared<ABYPartyConnection>(Role::server, "127.0.0.1", 6677); })};
    auto fut_c{std::async(std::launch::async, []
        { return std::make_shared<ABYPartyConnection>(Role::client, "127.0.0.1", 6677); })};
    auto aby_s{fut_s.get()};
    auto aby_c{fut_c.get()};
    return {aby_s, aby_c};
}

std::shared_ptr<CircuitWrapper> ABYPartyConnection::get_circuit()
{
    return std::make_shared<CircuitWrapper>(pImpl->circuit);
}

void ABYPartyConnection::execute()
{
    pImpl->party->ExecCircuit();
}

void ABYPartyConnection::reset()
{
    pImpl->party->Reset();
}
