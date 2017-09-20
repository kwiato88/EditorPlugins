#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "TagHierarchy.hpp"

namespace CTagsPlugin
{
namespace
{
std::vector<TagHolder> getDerivedTags(const ITagsReader& p_tags, const Tag& p_tag)
{
	return p_tags.findTag([&](const Tag& t) { return t.isDerived(p_tag); });
}
std::vector<TagHolder> getBaseTags(const ITagsReader& p_tags, const Tag& p_tag)
{
	return p_tags.findTag([&](const Tag& t) { return p_tag.isDerived(t); });
}
}

TagHierarchyItem::TagHierarchyItem(const Tag& p_tag,
	const std::function<std::vector<TagHolder>(const ITagsReader&, const Tag&)>& p_relaredTagsParser)
	: tag(p_tag), relatedTags(), relatedTagsParser(p_relaredTagsParser)
{}

TagHierarchyItem::TagHierarchyItem(const TagHierarchyItem& p_other)
	: tag(p_other.tag), relatedTags(p_other.relatedTags), relatedTagsParser(p_other.relatedTagsParser)
{}

void TagHierarchyItem::parse(const ITagsReader& p_tags)
{
	boost::range::copy(
		relatedTagsParser(p_tags, tag)
		| boost::adaptors::transformed([&](const auto& t) { return TagHierarchyItem(t, relatedTagsParser); }),
		std::back_inserter(relatedTags));
	for (auto& relatedTag : relatedTags)
		relatedTag.parse(p_tags);
}

std::vector<TagHolder> TagHierarchyItem::getRelatedTags() const
{
	std::vector<TagHolder> tags;
	boost::range::copy(
		relatedTags | boost::adaptors::transformed([&](const auto& h) { return h.tag; }),
		std::back_inserter(tags));
	return tags;
}

TagHierarchy::TagHierarchy(const ITagsReader& p_tags, const Tag& p_tag)
	: tags(p_tags), upHierarchy(p_tag, &getBaseTags), downHierarchy(p_tag, &getDerivedTags)
{}

TagHierarchy::TagHierarchy(const TagHierarchy& p_other)
	: tags(p_other.tags), upHierarchy(p_other.upHierarchy), downHierarchy(p_other.downHierarchy)
{}

void TagHierarchy::parse()
{
	upHierarchy.parse(tags);
	downHierarchy.parse(tags);
}

}
