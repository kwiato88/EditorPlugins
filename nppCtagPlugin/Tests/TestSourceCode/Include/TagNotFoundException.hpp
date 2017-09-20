#ifndef TAGNOTFOUNDEXCEPTION_HPP
#define TAGNOTFOUNDEXCEPTION_HPP

#include <stdexcept>

namespace CTagsPlugin
{

struct TagNotFoundException : public std::runtime_error
{
    TagNotFoundException() : std::runtime_error("Tag not found")
    {}
};

}  // namespace CTagsPlugin

#endif /* TAGNOTFOUNDEXCEPTION_HPP */
