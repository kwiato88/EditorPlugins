#ifndef TAGBUILDERS_HPP
#define TAGBUILDERS_HPP

#include <map>
#include <functional>

#include "BuildTag.hpp"

namespace CTagsPlugin
{

std::map<std::string, TagBuilder> buildTagBuilders();

} // namespace CTagsPlugin

#endif // TAGBUILDERS_HPP
