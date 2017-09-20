#ifndef ITAGPRINTER_HPP
#define ITAGPRINTER_HPP

#include <string>
#include <vector>

#include "Tag.hpp"

namespace CTagsPlugin
{

struct ITagPrinter
{
    virtual ~ITagPrinter() {}

    virtual std::vector<std::string> print(const Tag& p_tag) const = 0;
    virtual std::vector<std::string> titles() const = 0;
};

} // namespace CTagsPlugin

#endif // ITAGPRINTER_HPP
