#include "util.hpp"


bytes_t random_bytes(size_t n)
{
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    bytes_t result(n);
    urandom.read(reinterpret_cast<char*>(result.data()), result.size());
    return result;
}
