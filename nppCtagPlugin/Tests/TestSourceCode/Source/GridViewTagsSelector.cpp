#include <iterator>

#include <boost/range/algorithm/transform.hpp>

#include "GridViewTagsSelector.hpp"
#include "GridDialog.hpp"
#include "getCurrentHScintilla.hpp"

namespace CTagsPlugin
{

GridViewTagsSelector::GridViewTagsSelector(NppData& p_nppData, HINSTANCE& p_hModule, std::shared_ptr<ITagPrinter> p_printer)
    : m_nppData(p_nppData), m_hModule(p_hModule), m_printer(p_printer)
{
}

int GridViewTagsSelector::selectTag(const std::vector<TagHolder>& p_tags)
{
    if(p_tags.empty())
        return -1;

    std::vector<std::vector<std::string> > l_gridContent;
    boost::range::transform(
        p_tags,
        std::back_inserter(l_gridContent),
        [&](const Tag& t) { return m_printer->print(t); });

    WinApi::GridDialog l_dialog(m_hModule, NppPlugin::getCurrentHScintilla(m_nppData));
    l_dialog.setTitles(m_printer->titles());
    l_dialog.setContent(l_gridContent);
    l_dialog.show();

    return l_dialog.getSelectedItemIndex();
}

} // namespace CTagsPlugin
