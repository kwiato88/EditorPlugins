#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "TagHierarchy.hpp"
#include "Tag.hpp"

#include <boost/range/algorithm/find.hpp>

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
		auto base = BasedTags(p_tags)(tag);
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

TagHierarchy::TagHierarchy(const ITagsReader& p_tags, const Tag& p_tag)
	: tag(p_tag), tags(p_tags), upHierarchy(), downHierarchy(), allTagsHierarchy()
{}

TagHierarchy::TagHierarchy(const TagHierarchy& p_other)
	: tag(p_other.tag), tags(p_other.tags),
	upHierarchy(p_other.upHierarchy), downHierarchy(p_other.downHierarchy),
	allTagsHierarchy(p_other.allTagsHierarchy)
{}

void TagHierarchy::parse()
{
	//allTagsHierarchy.update(tags);
	//upHierarchy = allTagsHierarchy.getUpHierarchy(tag);
	//downHierarchy = allTagsHierarchy.getDownHierarchy(tag);
	upHierarchy = TagHierarchyItem::build(tag, BasedTags(tags));
	downHierarchy = TagHierarchyItem::build(tag, DerivedTags(tags));
}

}
