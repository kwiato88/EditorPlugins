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

    std::vector<std::vector<std::string> > l_gridContent;
	TagPrinter printer(m_config.getSupportedExtensionFileds());
    boost::range::transform(
        p_tags,
        std::back_inserter(l_gridContent),
        [&](const Tag& t) { t.print(printer); return printer.get(); });
    
	WinApi::GridDialog l_dialog(m_hModule, m_parrent, "Select tag");
    l_dialog.setTitles(printer.titles());
    l_dialog.setContent(l_gridContent);
    l_dialog.show();

    return l_dialog.getSelectedItemIndex();
}

} // namespace CTagsPlugin
