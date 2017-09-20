#ifndef CPPTAGPRINTER_HPP
#define CPPTAGPRINTER_HPP

#include "TagPrinter.hpp"

namespace CTagsPlugin
{
namespace Cpp
{

class TagPrinter: public CTagsPlugin::TagPrinter
{
public:
    TagPrinter(const IConfiguration& p_config);

private:
    virtual std::string kind(const Tag& p_tag) const;
    virtual std::string access(const Tag& p_tag) const;
    virtual std::string type(const Tag& p_tag) const;
};

} // namespace Cpp
} // namespace CTagsPlugin

#endif // CPPTAGPRINTER_HPP
