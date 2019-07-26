#include "ChildrenTags.hpp"
#include "CtagsMeasTags.hpp"
#include "Samples.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{

ChildrenTags::ChildrenTags(std::shared_ptr<ITagsReader> p_tags, const IConfiguration& p_config)
	: tags(p_tags), config(p_config)
{}

void ChildrenTags::clear()
{
	LOG_INFO << "Clear cache";
	childrenCache.clear();
}

std::vector<TagHolder> ChildrenTags::get(const Tag& p_parent)
{
	if (config.shouldCacheTags())
		return getCachedChildrenTags(p_parent);
	return findChildrenTags(p_parent);
}

std::vector<TagHolder> ChildrenTags::getCachedChildrenTags(const Tag& p_parent)
{
	LOG_INFO << "Get children for " << p_parent.getName();
	if (!hasChildren(p_parent))
		childrenCache[p_parent.getName()] = findChildrenTags(p_parent);
	return childrenCache.at(p_parent.getName());
}

std::vector<TagHolder> ChildrenTags::findChildrenTags(const Tag& p_parent)
{
	LOG_INFO << "Find children for " << p_parent.getName();
	Meas::ExecutionTimeSample<FindChildrenTagsTime> meas;
	return tags->findTag([&](const Tag& t) { return t.isChild(p_parent); });
}


bool ChildrenTags::hasChildren(const Tag& p_parent) const
{
	return childrenCache.find(p_parent.getName()) != childrenCache.end();
}

}