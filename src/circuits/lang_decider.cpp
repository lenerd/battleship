#include "aby_party_connection.hpp"
#include "lang_decider.hpp"

LangDecider::LangDecider(Role role, ABYPartyConnection &aby)
    : role_(role), aby_(aby)
{
}

