#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "time.hpp"

std::string print_now(bool ms, bool human)
{
    if (human)
    {
        auto tp = std::chrono::system_clock::now();
        return print_timestamp_human(tp, ms);
    }
    else
    {
        auto unix_timestamp = std::chrono::seconds(std::time(NULL));
        return std::to_string(unix_timestamp.count());
    }
}

std::string print_timestamp_human(std::chrono::time_point<std::chrono::system_clock> tp, bool ms)
{
    std::time_t tp_time = std::chrono::system_clock::to_time_t(tp);
    auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    auto fraction = tp - seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&tp_time), "%F %T");
    if (ms)
       ss << "." << std::setw(3) << std::setfill('0') << milliseconds.count();

    return ss.str();
}
