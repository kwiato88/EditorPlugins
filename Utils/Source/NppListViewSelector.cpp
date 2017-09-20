#include "NppListViewSelector.hpp"
#include "ListBoxDialog.hpp"

namespace NppPlugin
{

NppListViewSelector::NppListViewSelector(WinApi::Handle& p_npp, WinApi::InstanceHandle& p_hModule)
 : m_npp(p_npp), m_hModule(p_hModule)
{
}

int NppListViewSelector::select(const std::vector<std::string>& p_list)
{
    if(p_list.empty())
        return -1;

	WinApi::ListBoxDialog l_dialog(m_hModule, m_npp);
	l_dialog.setItems(p_list);
	l_dialog.show();
	return l_dialog.getSelectedItemIndex();
}

} // namespace NppPlugin
