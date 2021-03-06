#ifndef CPPTAGPARSER_HPP
#define CPPTAGPARSER_HPP

#include "TagHolder.hpp"
#include "BuildTag.hpp"

namespace CTagsPlugin
{
namespace Cpp
{

TagHolder buildTag(const Tag& p_baseTag, const std::vector<Field>& p_extFields);

} // namespace Cpp
} // namespace CTagsPlugin

#endif // CPPTAGPARSER_HPP
