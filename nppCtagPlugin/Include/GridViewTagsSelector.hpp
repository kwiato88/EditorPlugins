#ifndef GRID_VIEW_TAGS_SELECTOR_HPP
#define GRID_VIEW_TAGS_SELECTOR_HPP

#include <memory>

#include "WinApiTypes.hpp"
#include "ITagsSelector.hpp"
#include "IConfiguration.hpp"

namespace CTagsPlugin
{

class GridViewTagsSelector : public ITagsSelector
{
public:
    GridViewTagsSelector(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule,
        const IConfiguration& p_config);

    int selectTag(const std::vector<TagHolder>& p_tags);

private:
	WinApi::Handle& m_parrent;
	WinApi::InstanceHandle& m_hModule;
	const IConfiguration& m_config;
};

} // namespace CTagsPlugin

#endif // GRID_VIEW_TAGS_SELECTOR_HPP
