#include "GenericKindTag.hpp"
#include "GenericKindTagPrinter.hpp"

namespace CTagsPlugin
{

GenericKindTagPrinter::GenericKindTagPrinter(const IConfiguration& p_config): CTagsPlugin::TagPrinter(p_config)
{}

std::string GenericKindTagPrinter::kind(const Tag& p_tag) const
{
    const GenericKindTag& tag = dynamic_cast<const GenericKindTag&>(p_tag);
    return tag.kind;
}

} // namespace CTagsPlugin
