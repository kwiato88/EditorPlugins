#ifndef GRID_VIEW_TAGS_SELECTOR_HPP
#define GRID_VIEW_TAGS_SELECTOR_HPP

#include <memory>

#include "PluginInterface.h"
#include "ITagsSelector.hpp"
#include "ITagPrinter.hpp"

namespace CTagsPlugin
{

class GridViewTagsSelector : public ITagsSelector
{
public:
    GridViewTagsSelector(NppData& p_nppData, HINSTANCE& p_hModule,
        std::shared_ptr<ITagPrinter> p_printer);

    int selectTag(const std::vector<TagHolder>& p_tags);

private:
    NppData& m_nppData;
	HINSTANCE& m_hModule;
    std::shared_ptr<ITagPrinter> m_printer;
};

} // namespace CTagsPlugin

#endif // GRID_VIEW_TAGS_SELECTOR_HPP
