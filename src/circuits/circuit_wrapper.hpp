#ifndef CIRCUIT_WRAPPER_HPP
#define CIRCUIT_WRAPPER_HPP

#include <algorithm>
#include <bitset>
#include <cassert>
#include <limits>
#include <memory>
#include <numeric>
#include <type_traits>
#include <abycore/circuit/booleancircuits.h>
#include <abycore/circuit/share.h>
#include "misc/util.hpp"

using share_p = std::shared_ptr<share>;

class CircuitWrapper
{
public:
    CircuitWrapper(BooleanCircuit *circ) : circ_(circ) {}

    template <size_t len>
    std::vector<share_p> PutINGates(std::bitset<len> bits, e_role role)
    {
        std::vector<share_p> shares(len);
        for (size_t i{0}; i < len; ++i)
        {
            shares[i] = share_p(circ_->PutINGate(static_cast<uint8_t>(bits[i]), 1, role));
        }
        return shares;
    }

    template <typename T,
              typename = std::enable_if_t<std::is_unsigned<T>::value>>
    share_p PutINGate(T input, size_t size, e_role role)
    {
        assert(size <= sizeof(input) * 8);
        assert(size <= std::numeric_limits<uint32_t>::max());
        return share_p(circ_->PutINGate(input, static_cast<uint32_t>(size), role));
    }

    share_p PutDummyINGate(size_t size)
    {
        assert(size <= std::numeric_limits<uint32_t>::max());
        return share_p(circ_->PutDummyINGate(static_cast<uint32_t>(size)));
    }

    std::vector<share_p> PutDummyINGates(size_t num, size_t size)
    {
        assert(size <= std::numeric_limits<uint32_t>::max());
        std::vector<share_p> shares(num);
        for (size_t i{0}; i < num; ++i)
        {
            shares[i] = share_p(circ_->PutDummyINGate(static_cast<uint32_t>(size)));
        }
        return shares;
    }

    share_p PutOUTGate(share_p ina, e_role role)
    {
        return share_p(circ_->PutOUTGate(ina.get(), role));
    }


    template <typename T,
              typename = std::enable_if_t<std::is_unsigned<T>::value>>
    share_p PutCONSGate(T input, size_t size)
    {
        assert(size <= sizeof(input) * 8);
        assert(size <= std::numeric_limits<uint32_t>::max());
        return share_p(circ_->PutCONSGate(input, static_cast<uint32_t>(size)));
    }


    share_p PutANDGate(share_p ina, share_p inb)
    {
        return share_p(circ_->PutANDGate(ina.get(), inb.get()));
    }

    share_p PutXORGate(share_p ina, share_p inb)
    {
        return share_p(circ_->PutXORGate(ina.get(), inb.get()));
    }

    share_p PutORGate(share_p ina, share_p inb)
    {
        return share_p(circ_->PutANDGate(ina.get(), inb.get()));
    }

    share_p PutADDGate(share_p ina, share_p inb)
    {
        return share_p(circ_->PutADDGate(ina.get(), inb.get()));
    }

    share_p PutINVGate(share_p in)
    {
        return share_p(circ_->PutINVGate(in.get()));
    }

    share_p PutEQGate(share_p ina, share_p inb)
    {
        return share_p(circ_->PutEQGate(ina.get(), inb.get()));
    }

    share_p PutMUXGate(share_p ina, share_p inb, share_p sel)
    {
        return share_p(circ_->PutMUXGate(ina.get(), inb.get(), sel.get()));
    }

    share_p PutROTLGate(share_p in, size_t k)
    {
        size_t bitlength{in->get_bitlength()};
        auto wires{in->get_wires()};
        k %= bitlength;
        auto new_first{std::prev(wires.end(), static_cast<ptrdiff_t>(k))};
        std::rotate(wires.begin(), new_first, wires.end());
        return std::make_shared<boolshare>(wires, circ_);
    }


    // debug gates
    share_p PutPrintValueGate(share_p in, std::string infostring)
    {
        return share_p(circ_->PutPrintValueGate(in.get(), infostring));
    }

    template <typename T,
              typename = std::enable_if_t<std::is_unsigned<T>::value>>
    share_p PutAssertGate(share_p in, T value, size_t size)
    {
        return share_p(circ_->PutAssertGate(in, value, size));
    }


    static share_p get_wire_ids_as_share(share_p share, size_t index)
    {
        assert(index <= std::numeric_limits<uint32_t>::max());
        return share_p(share->get_wire_ids_as_share(static_cast<uint32_t>(index)));
    }


    // share conversion
    std::vector<share_p> split_share(share_p in, size_t bits_per_share)
    {
        assert(in->get_bitlength() % bits_per_share == 0);
        size_t num_shares{in->get_bitlength() / bits_per_share};
        std::vector<share_p> out(num_shares);
        auto in_wires{in->get_wires()};
        for (size_t i{0}; i < num_shares; ++i)
        {
            std::vector<uint32_t> wires(in_wires.cbegin() + static_cast<ptrdiff_t>(i*bits_per_share),
                                        in_wires.cbegin() + static_cast<ptrdiff_t>((i+1)*bits_per_share));

            out[i] = std::make_shared<boolshare>(wires, circ_);
        }
        return out;
    }

    share_p combine_shares(std::vector<share_p> in)
    {
        size_t num_bits{std::accumulate(in.cbegin(), in.cend(), size_t{0},
                [] (size_t acc, share_p s) -> size_t
                { return acc + s->get_bitlength(); })};
        assert(num_bits <= 64);
        std::vector<uint32_t> wires;
        for (auto &share : in)
        {
            auto in_wires{share->get_wires()};
            wires.insert(wires.cend(), in_wires.cbegin(), in_wires.cend());
        }
        return std::make_shared<boolshare>(wires, circ_);
    }


    std::vector<share_p> bytes_to_shares(const bytes_t &in)
    {
        std::vector<share_p> out(in.size());
        std::transform(in.begin(), in.end(), out.begin(),
                [this] (uint8_t b) -> share_p
                { return this->PutCONSGate(b, 8); });
        return out;
    }

    bytes_t shares_to_bytes(const std::vector<share_p> &in)
    {
        bytes_t out(in.size());
        std::transform(in.begin(), in.end(), out.begin(),
            [] (share_p b) -> uint8_t
            {
                assert(b->get_bitlength() == 8);
                return b->template get_clear_value<uint8_t>();
            });
        return out;
    }
// private:
    BooleanCircuit* circ_;
};

using CircuitW_p = std::shared_ptr<CircuitWrapper>;


#endif // CIRCUIT_WRAPPER_HPP
