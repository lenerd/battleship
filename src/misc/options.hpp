#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <cstdint>
#include <istream>
#include <ostream>


/**
 * A role in the client-server setting
 */
enum class Role
{
    client,
    server,
};

/**
 * The supported user interfaces
 */
enum class UIType
{
    ncurses,
    web,
};

/**
 * Configurable options of the application.
 */
struct Options
{
    Role role;
    UIType ui_type;
    bool demo;
    // network options
    std::string address;
    uint16_t game_port;
    uint16_t aby_port;
    uint16_t ws_port;
    uint16_t http_port;
};


/**
 * iostream support for the Role and UIType enums
 */
std::istream& operator>>(std::istream &is, Role &role);
std::ostream& operator<<(std::ostream &os, const Role &role);
std::istream& operator>>(std::istream &is, UIType &type);
std::ostream& operator<<(std::ostream &os, const UIType &type);

#endif // OPTIONS_HPP
