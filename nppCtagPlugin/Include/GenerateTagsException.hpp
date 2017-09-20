#ifndef GENERATETAGSEXCEPTION_HPP
#define GENERATETAGSEXCEPTION_HPP

#include <stdexcept>

namespace CTagsPlugin
{

struct GenerateTagsException : public std::runtime_error
{
    GenerateTagsException()
     : std::runtime_error("Generate tags file failed.")
     {}
};

} // namespace CTagsPlugin

#endif /* GENERATETAGSEXCEPTION_HPP */
