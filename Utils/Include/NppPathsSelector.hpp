#ifndef NPP_PATHS_SELECTOR_HPP
#define NPP_PATHS_SELECTOR_HPP

#include <vector>
#include <string>

#include "PluginInterface.h"
#include "IPathsSelector.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

enum class SelectorType
{
    Directory,
    File
};

template <SelectorType selector>
class PathsSelector : public Plugin::IPathsSelector
{
public:
    PathsSelector(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule);

    std::vector<std::string> select(const std::vector<std::string>& p_initialPaths,
                                    const std::string& p_startingPath);

private:
	WinApi::Handle& m_npp;
	WinApi::InstanceHandle& m_hModule;
};

} // namespace NppPlugin

#endif
