#ifndef ISTAG_HPP
#define ISTAG_HPP

#include <exception>
#include "Tag.hpp"

namespace CTagsPlugin
{

template <typename SpecificTag>
bool isTag(const Tag& p_tag)
{
    try
    {
        dynamic_cast<const SpecificTag&>(p_tag);
        return true;
    }
    catch(std::bad_cast&)
    {
        return false;
    }
}

} // namespace CTagsPlugin

#endif // ISTAG_HPP
