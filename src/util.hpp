#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

using bytes_t = std::vector<uint8_t>;
using coords_t = std::pair<size_t, size_t>;

enum class Role
{
    client,
    server,
};

bytes_t random_bytes(size_t n);

#endif // UTIL_HPP
