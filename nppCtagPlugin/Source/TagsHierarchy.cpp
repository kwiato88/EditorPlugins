#include "TagsHierarchy.hpp"

namespace CTagsPlugin
{

TagsHierarchy::TagsHierarchy(const IConfiguration& p_config)
	: config(p_config)
{}

TagHierarchy TagsHierarchy::get(const Tag& p_tag, const ITagsReader& p_tags)
{
	if (config.shouldCacheTags())
		return getCached(p_tag, p_tags);
	return TagHierarchy(p_tags, p_tag);
}

TagHierarchy TagsHierarchy::getCached(const Tag& p_tag, const ITagsReader& p_tags)
{
	if (hierarchy.find(p_tag.getName()) == hierarchy.end())
		hierarchy.emplace(p_tag.getName(), TagHierarchy(p_tags, p_tag));
	return hierarchy.at(p_tag.getName());
}

void TagsHierarchy::clear()
{
	hierarchy.clear();
}

}
