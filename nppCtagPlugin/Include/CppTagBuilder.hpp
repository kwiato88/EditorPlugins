#ifndef CPPTAGPARSER_HPP
#define CPPTAGPARSER_HPP

#include "TagHolder.hpp"
#include "BuildTag.hpp"

namespace CTagsPlugin
{
class CppTag;
namespace Cpp
{

void fill(CppTag& p_outTag, const ExtensionFields& p_fields);
TagHolder buildTag(const Tag& p_baseTag, const ExtensionFields& p_extFields);

} // namespace Cpp
} // namespace CTagsPlugin

#endif // CPPTAGPARSER_HPP
