#include <algorithm>
#include <iterator>

#include "ContainerTagsReader.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{

ContainerTagsReader::ContainerTagsReader(const std::vector<TagHolder>& p_tags)
	: tags(p_tags)
{}

std::vector<TagHolder> ContainerTagsReader::findTag(const std::string& p_tagName) const
{
	std::vector<TagHolder> found;
	std::copy_if(tags.begin(), tags.end(), std::back_inserter(found),
		[&](const auto& t) { return t->isTagWithName(p_tagName); });
	return found;
}

std::vector<TagHolder> ContainerTagsReader::findTag(TagMatcher p_matcher) const
{
	std::vector<TagHolder> found;
	std::copy_if(tags.begin(), tags.end(), std::back_inserter(found), p_matcher);
	return found;
}

}
