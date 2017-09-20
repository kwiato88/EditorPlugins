#ifndef NPPLISTVIEWSELECTOR_HPP
#define NPPLISTVIEWSELECTOR_HPP

#include "IItemsSelector.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

class NppListViewSelector : public Plugin::IItemsSelector
{
public:
    NppListViewSelector(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule);
    int select(const std::vector<std::string>& p_list);

private:
    WinApi::Handle& m_npp;
	WinApi::InstanceHandle& m_hModule;
};

} // namespace NppPlugin

#endif
