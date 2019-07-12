#pragma once

#include <memory>
#include <map>
#include <string>
#include "Tag.hpp"
#include "TagHolder.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

class ChildrenTags
{
public:
	ChildrenTags(std::shared_ptr<ITagsReader> p_tags);

	std::vector<TagHolder> get(const Tag& p_parent);
	void clear();

private:
	bool hasChildren(const Tag& p_parent) const;

	std::shared_ptr<ITagsReader> tags;
	std::map<std::string, std::vector<TagHolder>> childrenCache;
};

}
