#include "LazyInitializedTagsSelector.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{

LazyInitializedTagsSelector::LazyInitializedTagsSelector(SelectorFactory p_factory)
	: factory(p_factory)
{}

int LazyInitializedTagsSelector::selectTag(const std::vector<TagHolder>& p_tags)
{
	if (selector == nullptr)
		selector = factory();
	return selector->selectTag(p_tags);
}

}
