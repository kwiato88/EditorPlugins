#pragma once

#include <vector>
#include <functional>

#include "Tree.hpp"
#include "TagHolder.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

using TagHierarchyItem = Plugin::Tree<TagHolder>;

class TagHierarchy
{
public:
	TagHierarchyItem upHierarchy;
	TagHierarchyItem downHierarchy;

	TagHierarchy(const ITagsReader& p_tags, const Tag& p_tag);
	TagHierarchy(const TagHierarchy& p_other);
	TagHierarchy& operator=(const TagHierarchy& p_other);

	void parse();

private:
	TagHolder tag;
	const ITagsReader& tags;
};

}
