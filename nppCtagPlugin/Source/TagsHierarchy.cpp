#include "TagsHierarchy.hpp"
#include "CtagsMeasTags.hpp"
#include "Samples.hpp"
#include "Counters.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{

TagsHierarchy::TagsHierarchy(const IConfiguration& p_config)
	: config(p_config)
{}

TagHierarchy TagsHierarchy::get(const Tag& p_tag, const ITagsReader& p_tags)
{
	if (config.shouldCacheTags())
		return getCached(p_tag, p_tags);
	return buildHierarchy(p_tag, p_tags);
}

TagHierarchy TagsHierarchy::getCached(const Tag& p_tag, const ITagsReader& p_tags)
{
	Meas::Counter<TagsHierarchyCallCount>::increment();
	if (hasHierarchy(p_tag))
		hierarchy.emplace(p_tag.getName(), buildHierarchy(p_tag, p_tags));
	return hierarchy.at(p_tag.getName());
}

bool TagsHierarchy::hasHierarchy(const Tag& p_tag) const
{
	bool alreadyCached = hierarchy.find(p_tag.getName()) == hierarchy.end();
	LOG_DEBUG << "Cache for tag " << (alreadyCached ? "present" : "not present");
	if (alreadyCached)
		Meas::Counter<TagsHierarchyCacheHits>::increment();
	return alreadyCached;
}

TagHierarchy TagsHierarchy::buildHierarchy(const Tag& p_tag, const ITagsReader& p_tags)
{
	Meas::ExecutionTimeSample<ParseTagsHierarchyTime> meas;
	return TagHierarchy(p_tags, p_tag);
}

void TagsHierarchy::clear()
{
	hierarchy.clear();
}

}
