#pragma once

#include <unordered_map>
#include <vector>

#include "Tree.hpp"
#include "TagHolder.hpp"

namespace CTagsPlugin
{

using TagHierarchyItem = Plugin::Tree<TagHolder>;

class ITagsReader;

struct TagNameHash
{
	std::size_t operator()(const TagHolder& p) const noexcept;
};

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

}
