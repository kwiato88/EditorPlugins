#ifndef BUILDTAG_HPP
#define BUILDTAG_HPP

#include <string>
#include <vector>
#include <functional>
#include "TagHolder.hpp"
#include "ExtensionFields.hpp"

namespace CTagsPlugin
{

typedef std::function<TagHolder (const Tag& p_baseTag, const ExtensionFields& p_extFields)> TagBuilder;

} // namespace CTagsPlugin

#endif // BUILDTAG_HPP
