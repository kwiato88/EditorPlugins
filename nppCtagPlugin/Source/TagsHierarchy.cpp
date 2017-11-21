#include <boost/range/algorithm/find.hpp>

#include "TagsHierarchy.hpp"
#include "ITagsReader.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{

std::size_t TagNameHash::operator()(const TagHolder& p) const noexcept
{
	return std::hash<std::string>{}(p->getName());
}

TagHierarchyItem TagsHierarchy::getUpHierarchy(const Tag& p_tag)
{
	return TagHierarchyItem::build(p_tag,
		[&](const TagHolder& t)
	    {
			auto relatedTags = baseTags.find(t);
			if (relatedTags != baseTags.end())
				return relatedTags->second;
			return std::vector<TagHolder>();
	    });
}

TagHierarchyItem TagsHierarchy::getDownHierarchy(const Tag& p_tag)
{
	return TagHierarchyItem::build(p_tag,
		[&](const TagHolder& t)
		{
			auto relatedTags = derivedTags.find(t);
			if (relatedTags != derivedTags.end())
				return relatedTags->second;
			return std::vector<TagHolder>();
		});
}

void TagsHierarchy::update(const ITagsReader& p_tags)
{
	auto complexTags = p_tags.findTag([](const auto& t) { return t.isComplex(); });
	buildBase(complexTags, p_tags);
	buildDerived();
}

void TagsHierarchy::buildBase(const std::vector<TagHolder>& p_complexTags, const ITagsReader& p_tags)
{
	baseTags.clear();
	for (const auto& tag : p_complexTags)
	{
		auto base = tag->baseTags(p_tags);
		if(!base.empty())
			baseTags[tag] = base;
	}
}

void TagsHierarchy::buildDerived()
{
	derivedTags.clear();
	for (const auto& tagToBase : baseTags)
		updateDerived(tagToBase.first, tagToBase.second);
}

void TagsHierarchy::updateDerived(const TagHolder& p_derived, const std::vector<TagHolder>& p_base)
{
	for (const auto& base : p_base)
	{
		bool isAlreadyIncluded = boost::range::find(derivedTags[base], p_derived) != derivedTags[base].end();
		if(!isAlreadyIncluded)
			derivedTags[base].push_back(p_derived);
	}
}

}
