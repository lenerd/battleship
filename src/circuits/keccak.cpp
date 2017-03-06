#include "keccak.hpp"
#include "circuit_wrapper.hpp"
#include "crypto/keccak.hpp"

StateShare::StateShare(std::vector<share_p> input)
{
    std::copy(input.cbegin(), input.cend(), data.begin());
}

StateShare::StateShare(const StateShare &other)
    : data(other.data)
{
}

StateShare& StateShare::operator=(const StateShare &other)
{
    data = other.data;
    return *this;
}

share_p& StateShare::operator()(size_t x, size_t y)
{
    assert(x < 5 && y < 5);
    return data[x + y*5];
}

const share_p& StateShare::operator()(size_t x, size_t y) const
{
    assert(x < 5 && y < 5);
    return data[x + y*5];
}


void build_theta(CircuitW_p circ, StateShare &state, size_t)
{
    std::array<share_p, 5> C;
    share_p D;

    for (size_t x{0}; x < 5; ++x)
    {
        C[x] = circ->PutXORGate(state(x, 0), state(x, 1));
        for (size_t y{2}; y < 5; ++y)
        {
            C[x] = circ->PutXORGate(C[x], state(x, y));
        }
    }
    for (size_t x{0}; x < 5; ++x)
    {
        D = circ->PutXORGate(C[(x+5-1) % 5], circ->PutROTLGate(C[(x+1) % 5], 1));
        for (size_t y{0}; y < 5; ++y)
        {
            state(x, y) = circ->PutXORGate(state(x, y), D);
        }
    }
}

void build_rho_pi(CircuitW_p circ, StateShare &state, size_t)
{
    size_t x = 1;
    size_t y = 0;
    share_p current{state(x, y)};
    for (size_t t{0}; t < 24; ++t)
    {
        {
            auto new_y{(2*x + 3*y) % 5};
            x = y;
            y = new_y;
        }
        {
            auto new_lane{circ->PutROTLGate(current, (t+1) * (t+2) / 2)};
            current = state(x, y);
            state(x, y) = new_lane;
        }
    }
}

void build_chi(CircuitW_p circ, StateShare &state, size_t)
{
    StateShare tmp{state};
    for (size_t x{0}; x < 5; ++x)
    {
        for (size_t y{0}; y < 5; ++y)
        {
            auto t1{circ->PutINVGate(tmp((x+1) % 5, y))};
            auto t2{circ->PutANDGate(t1, tmp((x+2) % 5, y))};
            state(x, y) = circ->PutXORGate(tmp(x, y), t2);
        }
    }
}

void build_iota(CircuitW_p circ, StateShare &state, size_t round)
{
    auto cons{circ->PutCONSGate(Keccak::round_constants[round], 64)};
    state(0, 0) = circ->PutXORGate(state(0, 0), cons);
}

void build_round(CircuitW_p circ, StateShare &state, size_t round)
{
    build_theta(circ, state, round);
    build_rho_pi(circ, state, round);
    build_chi(circ, state, round);
    build_iota(circ, state, round);
}


void build_permutation(CircuitW_p circ, StateShare &state, size_t)
{
    for (size_t i{0}; i < 24; ++i)
    {
        build_round(circ, state, i);
    }
}
