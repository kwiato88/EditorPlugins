#ifndef TAGPRINTER_HPP
#define TAGPRINTER_HPP

#include <string>
#include <vector>

#include "Tag.hpp"
#include "ITagPrinter.hpp"
#include "IConfiguration.hpp"

namespace CTagsPlugin
{

class TagPrinter: public ITagPrinter
{
public:
    TagPrinter(const IConfiguration& p_config);

    std::vector<std::string> print(const Tag& p_tag) const;
    std::vector<std::string> titles() const;

private:
    virtual std::string name(const Tag& p_tag) const;
    virtual std::string file(const Tag& p_tag) const;
    virtual std::string path(const Tag& p_tag) const;
    virtual std::string addr(const Tag& p_tag) const;
    virtual std::string kind(const Tag& p_tag) const;
    virtual std::string access(const Tag& p_tag) const;
    virtual std::string type(const Tag& p_tag) const;

protected:
    const IConfiguration& m_config;
};

} // namespace CTagsPlugin

#endif // TAGPRINTER_HPP
