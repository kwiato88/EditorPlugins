#pragma once

#include <memory>
#include <map>
#include <string>
#include "Tag.hpp"
#include "TagHolder.hpp"
#include "ITagsReader.hpp"
#include "IConfiguration.hpp"

namespace CTagsPlugin
{

class ChildrenTags
{
public:
	ChildrenTags(std::shared_ptr<ITagsReader> p_tags, const IConfiguration& p_config);

	std::vector<TagHolder> get(const Tag& p_parent);
	void clear();

private:
	bool hasChildren(const Tag& p_parent) const;
	std::vector<TagHolder> findChildrenTags(const Tag& p_parent);
	std::vector<TagHolder> getCachedChildrenTags(const Tag& p_parent);

	std::shared_ptr<ITagsReader> tags;
	const IConfiguration& config;
	std::map<std::string, std::vector<TagHolder>> childrenCache;
};

}
