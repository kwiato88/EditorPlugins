#ifndef I_TAGS_SELECTOR_HPP
#define I_TAGS_SELECTOR_HPP

#include <vector>
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

struct ITagsSelector
{
    virtual ~ITagsSelector() {}

    virtual int selectTag(const std::vector<TagHolder>& p_tags) = 0;
};

} // namespace CTagsPlugin

#endif // I_TAGS_SELECTOR_HPP
