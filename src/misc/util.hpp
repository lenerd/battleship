#ifndef UTIL_HPP
#define UTIL_HPP

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <vector>

using bytes_t = std::vector<uint8_t>;
using coords_t = std::pair<size_t, size_t>;

bytes_t random_bytes(size_t n);

template <size_t len>
std::bitset<len> string_to_bits(std::string s)
{
    return std::bitset<len>(std::string(s.crbegin(), s.crend()));
}

template <size_t len>
bytes_t bits_to_bytes(std::bitset<len> bits)
{
    bytes_t bytes;
    size_t num_bytes{bits.size() / 8};
    for (size_t n{0}; n < num_bytes; ++n)
    {
        uint8_t byte{0};
        for (size_t i{0}; i < 8; ++i)
        {
            byte |= bits[8 * n + i] << i;
        }

        bytes.push_back(byte);
    }
    return bytes;
}

std::string hexlify(const bytes_t &data, bool upper=false);
bytes_t unhexlify(const std::string &hex);

#endif // UTIL_HPP
