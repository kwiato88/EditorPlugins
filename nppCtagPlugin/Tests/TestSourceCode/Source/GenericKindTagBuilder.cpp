#include <boost/range/algorithm/find_if.hpp>

#include "GenericKindTagBuilder.hpp"
#include "GenericKindTag.hpp"

namespace CTagsPlugin
{
namespace
{
std::string getKind(const std::vector<Field>& p_extFields)
{
    auto kind = boost::range::find_if(p_extFields, [&](const Field& f){ return f.name == "kind"; });
    return (kind == p_extFields.end()) ? "" : kind->value;
}
} // namespace

TagHolder buildGenericKindTag(const Tag& p_baseTag, const std::vector<Field>& p_extFields)
{
    GenericKindTag tag;
    tag.assign(p_baseTag);
    tag.kind = getKind(p_extFields);
    return tag;
}

} // namespace CTagsPlugin
