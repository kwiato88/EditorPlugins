#pragma once

#include <vector>
#include "TagHolder.hpp"

namespace CTagsPlugin
{

struct ITagsSelector
{
    virtual ~ITagsSelector() {}

    virtual int selectTag(const std::vector<TagHolder>& p_tags) = 0;
};

} // namespace CTagsPlugin
