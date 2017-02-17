#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <cstdint>
#include <istream>
#include <ostream>


enum class Role
{
    client,
    server,
};

enum class UIType
{
    ncurses,
    web,
};

struct Options
{
    Role role;
    UIType ui_type;
    // network options
    std::string address;
    uint16_t game_port;
    uint16_t aby_port;
    uint16_t ws_port;
    uint16_t http_port;
};


std::istream& operator>>(std::istream &is, Role &role);
std::ostream& operator<<(std::ostream &os, const Role &role);
std::istream& operator>>(std::istream &is, UIType &type);
std::ostream& operator<<(std::ostream &os, const UIType &type);

#endif // OPTIONS_HPP
