#ifndef GAME_EXCEPT_HPP
#define GAME_EXCEPT_HPP

#include <exception>


class CheatingException : std::exception
{
public:
    explicit CheatingException(const std::string &what_arg)
        : what_str_(what_arg.c_str()) {}
    explicit CheatingException(const char *what_arg)
        : what_str_(what_arg) {}
    virtual const char* what() const noexcept override
    {
        return what_str_.c_str();
    }
private:
    std::string what_str_;
};

#endif // GAME_EXCEPT_HPP
