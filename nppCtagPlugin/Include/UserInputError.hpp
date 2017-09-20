#pragma once

#include <stdexcept>
#include <string>

namespace Plugin
{

struct UserInputError: public std::runtime_error
{
    UserInputError(const std::string& p_message)
        : std::runtime_error(p_message)
    {}
};

}
