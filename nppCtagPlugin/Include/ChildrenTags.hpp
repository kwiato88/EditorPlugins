#pragma once

#include <memory>
#include <map>
#include <string>
#include <functional>
#include "Tag.hpp"
#include "TagHolder.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

class ChildrenTags
{
public:
	ChildrenTags(std::shared_ptr<ITagsReader> p_tags, bool p_useCache);

	std::vector<TagHolder> get(const Tag& p_parent);
	void clear();

private:
	bool hasChildren(const Tag& p_parent) const;
	std::vector<TagHolder> findChildrenTags(const Tag& p_parent);
	std::vector<TagHolder> getCachedChildrenTags(const Tag& p_parent);

	std::shared_ptr<ITagsReader> tags;
	std::map<std::string, std::vector<TagHolder>> childrenCache;
	std::function<std::vector<TagHolder>(const Tag&)> getImpl;
};

}
