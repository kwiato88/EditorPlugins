#ifndef TAGPRINTERS_HPP
#define TAGPRINTERS_HPP

#include <list>
#include <memory>
#include <utility>
#include <functional>

#include "ITagPrinter.hpp"
#include "IConfiguration.hpp"

namespace CTagsPlugin
{

typedef std::function<bool(const Tag&)> ShouldApply;
typedef std::pair<ShouldApply, std::shared_ptr<ITagPrinter>> Printer;
typedef std::list<Printer> Printers;

Printers buildTagPrinters(const IConfiguration& p_config);

} // namespace CTagsPlugin

#endif // TAGPRINTERS_HPP
