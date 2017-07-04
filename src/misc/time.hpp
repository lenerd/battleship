#ifndef MISC_TIME_HPP
#define MISC_TIME_HPP

#include <chrono>
#include <string>


std::string print_now(bool ms=false);
std::string print_timestamp(std::chrono::time_point<std::chrono::system_clock> time, bool ms=false);


#endif // MISC_TIME_HPP
