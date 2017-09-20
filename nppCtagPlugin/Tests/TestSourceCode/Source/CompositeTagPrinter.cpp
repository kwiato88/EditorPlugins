#include "CompositeTagPrinter.hpp"

namespace CTagsPlugin
{

CompositeTagPrinter::CompositeTagPrinter(const IConfiguration& p_config,  Printers& p_printers)
    : CTagsPlugin::TagPrinter(p_config), m_printers(p_printers)
{
}

std::vector<std::string> CompositeTagPrinter::print(const Tag& p_tag) const
{
    for(const auto& printer : m_printers)
        if(printer.first(p_tag))
            return printer.second->print(p_tag);
    return TagPrinter::print(p_tag);
}

} // namespace CTagsPlugin
