#include "NppPathsSelector.hpp"
#include "SelectPathsDialog.hpp"

namespace NppPlugin
{

template <NppPlugin::SelectorType selector>
constexpr WinApi::SelectorType getSelectorType()
{
	return WinApi::SelectorType::Directory;
}

template <>
constexpr WinApi::SelectorType getSelectorType<NppPlugin::SelectorType::Directory>()
{
	return WinApi::SelectorType::Directory;
};

template <>
constexpr WinApi::SelectorType getSelectorType<NppPlugin::SelectorType::File>()
{
	return WinApi::SelectorType::File;
};

template <SelectorType selector>
PathsSelector<selector>::PathsSelector(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule)
    : m_npp(p_npp), m_hModule(p_hModule)
{}

template <SelectorType selector>
std::vector<std::string> PathsSelector<selector>::select(const std::vector<std::string>& p_initialPaths,
                                                         const std::string& p_startingPath)
{
    WinApi::SelectPathsDialog<getSelectorType<selector>()> l_dialog(m_hModule, m_npp);
    l_dialog.setInitalPath(p_startingPath);
    l_dialog.setPaths(p_initialPaths);
    if(l_dialog.show() == WinApi::SelectPathsDialog<getSelectorType<selector>()>::BUTTON_OK)
        return l_dialog.getPaths();
    else
        return p_initialPaths;
}

template class PathsSelector<SelectorType::Directory>;
template class PathsSelector<SelectorType::File>;

} // namespace NppPlugin
