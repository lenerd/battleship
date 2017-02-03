#include <fstream>
#include <limits>
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
