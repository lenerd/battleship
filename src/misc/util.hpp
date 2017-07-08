#ifndef UTIL_HPP
#define UTIL_HPP

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <vector>

/**
 * Representation of a byte string
 */
using bytes_t = std::vector<uint8_t>;
/**
 * Representation of two-dimensional coordinates.
 */
using coords_t = std::pair<size_t, size_t>;

/**
 * Writes coordinates as a string:
 * (3,3) |-> (D,3)
 */
std::string coords_to_string(coords_t s);

/**
 * Generate n uniformly random bytes
 */
bytes_t random_bytes(size_t n);

/**
 * Convert a string of zeros and ones to a bitset
 */
template <size_t len>
std::bitset<len> string_to_bits(std::string s)
{
    return std::bitset<len>(std::string(s.crbegin(), s.crend()));
}

/**
 * Convert a bitset to a bytes_t object
 */
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

/**
 * Encode bytes in hexadecimal representation
 */
std::string hexlify(const bytes_t &data, bool upper=false);
/**
 * Decode hexadecimal into bytes
 */
bytes_t unhexlify(const std::string &hex);
/**
 * Convert a string to bytes
 */
bytes_t string_to_bytes(const std::string &str);

#endif // UTIL_HPP
