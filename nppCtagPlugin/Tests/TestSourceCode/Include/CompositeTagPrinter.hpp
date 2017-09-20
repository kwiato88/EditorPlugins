#ifndef COMPOSITETAGPRINTER_HPP
#define COMPOSITETAGPRINTER_HPP

#include <list>
#include <memory>
#include <utility>
#include <functional>

#include "TagPrinter.hpp"

namespace CTagsPlugin
{

typedef std::function<bool(const Tag&)> ShouldApply;
typedef std::pair<ShouldApply, std::shared_ptr<ITagPrinter>> Printer;
typedef std::list<Printer> Printers;

class CompositeTagPrinter: public CTagsPlugin::TagPrinter
{
public:
    CompositeTagPrinter(const IConfiguration& p_config,  Printers& p_printers);

    std::vector<std::string> print(const Tag& p_tag) const;

private:
    Printers m_printers;
};

} // namespace CTagsPlugin

#endif // COMPOSITETAGPRINTER_HPP