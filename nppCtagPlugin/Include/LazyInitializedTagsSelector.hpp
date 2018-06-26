#pragma once

#include <memory>
#include <functional>

#include "ITagsSelector.hpp"

namespace CTagsPlugin
{

typedef std::function<std::unique_ptr<ITagsSelector>()> SelectorFactory;

class LazyInitializedTagsSelector : public ITagsSelector
{
public:
	LazyInitializedTagsSelector(SelectorFactory p_factory);
	int selectTag(const std::vector<TagHolder>& p_tags) override;

private:
	SelectorFactory factory;
	std::unique_ptr<ITagsSelector> selector;
};

}
