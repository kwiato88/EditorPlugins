#include "TagsHierarchy.hpp"
#include "CtagsMeasTags.hpp"
#include "Samples.hpp"

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
	if (hierarchy.find(p_tag.getName()) == hierarchy.end())
		hierarchy.emplace(p_tag.getName(), buildHierarchy(p_tag, p_tags));
	return hierarchy.at(p_tag.getName());
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
