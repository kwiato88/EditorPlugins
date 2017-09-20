#ifndef I_PATHS_SELECTOR_HPP
#define I_PATHS_SELECTOR_HPP

#include <string>
#include <vector>

namespace Plugin
{

struct IPathsSelector
{
    virtual ~IPathsSelector() {}

    virtual std::vector<std::string> select(const std::vector<std::string>& p_initialPaths,
                                            const std::string& p_startingPath) = 0;
};

} // namespace Plugin

#endif
