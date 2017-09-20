#ifndef LIST_VIEW_TAGS_SELECTOR_HPP
#define LIST_VIEW_TAGS_SELECTOR_HPP

#include <vector>
#include <memory>

#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "WinApiTypes.hpp"

namespace CTagsPlugin
{

class ListViewTagsSelector : public ITagsSelector
{
public:
	ListViewTagsSelector(WinApi::InstanceHandle& p_hInstance, WinApi::Handle& p_parrent);

	int selectTag(const std::vector<TagHolder>& p_tags);

private:
	WinApi::InstanceHandle& hInstance;
	WinApi::Handle& parrent;
	std::vector<TagHolder> cutFilesDirs(const std::vector<TagHolder>& p_tags);
};

} // namespace CTagsPlugin
#endif // LIST_VIEW_TAGS_SELECTOR_HPP
