#include "keccak.hpp"
#include "circuit_wrapper.hpp"
#include "crypto/keccak.hpp"


StateShare::StateShare(CircuitW_p circ, permutation_builder permutation, size_t capacity_bytes)
    : circ(circ), permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(uint64_t));
    rate_bytes = data.size() * sizeof(uint64_t) - capacity_bytes;
    data.fill(circ->PutCONSGate(uint64_t{0}, 64));
}

StateShare::StateShare(CircuitW_p circ, permutation_builder permutation, size_t capacity_bytes, std::vector<share_p> initial)
    : circ(circ), permutation(permutation), capacity_bytes(capacity_bytes)
{
    assert(capacity_bytes < data.size() * sizeof(uint64_t));
    rate_bytes = data.size() * sizeof(uint64_t) - capacity_bytes;
    assert(initial.size() == data.size());
    std::copy(initial.cbegin(), initial.cend(), data.begin());
}

StateShare::StateShare(const StateShare &other)
    : circ(other.circ), permutation(other.permutation),
    capacity_bytes(other.capacity_bytes), rate_bytes(other.rate_bytes),
    data(other.data) {}

StateShare& StateShare::operator=(const StateShare &other)
{
    circ = other.circ;
    permutation = other.permutation;
    capacity_bytes = other.capacity_bytes;
    rate_bytes = other.rate_bytes;
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


void StateShare::absorb(const std::vector<share_p> &input)
{
    assert(input.size() == rate_bytes);
    absorb(input.cbegin());
}

void StateShare::absorb(std::vector<share_p>::const_iterator first)
{
    for (size_t i{0}; i < rate_bytes; ++i)
    {
        assert((*(first + static_cast<ptrdiff_t>(i)))->get_bitlength() == 8);
    }

    std::vector<share_p> byte_shares;
    for (auto &word_share : data)
    {
        auto tmp{circ->split_share(word_share, 8)};
        byte_shares.insert(byte_shares.cend(), tmp.cbegin(), tmp.cend());
    }

    std::transform(byte_shares.begin(),
                   byte_shares.begin() + static_cast<ptrdiff_t>(rate_bytes),
                   first, byte_shares.begin(),
                   [this] (auto a, auto b)
                   { return this->circ->PutXORGate(a, b); });


    for (size_t i{0}; i < 25; ++i)
    {
        std::vector<share_p> tmp(byte_shares.begin() + static_cast<ptrdiff_t>(i * 8),
                                 byte_shares.begin() + static_cast<ptrdiff_t>((i + 1) * 8));
        data[i] = circ->combine_shares(tmp);
    }

    permutation(circ, *this, 0);
}


std::vector<share_p> StateShare::squeeze()
{
    std::vector<share_p> output(rate_bytes);
    squeeze(output.begin());
    return output;
}

void StateShare::squeeze(std::vector<share_p>::iterator out_first)
{
    std::vector<share_p> byte_shares;
    byte_shares.reserve(rate_bytes);
    for (auto &word_share : data)
    {
        auto tmp{circ->split_share(word_share, 8)};
        byte_shares.insert(byte_shares.cend(), tmp.cbegin(), tmp.cend());
    }
    std::copy(byte_shares.begin(),
              byte_shares.begin() + static_cast<ptrdiff_t>(rate_bytes),
              out_first);
    permutation(circ, *this, 0);
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
    std::cerr << "build_permutation: " << circ->circ_->m_cCircuit->GetGateHead() << " gates\n";
}


std::vector<share_p> build_padding(CircuitW_p circ, std::vector<share_p> input_shares, size_t blocksize, uint8_t d)
{
    for (auto &s : input_shares)
        assert(s->get_bitlength() == 8);

    size_t message_size{input_shares.size()};
    size_t pad_size{blocksize - (message_size % blocksize)};
    input_shares.resize(message_size + pad_size);
    auto start_of_padding{std::next(input_shares.begin(),
            static_cast<ssize_t>(message_size))};
    if (pad_size == 1)
    {
        *start_of_padding = circ->PutCONSGate(0x81U, 8);
    }
    else
    {
        auto zero{circ->PutCONSGate(0x00U, 8)};
        *start_of_padding = circ->PutCONSGate(d, 8);
        std::fill(start_of_padding + 1, input_shares.end() - 1, zero);
        *input_shares.rbegin() = circ->PutCONSGate(0x80U, 8);
    }
    return input_shares;
}

std::vector<share_p> build_keccak(CircuitW_p circ, std::vector<share_p> input_shares, size_t capacity_bits, uint8_t d, size_t output_bits)
{
    assert(capacity_bits % 8 == 0);
    assert(output_bits % 8 == 0);
    size_t capacity_bytes{capacity_bits / 8};
    size_t rate_bytes{200 - capacity_bytes};
    size_t output_bytes{output_bits / 8};
    auto padded_shares{build_padding(circ, input_shares, rate_bytes, d)};
    std::cerr << "after padding: " << circ->circ_->m_cCircuit->GetGateHead() << " gates\n";
    StateShare state{circ, build_permutation, capacity_bytes};
    std::cerr << "after state init: " << circ->circ_->m_cCircuit->GetGateHead() << " gates\n";

    assert(padded_shares.size() % state.rate_bytes == 0);

    // blockwise absorb
    auto next_block{padded_shares.cbegin()};
    for (; next_block != padded_shares.cend(); next_block += static_cast<ptrdiff_t>(state.rate_bytes))
    {
        state.absorb(next_block);
        std::cerr << "after absorb: " << circ->circ_->m_cCircuit->GetGateHead() << " gates\n";
    }
    // squeeze
    std::vector<share_p> output(output_bytes);
    auto full_blocks{output_bytes / state.rate_bytes};
    auto extra_bytes{output_bytes % state.rate_bytes};
    auto last_full_block{std::next(output.begin(),
            static_cast<ptrdiff_t>(full_blocks * state.rate_bytes))};
    for (auto it{output.begin()}; it != last_full_block;
         it += static_cast<ptrdiff_t>(state.rate_bytes))
    {
        state.squeeze(it);
        std::cerr << "after squeeze: " << circ->circ_->m_cCircuit->GetGateHead() << " gates\n";
    }
    auto tmp{state.squeeze()};
    std::copy(tmp.cbegin(), std::next(tmp.cbegin(), static_cast<ptrdiff_t>(extra_bytes)), last_full_block);
        std::cerr << "sha2 end: " << circ->circ_->m_cCircuit->GetGateHead() << " gates\n";
    return output;
}


std::vector<share_p> build_sha3_256(CircuitW_p circ, std::vector<share_p> input_shares)
{
    return build_keccak(circ, input_shares, 512, 0x06, 256);
}

std::vector<share_p> build_sha3_512(CircuitW_p circ, std::vector<share_p> input_shares)
{
    return build_keccak(circ, input_shares, 1024, 0x06, 512);
}
