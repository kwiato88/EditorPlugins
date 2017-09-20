#ifndef IITEMSSELECTOR_HPP
#define IITEMSSELECTOR_HPP

#include <string>
#include <vector>

namespace Plugin
{

struct IItemsSelector
{
    virtual ~IItemsSelector() {}

    virtual int select(const std::vector<std::string>& p_list) = 0;
};

} // namespace Plugin

#endif /* IITEMSSELECTOR_HPP */
