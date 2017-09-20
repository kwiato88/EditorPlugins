#ifndef SHELL_COMMAND_EXCEPTION_HPP
#define SHELL_COMMAND_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace Plugin
{

struct ShellCommandException : public std::runtime_error
{
    ShellCommandException(std::string p_message)
     : std::runtime_error(p_message)
     {}
};

} // namespace Plugin

#endif // SHELL_COMMAND_EXCEPTION
