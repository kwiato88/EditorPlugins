#ifndef BUILDTAG_HPP
#define BUILDTAG_HPP

#include <string>
#include <vector>
#include <functional>
#include "TagHolder.hpp"

namespace CTagsPlugin
{

struct Field
{
    Field()
     : name(), value()
    {}
    Field(const std::string& p_name, const std::string& p_value)
     : name(p_name), value(p_value)
    {}

    std::string name;
    std::string value;
};

typedef std::function<TagHolder (const Tag& p_baseTag, const std::vector<Field>& p_extFields)> TagBuilder;

} // namespace CTagsPlugin

#endif // BUILDTAG_HPP
