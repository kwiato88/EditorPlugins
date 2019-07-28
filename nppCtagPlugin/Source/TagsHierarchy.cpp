#include "TagsHierarchy.hpp"
#include "CtagsMeasTags.hpp"
#include "Samples.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{

TagsHierarchy::TagsHierarchy(const IConfiguration& p_config)
	: config(p_config)
{}

TagHierarchy TagsHierarchy::get(const Tag& p_tag, const ITagsReader& p_tags)
{
	LOG_INFO << "Get hierarchy for " << p_tag.getName();
	if (config.shouldCacheTags())
		return getCached(p_tag, p_tags);
	return buildHierarchy(p_tag, p_tags);
}

TagHierarchy TagsHierarchy::getCached(const Tag& p_tag, const ITagsReader& p_tags)
{
	if (hierarchy.find(p_tag.getName()) == hierarchy.end())
		hierarchy.emplace(p_tag.getName(), buildHierarchy(p_tag, p_tags));
	return hierarchy.at(p_tag.getName());
}

TagHierarchy TagsHierarchy::buildHierarchy(const Tag& p_tag, const ITagsReader& p_tags)
{
	LOG_INFO << "Build hierarchy for " << p_tag.getName();
	Meas::ExecutionTimeSample<ParseTagsHierarchyTime> meas;
	return TagHierarchy(p_tags, p_tag);
}

void TagsHierarchy::clear()
{
	LOG_INFO << "Clear cache";
	hierarchy.clear();
}

}
