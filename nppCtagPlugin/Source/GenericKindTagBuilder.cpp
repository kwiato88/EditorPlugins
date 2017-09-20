#include "GenericKindTagBuilder.hpp"
#include "GenericKindTag.hpp"

namespace CTagsPlugin
{

TagHolder buildGenericKindTag(const Tag& p_baseTag, const ExtensionFields& p_extFields)
{
    GenericKindTag tag;
    tag.assign(p_baseTag);
    tag.kind = p_extFields.getStringValue("kind", "");
    return tag;
}

} // namespace CTagsPlugin
