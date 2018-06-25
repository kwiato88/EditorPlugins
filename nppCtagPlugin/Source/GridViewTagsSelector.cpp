#include <iterator>

#include <boost/range/algorithm/transform.hpp>

#include "GridViewTagsSelector.hpp"
#include "GridDialog.hpp"
#include "TagPrinter.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{

GridViewTagsSelector::GridViewTagsSelector(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule, const IConfiguration& p_config)
    : m_parrent(p_parrent), m_hModule(p_hModule), m_config(p_config)
{
}

int GridViewTagsSelector::selectTag(const std::vector<TagHolder>& p_tags)
{
    if(p_tags.empty())
        return -1;

	TagPrinter tagPrinter(m_config.getSupportedExtensionFileds());
	TagsPrinter<TagPrinter, std::vector<std::string>> printer(tagPrinter);
	for (const Tag& tag : p_tags)
		tag.print(printer);
    
	WinApi::GridDialog l_dialog(m_hModule, m_parrent, "Select tag");
    l_dialog.setTitles(tagPrinter.titles());
    l_dialog.setContent(printer.get());
    l_dialog.show();

    return l_dialog.getSelectedItemIndex();
}

} // namespace CTagsPlugin
