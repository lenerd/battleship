#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <limits>
#include <map>
#include <stdexcept>
#include "util.hpp"


bytes_t random_bytes(size_t n)
{
    if (n > std::numeric_limits<std::streamsize>::max())
    {
        throw std::range_error("random_bytes: cannot cast size_t to streamsize");
    }
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    bytes_t result(n);
    urandom.read(reinterpret_cast<char*>(result.data()),
                 static_cast<std::streamsize>(result.size()));
    return result;
}


static const std::string hex_digits{"0123456789abcdef"};
static const std::string hex_digits_upper{"0123456789ABCDEF"};
static const std::map<char, uint8_t> hex2val{
    {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4},
    {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
    {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}, 
    {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}};

std::string hexlify(const bytes_t &data, bool upper)
{

    auto &digits{upper ? hex_digits_upper : hex_digits};
    std::string hex;
    hex.reserve(2 * data.size());

    for (auto &b : data)
    {
        hex.push_back(digits[(b >> 4) & 0x0f]);
        hex.push_back(digits[b & 0x0f]);
    }

    return hex;
}

bytes_t unhexlify(const std::string &hex)
{
    assert(hex.size() % 2 == 0);
    assert(std::all_of(hex.cbegin(), hex.cend(),
                [] (auto c) { return std::isxdigit(c) != 0; }));
    bytes_t data;
    data.reserve(hex.size() / 2);
    for (auto it{hex.cbegin()}; it != hex.cend(); it += 2)
    {
        uint8_t upper_nibble{hex2val.at(*it)};
        uint8_t lower_nibble{hex2val.at(*(it+1))};
        data.push_back(static_cast<uint8_t>((upper_nibble << 4) | lower_nibble));
    }
    return data;
}
