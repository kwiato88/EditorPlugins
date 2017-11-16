#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>
#include <iterator>

#include "TagHierarchy.hpp"
#include "ITagsReader.hpp"

namespace CTagsPlugin
{

using namespace ::testing;

class HierTestTag : public Tag
{
public:
	HierTestTag() : Tag() {}
	HierTestTag(const HierTestTag& p_other) : Tag(p_other), baseTagsNames(p_other.baseTagsNames) {}
	HierTestTag(const std::string& p_name) : Tag()
	{
		name = p_name;
	}
	void addBase(const Tag& p_tag)
	{
		baseTagsNames.push_back(p_tag.getName());
	}
	bool isDerived(const Tag& p_base) const override
	{
		return std::find(baseTagsNames.begin(), baseTagsNames.end(), p_base.getName()) != baseTagsNames.end();
	}

	Tag* clone() const override
	{
		return new HierTestTag(*this);
	}
	void assign(const Tag& p_tag) override
	{
		Tag::assign(p_tag);
		if (typeid(HierTestTag) == typeid(p_tag))
		{
			const HierTestTag& other = static_cast<const HierTestTag&>(p_tag);
			baseTagsNames = other.baseTagsNames;
		}
	}
	void describe(std::ostream& p_out) const override
	{
		p_out << "HierTestTag [name: " << name << ", baseClasses: ";
		for (const auto& name : baseTagsNames)
			p_out << name << ", ";
		p_out << "]";
	}

protected:
	bool isEqual(const Tag& p_tag) const override
	{
		const HierTestTag& tag = static_cast<const HierTestTag&>(p_tag);
		return Tag::isEqual(p_tag) && (baseTagsNames == tag.baseTagsNames);
	}
private:
	std::vector<std::string> baseTagsNames;
};

class TagsReaderFake : public ITagsReader
{
public:
	TagsReaderFake(const std::vector<TagHolder>& p_tags)
		: m_tags(p_tags)
	{}
	std::vector<TagHolder> findTag(const std::string& p_tagName) const override
	{
		return{};
	}
	virtual std::vector<TagHolder> findTag(TagMatcher p_matcher) const override
	{
		std::vector<TagHolder> found;
		std::copy_if(m_tags.begin(), m_tags.end(), std::back_inserter(found),
			[=](const Tag& t) { return p_matcher(t); });
		return found;
	}
private:
	std::vector<TagHolder> m_tags;
};

struct TagHierarchyTS : public Test
{
	TagHierarchy parseHier(TagHolder p_tag, const std::initializer_list<TagHolder>& p_tags)
	{
		TagsReaderFake tags(p_tags);
		TagHierarchy hier(tags, p_tag);
		hier.parse();
		return hier;
	}
	TagHierarchyItem findTagHier(const TagHolder& p_tagToFind, const std::vector<TagHierarchyItem>& p_hier)
	{
		auto found = std::find_if(p_hier.begin(), p_hier.end(), [&](const auto& h) { return h.value == p_tagToFind; });
		EXPECT_TRUE(found != p_hier.end());
		return *found;
	}
};

TEST_F(TagHierarchyTS, shoudlInitializeWithGivenTag)
{
	HierTestTag tag("Tag");

	auto hier = parseHier(tag, {});

	ASSERT_EQ(TagHolder(tag), hier.downHierarchy.value);
	ASSERT_EQ(TagHolder(tag), hier.upHierarchy.value);
}

TEST_F(TagHierarchyTS, shouldFindNoTagsWhenNoTagsGiven)
{
	HierTestTag baseTag("TagA");

	HierTestTag derivedTag("TagB");
	derivedTag.addBase(baseTag);

	auto hier = parseHier(derivedTag, {});

	ASSERT_TRUE(hier.downHierarchy.children.empty());
	ASSERT_TRUE(hier.upHierarchy.children.empty());
}

TEST_F(TagHierarchyTS, shouldFindNoTagsWhenNoBaseTags)
{
	HierTestTag tagA("TagA"), tagB("TagB");

	auto hier = parseHier(tagA, { tagA, tagB });

	ASSERT_TRUE(hier.downHierarchy.children.empty());
	ASSERT_TRUE(hier.upHierarchy.children.empty());
}

struct TagsUpHierarchyTS : public TagHierarchyTS
{};

TEST_F(TagsUpHierarchyTS, shouldFindBaseTag)
{
	HierTestTag baseTag("base"), derivedTag("derived"), otherTag("other");
	derivedTag.addBase(baseTag);

	auto hier = parseHier(derivedTag, { otherTag, baseTag });

	ASSERT_THAT(
		hier.upHierarchy.childrenValues(),
		ElementsAre(TagHolder(baseTag)));
	ASSERT_TRUE(findTagHier(baseTag, hier.upHierarchy.children).children.empty());
}

TEST_F(TagsUpHierarchyTS, shouldNotFindBaseTagNotIncludedInTagReader)
{
	HierTestTag baseTag("base"), derivedTag("derived"), otherTag("other");
	derivedTag.addBase(baseTag);

	auto hier = parseHier(derivedTag, { otherTag });

	ASSERT_TRUE(hier.upHierarchy.children.empty());
}

TEST_F(TagsUpHierarchyTS, shouldFindManyBaseTags)
{
	HierTestTag baseTag("base"), otherBaseTag("secondBaseTag"), derivedTag("derived"), otherTag("other");
	derivedTag.addBase(baseTag);
	derivedTag.addBase(otherBaseTag);

	auto hier = parseHier(derivedTag, { baseTag, otherTag, otherBaseTag });

	ASSERT_THAT(
		hier.upHierarchy.childrenValues(),
		ElementsAre(TagHolder(baseTag), TagHolder(otherBaseTag)));

	ASSERT_TRUE(findTagHier(baseTag, hier.upHierarchy.children).children.empty());
	ASSERT_TRUE(findTagHier(otherBaseTag, hier.upHierarchy.children).children.empty());
}

TEST_F(TagsUpHierarchyTS, shouldFindSecondLevelBaseTag)
{
	HierTestTag tag("tag"), parrentTag("parrent"), grandParentTag("grandparrent");
	parrentTag.addBase(grandParentTag);
	tag.addBase(parrentTag);

	auto hier = parseHier(tag, { tag, grandParentTag, parrentTag });

	ASSERT_THAT(
		hier.upHierarchy.childrenValues(),
		ElementsAre(parrentTag));
	ASSERT_THAT(
		findTagHier(parrentTag, hier.upHierarchy.children).childrenValues(),
		ElementsAre(grandParentTag));
}

TEST_F(TagsUpHierarchyTS, shouldParseTwoLevelsOfInheritance)
{
	HierTestTag tag("tag");
	HierTestTag firstParrent("parr 1"), secondParrent("parr 2");
	HierTestTag grandparrentFromSecondParrent("grand");
	secondParrent.addBase(grandparrentFromSecondParrent);
	tag.addBase(firstParrent);
	tag.addBase(secondParrent);

	auto hier = parseHier(tag, { grandparrentFromSecondParrent, firstParrent, tag, secondParrent });

	ASSERT_THAT(
		hier.upHierarchy.childrenValues(),
		ElementsAre(firstParrent, secondParrent));
	
	ASSERT_TRUE(findTagHier(firstParrent, hier.upHierarchy.children).children.empty());
	
	auto secondParrentUpHier = findTagHier(secondParrent, hier.upHierarchy.children);
	ASSERT_THAT(
		secondParrentUpHier.childrenValues(),
		ElementsAre(grandparrentFromSecondParrent));
	
	auto grandParrentUpHier = findTagHier(grandparrentFromSecondParrent, secondParrentUpHier.children);
	ASSERT_TRUE(grandParrentUpHier.children.empty());
}

struct TagsDownHierarchyTS : public TagHierarchyTS
{};

TEST_F(TagsDownHierarchyTS, shoudFindDerivedTag)
{
	HierTestTag tag("tag"), derivedTag("derived"), otherTag("other");
	derivedTag.addBase(tag);

	auto hier = parseHier(tag, { otherTag, derivedTag, tag });

	ASSERT_THAT(
		hier.downHierarchy.childrenValues(),
		ElementsAre(derivedTag));
	ASSERT_TRUE(findTagHier(derivedTag, hier.downHierarchy.children).children.empty());
}

TEST_F(TagsDownHierarchyTS, shouldNotFindDerivedTagNotIncludedInTagReader)
{
	HierTestTag tag("tag"), derivedTag("derived"), otherTag("other");
	derivedTag.addBase(tag);
	
	auto hier = parseHier(tag, { tag, otherTag });

	ASSERT_TRUE(hier.downHierarchy.children.empty());
}

TEST_F(TagsDownHierarchyTS, shouldFindManyDerivedTags)
{
	HierTestTag tag("tag"), firstDerived("der 1"), secondDerived("der 2"), other("other");
	firstDerived.addBase(tag);
	secondDerived.addBase(tag);

	auto hier = parseHier(tag, { tag, firstDerived, other, secondDerived });

	ASSERT_THAT(
		hier.downHierarchy.childrenValues(),
		ElementsAre(firstDerived, secondDerived));
	
	ASSERT_TRUE(findTagHier(firstDerived, hier.downHierarchy.children).children.empty());
	ASSERT_TRUE(findTagHier(secondDerived, hier.downHierarchy.children).children.empty());
}

TEST_F(TagsDownHierarchyTS, shouldFindSecondLevelDerivedTag)
{
	HierTestTag tag("tag"), child("child"), grandchild("grand");
	child.addBase(tag);
	grandchild.addBase(child);

	auto hier = parseHier(tag, { child, grandchild });

	ASSERT_THAT(
		hier.downHierarchy.childrenValues(),
		ElementsAre(child));
	
	auto childDownHier = findTagHier(child, hier.downHierarchy.children);
	ASSERT_THAT(
		childDownHier.childrenValues(),
		ElementsAre(grandchild));
	
	ASSERT_TRUE(findTagHier(grandchild, childDownHier.children).children.empty());
}

TEST_F(TagsDownHierarchyTS, shouldParseTwoLevelsOfInheritance)
{
	HierTestTag tag("tag"), firstChild("der 1"), secondChild("der 2"), other("other");
	HierTestTag grandChildfromFirstChild("grand");
	firstChild.addBase(tag);
	secondChild.addBase(tag);
	grandChildfromFirstChild.addBase(firstChild);

	auto hier = parseHier(tag, { tag, grandChildfromFirstChild, firstChild, other, secondChild });

	ASSERT_THAT(
		hier.downHierarchy.childrenValues(),
		ElementsAre(firstChild, secondChild));
	
	auto firstChildDowHier = findTagHier(firstChild, hier.downHierarchy.children);
	ASSERT_THAT(
		firstChildDowHier.childrenValues(),
		ElementsAre(grandChildfromFirstChild));
	auto grandChildHier = findTagHier(grandChildfromFirstChild, firstChildDowHier.children);
	ASSERT_TRUE(grandChildHier.children.empty());

	auto secondChildHier = findTagHier(secondChild, hier.downHierarchy.children);
	ASSERT_TRUE(secondChildHier.children.empty());
}

}
