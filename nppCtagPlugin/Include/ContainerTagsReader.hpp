#pragma once

#include "ITagsReader.hpp"

namespace CTagsPlugin
{

class ContainerTagsReader : public ITagsReader
{
public:
	ContainerTagsReader(const std::vector<TagHolder>& p_tags);
	
	std::vector<TagHolder> findTag(const std::string& p_tagName) const;
	std::vector<TagHolder> findTag(TagMatcher p_matcher) const;
private:
	std::vector<TagHolder> tags;
};

}
