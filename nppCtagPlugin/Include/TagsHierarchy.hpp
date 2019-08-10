#pragma once

#include <map>

#include "Tag.hpp"
#include "TagHierarchy.hpp"
#include "IConfiguration.hpp"

namespace CTagsPlugin
{

class ITagsReader;

class TagsHierarchy
{
public:
	TagsHierarchy(const IConfiguration& p_config);

	TagHierarchy get(const Tag& p_tag, const ITagsReader& p_tags);
	void clear();

private:
	TagHierarchy getCached(const Tag& p_tag, const ITagsReader& p_tags);
	TagHierarchy buildHierarchy(const Tag& p_tag, const ITagsReader& p_tags);
	bool hasHierarchy(const Tag& p_tag) const;

	const IConfiguration& config;
	std::map<std::string, TagHierarchy> hierarchy;
};

}
