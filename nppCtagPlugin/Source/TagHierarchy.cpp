#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "TagHierarchy.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{
namespace
{

class DerivedTags
{
public:
	DerivedTags(const ITagsReader& p_tags) : tags(p_tags) {}
	std::vector<TagHolder> operator()(const Tag& p_tag)
	{
		return tags.findTag([&](const Tag& t) { return t.isDerived(p_tag); });
	}
private:
	const ITagsReader& tags;
};

class BasedTags
{
public:
	BasedTags(const ITagsReader& p_tags) : tags(p_tags) {}
	std::vector<TagHolder> operator()(const Tag& p_tag)
	{
		return p_tag.baseTags(tags);
	}
private:
	const ITagsReader& tags;
};

}

TagHierarchy::TagHierarchy(const ITagsReader& p_tags, const Tag& p_tag)
	: tag(p_tag), tags(p_tags), upHierarchy(), downHierarchy()
{}

TagHierarchy::TagHierarchy(const TagHierarchy& p_other)
	: tag(p_other.tag), tags(p_other.tags),
	upHierarchy(p_other.upHierarchy), downHierarchy(p_other.downHierarchy)
{}

void TagHierarchy::parse()
{
	upHierarchy = TagHierarchyItem::build(tag, BasedTags(tags));
	downHierarchy = TagHierarchyItem::build(tag, DerivedTags(tags));
}

}
