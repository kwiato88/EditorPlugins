#pragma once

#include <vector>
#include <functional>

#include "Tree.hpp"
#include "TagHolder.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

class TagHierarchyItem
{
public:
	TagHolder tag;
	std::vector<TagHierarchyItem> relatedTags;

	TagHierarchyItem(const Tag& p_tag,
		const std::function<std::vector<TagHolder>(const ITagsReader&, const Tag&)>& p_relaredTagsParser);
	TagHierarchyItem(const TagHierarchyItem& p_other);

	void parse(const ITagsReader& p_tags);
	std::vector<TagHolder> getRelatedTags() const;

private:
	std::function<std::vector<TagHolder>(const ITagsReader&, const Tag&)> relatedTagsParser;
};

class TagHierarchy
{
public:
	TagHierarchyItem upHierarchy;
	TagHierarchyItem downHierarchy;

	TagHierarchy(const ITagsReader& p_tags, const Tag& p_tag);
	TagHierarchy(const TagHierarchy& p_other);

	void parse();

private:
	const ITagsReader& tags;
};

}
