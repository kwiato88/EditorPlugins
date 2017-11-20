#include <boost/assign/list_of.hpp>

#include "Tag.hpp"
#include "TagBuilders.hpp"
#include "CppTagBuilder.hpp"
#include "GenericKindTagBuilder.hpp"

namespace CTagsPlugin
{

std::map<std::string, TagBuilder> buildTagBuilders()
{
    std::map<std::string, TagBuilder> builders = boost::assign::map_list_of
        ("C++", &Cpp::buildTag)
        ("C", &Cpp::buildTag)
        ("TTCN", &buildGenericKindTag);
    return builders;
}

} // namespace CTagsPlugin
