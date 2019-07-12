#include "ChildrenTags.hpp"

namespace CTagsPlugin
{

ChildrenTags::ChildrenTags(std::shared_ptr<ITagsReader> p_tags, bool p_useCache)
	: tags(p_tags),
	getImpl(p_useCache
		? std::bind(&ChildrenTags::getCachedChildrenTags, this, std::placeholders::_1)
		: std::bind(&ChildrenTags::findChildrenTags, this, std::placeholders::_1))
{}

void ChildrenTags::clear()
{
	childrenCache.clear();
}

std::vector<TagHolder> ChildrenTags::get(const Tag& p_parent)
{
	return getImpl(p_parent);
}

std::vector<TagHolder> ChildrenTags::getCachedChildrenTags(const Tag& p_parent)
{
	if (!hasChildren(p_parent))
		childrenCache[p_parent.getName()] = findChildrenTags(p_parent);
	return childrenCache.at(p_parent.getName());
}

std::vector<TagHolder> ChildrenTags::findChildrenTags(const Tag& p_parent)
{
	return tags->findTag([&](const Tag& t) { return t.isChild(p_parent); });
}


bool ChildrenTags::hasChildren(const Tag& p_parent) const
{
	return childrenCache.find(p_parent.getName()) != childrenCache.end();
}

}
