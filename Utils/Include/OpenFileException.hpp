#ifndef OPEN_FILE_EXCEPTION_HPP
#define OPEN_FILE_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace Plugin
{

struct OpenFileException : public std::runtime_error
{
    OpenFileException(std::string p_message)
     : std::runtime_error(p_message)
     {}
};

} // namespace Plugin

#endif // OPEN_FILE_EXCEPTION
