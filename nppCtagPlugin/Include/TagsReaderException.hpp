#ifndef TAGS_READER_EXCEPTION_HPP
#define TAGS_READER_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace CTagsPlugin
{

struct TagsReaderException : public std::runtime_error
{
    TagsReaderException(std::string p_message)
     : std::runtime_error(p_message)
     {}
};

} // namespace CTagsPlugin

#endif // TAGS_READER_EXCEPTION
