#include <boost/assign/list_of.hpp>
#include "TagPrinters.hpp"
#include "CppTag.hpp"
#include "GenericKindTag.hpp"
#include "IsTag.hpp"
#include "CppTagPrinter.hpp"
#include "GenericKindTagPrinter.hpp"

namespace CTagsPlugin
{
namespace
{

template <typename TagType, typename PrinterType>
Printer makePrinter(const IConfiguration& p_config)
{
    return std::make_pair(&isTag<TagType>, std::make_shared<PrinterType>(p_config));
}

} // namespace

Printers buildTagPrinters(const IConfiguration& p_config)
{
    Printers printers = boost::assign::list_of
        (makePrinter<CppTag, Cpp::TagPrinter>(p_config))
        (makePrinter<GenericKindTag, GenericKindTagPrinter>(p_config));
    return printers;
}

} // namespace CTagsPlugin
