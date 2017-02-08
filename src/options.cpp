#include "options.hpp"
#include <boost/program_options/errors.hpp>

namespace po = boost::program_options;


std::istream& operator>>(std::istream &is, Role &role)
{
    std::string token;
    is >> token;
    if (token == "server")
        role = Role::server;
    else if (token == "client")
        role = Role::client;
    else
        throw po::invalid_option_value(token);
    return is;
}

std::ostream& operator<<(std::ostream &os, const Role &role)
{
    switch (role)
    {
        case Role::server:
            os << "server";
            break;
        case Role::client:
            os << "client";
            break;
    }
    return os;
}

std::istream& operator>>(std::istream &is, UIType &type)
{
    std::string token;
    is >> token;
    if (token == "ncurses")
        type = UIType::ncurses;
    else if (token == "web")
        type = UIType::web;
    else
        throw po::invalid_option_value(token);
    return is;
}

std::ostream& operator<<(std::ostream &os, const UIType &type)
{
    switch (type)
    {
        case UIType::ncurses:
            os << "ncurses";
            break;
        case UIType::web:
            os << "web";
            break;
    }
    return os;
}
