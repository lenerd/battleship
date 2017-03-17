#ifndef TEST_CIRCUITS_HPP
#define TEST_CIRCUITS_HPP

#include <cstdint>
#include <future>
#include <vector>
#include "circuits/aby_party_connection.hpp"
#include "circuits/circuit_wrapper.hpp"
#include "circuits/keccak.hpp"
#include "misc/options.hpp"

using perm_fun = std::function<std::array<uint64_t, 25>(std::array<uint64_t, 25>, size_t)>;
using build_perm_fun = std::function<void(CircuitW_p, StateShare&, size_t)>;

std::vector<share_p> build_circuit(CircuitW_p circ, build_perm_fun fun, std::array<uint64_t, 25> input, size_t round)
{
    std::vector<share_p> input_shares;
    std::vector<share_p> output_shares;
    for (auto &in : input)
    {
        input_shares.push_back(circ->PutCONSGate(in, 64));
    }
    StateShare state{nullptr, [] (auto, auto, auto) {}, 136, input_shares};
    fun(circ, state, round);
    for (auto &share : state.data)
    {
        output_shares.push_back(circ->PutOUTGate(share, ALL));
    }
    return output_shares;
}


perm_fun mk_fun(Role role, build_perm_fun builder)
{
    return [role, builder] (std::array<uint64_t, 25> input, size_t round)
    {
        auto aby{std::make_shared<ABYPartyConnection>(role, "127.0.0.1", 6677)};
        auto output_shares{build_circuit(aby->get_circuit(), builder, input, round)};
        aby->execute();
        std::array<uint64_t, 25> output;
        for (size_t i{0}; i < 25; ++i)
        {
            output[i] = output_shares[i]->template get_clear_value<uint64_t>();
        }

        return output;
    };

}


std::array<uint64_t, 25> eval_permutation(build_perm_fun builder, std::array<uint64_t, 25> input, size_t round = 0)
{
    auto fut_s{std::async(std::launch::async, mk_fun(Role::server, builder), input, round)};
    auto fut_c{std::async(std::launch::async, mk_fun(Role::client, builder), input, round)};
    auto res_s{fut_s.get()};
    auto res_c{fut_c.get()};
    assert(res_s == res_c);
    return res_s;
}


using build_hash_fun = std::function<std::vector<share_p>(CircuitW_p, std::vector<share_p>)>;
using hash_fun = std::function<bytes_t(bytes_t)>;

hash_fun mk_hash_fun(Role role, build_hash_fun builder)
{
    return [role, builder] (bytes_t input) -> bytes_t
    {
        auto aby{std::make_shared<ABYPartyConnection>(role, "127.0.0.1", 6677)};
        auto circ{aby->get_circuit()};
        auto input_shares{circ->bytes_to_shares(input)};
        auto digest_shares{builder(circ, input_shares)};
        std::vector<share_p> output_shares(digest_shares.size());
        std::transform(digest_shares.cbegin(), digest_shares.cend(), output_shares.begin(),
                [circ] (share_p a) -> share_p { return circ->PutOUTGate(a, ALL); });
        aby->execute();
        return circ->shares_to_bytes(output_shares);
    };
}

bytes_t eval_hash(build_hash_fun builder, bytes_t input)
{
    auto fut_s{std::async(std::launch::async, mk_hash_fun(Role::server, builder), input)};
    auto fut_c{std::async(std::launch::async, mk_hash_fun(Role::client, builder), input)};
    auto res_s{fut_s.get()};
    auto res_c{fut_c.get()};
    assert(res_s == res_c);
    return res_s;
}


#endif // TEST_CIRCUITS_HPP
