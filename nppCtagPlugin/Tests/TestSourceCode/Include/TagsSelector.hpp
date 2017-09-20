#ifndef TAGS_SELECTOR_HPP
#define TAGS_SELECTOR_HPP

#include <vector>
#include <boost\shared_ptr.hpp>

#include "ITagsSelector.hpp"
#include "IItemsSelector.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

class TagsSelector : public ITagsSelector
{
public:
	TagsSelector(boost::shared_ptr<Plugin::IItemsSelector> p_printer);

	int selectTag(const std::vector<TagHolder>& p_tags);

private:
	boost::shared_ptr<Plugin::IItemsSelector> m_selector;
	std::vector<TagHolder> cutFilesDirs(const std::vector<TagHolder>& p_tags);
};

} // namespace CTagsPlugin
#endif // TAGS_SELECTOR_HPP
