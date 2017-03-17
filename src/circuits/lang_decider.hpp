#ifndef LANG_DECIDER_HPP
#define LANG_DECIDER_HPP

#include <bitset>
#include <cstddef>
#include <abycore/aby/abyparty.h>
#include "aby_party_connection.hpp"
#include "misc/options.hpp"

class share;


template <size_t word_len>
class LangDecider
{
public:
    LangDecider(Role role, APC_p &aby)
        : role_(role), aby_(aby)
    {
    }
    virtual void build() = 0;
    virtual bool execute(std::bitset<word_len>) = 0;
protected:
    Role role_;
    APC_p &aby_;
    std::bitset<word_len> input_;
    share *input_share = nullptr;
};


#endif // LANG_DECIDER_HPP
