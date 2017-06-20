#include "keccak.hpp"


template <>
const size_t Keccak<uint64_t>::num_rounds = 24;
template <>
const size_t Keccak<uint32_t>::num_rounds = 22;
template <>
const size_t Keccak<uint16_t>::num_rounds = 20;
// template <>
// const size_t Keccak<uint8_t>::num_rounds = 18;
