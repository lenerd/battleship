#ifndef TEST_DECIDER_HPP
#define TEST_DECIDER_HPP

#include <bitset>
#include <functional>
#include <future>
#include "circuits/aby_party_connection.hpp"
#include "misc/options.hpp"

template <size_t input_size>
using decision_fun_c = std::function<bool(std::bitset<input_size>)>;

using decision_fun_s = std::function<bool()>;

template <typename D>
decision_fun_s mk_fun_s()
{
    return [] 
    {
        auto aby{std::make_shared<ABYPartyConnection>(Role::server, "127.0.0.1", 6677)};
        D decider_s{Role::server, aby};
        return decider_s.decide();
    };

}

template <typename D, size_t input_size>
decision_fun_c<input_size> mk_fun_c()
{
    return [] (std::bitset<input_size> input)
    {
        auto aby{std::make_shared<ABYPartyConnection>(Role::client, "127.0.0.1", 6677)};
        D decider_c{Role::client, aby};
        return decider_c.decide(input);
    };

}

template <typename D>
std::future<bool> decide_async_s()
{
    return std::async(std::launch::async, mk_fun_s<D>());
}

template <typename D, size_t input_size>
std::future<bool> decide_async_c(std::bitset<input_size> input)
{
    return std::async(std::launch::async, mk_fun_c<D, input.size()>(), input);
}



#endif // TEST_DECIDER_HPP
