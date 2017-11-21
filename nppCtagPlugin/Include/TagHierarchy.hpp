#pragma once

#include <vector>
#include <functional>

#include "Tree.hpp"
#include "TagHolder.hpp"
#include "ITagsReader.hpp"

#include <unordered_map>

namespace CTagsPlugin
{

using TagHierarchyItem = Plugin::Tree<TagHolder>;

struct TagNameHash
{
	std::size_t operator()(const TagHolder& p) const noexcept;
};
//TODO: extract to different file TagsHierarchy
class TagsHierarchy
{
public:
	void update(const ITagsReader& p_tags);
	TagHierarchyItem getUpHierarchy(const Tag& p_tag);
	TagHierarchyItem getDownHierarchy(const Tag& p_tag);
private:
	void buildBase(const std::vector<TagHolder>& p_complexTags, const ITagsReader& p_tags);
	void buildDerived();
	void updateDerived(const TagHolder& p_derived, const std::vector<TagHolder>& p_base);

	std::unordered_map<TagHolder, std::vector<TagHolder>, TagNameHash> baseTags;
	std::unordered_map<TagHolder, std::vector<TagHolder>, TagNameHash> derivedTags;
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
	TagHolder tag;
	const ITagsReader& tags;
	TagsHierarchy allTagsHierarchy;
};

}
