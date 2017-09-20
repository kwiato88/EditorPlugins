#ifndef GENERICKINDTAGPRINTER_HPP
#define GENERICKINDTAGPRINTER_HPP

#include "TagPrinter.hpp"

namespace CTagsPlugin
{

class GenericKindTagPrinter: public CTagsPlugin::TagPrinter
{
public:
    GenericKindTagPrinter(const IConfiguration& p_config);

private:
    virtual std::string kind(const Tag& p_tag) const;
};

} // namespace CTagsPlugin

#endif // GENERICKINDTAGPRINTER_HPP
