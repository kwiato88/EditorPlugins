#include "ChildrenTags.hpp"

namespace CTagsPlugin
{

ChildrenTags::ChildrenTags(std::shared_ptr<ITagsReader> p_tags)
	: tags(p_tags)
{}

void ChildrenTags::clear()
{
	childrenCache.clear();
}

std::vector<TagHolder> ChildrenTags::get(const Tag& p_parent)
{
	if (!hasChildren(p_parent))
		childrenCache[p_parent.getName()] = tags->findTag([&](const Tag& t) { return t.isChild(p_parent); });
	return childrenCache.at(p_parent.getName());
}

bool ChildrenTags::hasChildren(const Tag& p_parent) const
{
	return childrenCache.find(p_parent.getName()) != childrenCache.end();
}

}
