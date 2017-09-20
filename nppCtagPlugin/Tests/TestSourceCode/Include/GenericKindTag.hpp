#ifndef GENERICKINDTAG_HPP
#define GENERICKINDTAG_HPP

#include "Tag.hpp"

namespace CTagsPlugin
{

struct GenericKindTag: public Tag
{
    virtual Tag* clone() const
    {
        return new GenericKindTag(*this);
    }
    virtual size_t size()
    {
        return (sizeof(GenericKindTag) + kind.size() * sizeof(std::string::value_type)
            + Tag::size());
    }

    std::string kind;
};

} // namespace CTagsPlugin

#endif // GENERICKINDTAG_HPP
