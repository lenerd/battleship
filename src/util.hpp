#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <fstream>
#include <vector>

using bytes_t = std::vector<uint8_t>;
using coords_t = std::pair<int, int>;

enum class Role
{
    client,
    server,
};

bytes_t random_bytes(size_t n);

#endif // UTIL_HPP
