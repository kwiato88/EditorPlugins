#include <iterator>
#include <algorithm>
#include <functional>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/lexical_cast.hpp>

#include "CTagsNavigator.hpp"
#include "CppIsTagWithAtt.hpp"
#include "TagNotFoundException.hpp"
#include "ContainerTagsReader.hpp"
#include "CtagsMeasTags.hpp"
#include "Samples.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{
namespace
{
Location convertToLocation(const Tag& p_tag)
{
	auto addr = p_tag.getAddr();
	Location tagLocation = {};
	tagLocation.filePath = addr.filePath();
	tagLocation.columNumber = addr.cloNoInFile();
	tagLocation.lineNumber = addr.lineNoInFile();
	LOG_DEBUG << "Found tag in location. Pah: " << tagLocation.filePath << ", line: " << tagLocation.lineNumber
		<< ", col: " << tagLocation.columNumber;

	return tagLocation;
}

std::string getFileName(const std::string& p_path)
{
	size_t l_pos = p_path.find_last_of('\\');
	if(l_pos != std::string::npos)
		return p_path.substr(l_pos + 1);
	return p_path;
}

bool compareTags(const Tag& p_lhs, const Tag& p_rhs)
{
    if(p_lhs.getName() != p_rhs.getName())
        return p_lhs.getName() < p_rhs.getName();
    return getFileName(p_lhs.getAddr().filePath()) < getFileName(p_rhs.getAddr().filePath());
};

template <typename T>
std::vector<TagHolder> findTag(std::shared_ptr<ITagsReader> p_tagsReader, const T& p_toFind)
{
    std::vector<TagHolder> l_foundTags = p_tagsReader->findTag(p_toFind);
    if(l_foundTags.empty())
        throw TagNotFoundException();
    boost::range::sort(l_foundTags, &compareTags);

    return l_foundTags;
}
}

class ClassDiagramBuilder
{
public:
	ClassDiagramBuilder(
		const ClassDiagramConfig& p_config, ClassDiagram& p_diagram,
		const Tag& p_tag, const std::vector<TagHolder>& p_children,
		const TagHierarchy& p_hier)
		: ClassDiagramBuilder(p_config, p_diagram, p_tag, p_children, p_hier.upHierarchy, p_hier.downHierarchy)
	{}

	ClassDiagramBuilder(
		const ClassDiagramConfig& p_config, ClassDiagram& p_diagram,
		const Tag& p_tag, const std::vector<TagHolder>& p_children,
		const TagHierarchyItem& p_upHier, const TagHierarchyItem& p_downHier)
		: config(p_config), diagram(p_diagram), tag(p_tag), tagDesc(p_tag), children(p_children),
		upHier(p_upHier), downHier(p_downHier)
	{}

	void append()
	{
		if (config.includeMembers)
			appendMembers();
		if (config.inheritedTags == ClassDiagramConfig::Hierarchy::direct)
			appendDirectInherited();
		if (config.inheritedTags == ClassDiagramConfig::Hierarchy::full)
			appendInheritedHierarchy();
		if (config.derivedTags == ClassDiagramConfig::Hierarchy::direct)
			appendDirectDerived();
		if (config.derivedTags == ClassDiagramConfig::Hierarchy::full)
			appendDerivedHierarchy();

		diagram.add(tagDesc);
	}

private:
	void appendMembers()
	{
		for (const Tag& child : children)
			tagDesc.addMember(child);
	}
	ClassDiagramConfig fullInheritedHierConfigOnly() const
	{
		ClassDiagramConfig upHierOnly;
		upHierOnly.includeMembers = false;
		upHierOnly.inheritedTags = ClassDiagramConfig::Hierarchy::full;
		upHierOnly.derivedTags = ClassDiagramConfig::Hierarchy::none;
		return upHierOnly;
	}
	void appendInheritedHierarchy()
	{
		for (const auto& base : upHier.children)
		{
			tagDesc.addBase(base.value);

			ClassDiagramBuilder baseBuilder(fullInheritedHierConfigOnly(), diagram,
				base.value, {}, base, TagHierarchyItem());
			baseBuilder.append();
		}
	}
	void appendDirectInherited()
	{
		for (const Tag& base : upHier.childrenValues())
			tagDesc.addBase(base);
	}
	ClassDiagramConfig fullDerivedHierConfigOnly() const
	{
		ClassDiagramConfig downHierOnly;
		downHierOnly.includeMembers = false;
		downHierOnly.inheritedTags = ClassDiagramConfig::Hierarchy::none;
		downHierOnly.derivedTags = ClassDiagramConfig::Hierarchy::full;
		return downHierOnly;
	}
	void appendDerivedHierarchy()
	{
		for (const auto& derived : downHier.children)
		{
			ClassDiagram::Class derivedDesc(derived.value);
			derivedDesc.addBase(tag);
			diagram.add(derivedDesc);

			ClassDiagramBuilder baseBuilder(fullDerivedHierConfigOnly(), diagram,
				derived.value, {}, TagHierarchyItem(), derived);
			baseBuilder.append();
		}
	}
	void appendDirectDerived()
	{
		for (const Tag& derived : downHier.childrenValues())
		{
			ClassDiagram::Class derivedDesc(derived);
			derivedDesc.addBase(tag);
			diagram.add(derivedDesc);
		}
	}

private:
	ClassDiagramConfig config;
	ClassDiagram& diagram;

	const Tag& tag;
	ClassDiagram::Class tagDesc;
	std::vector<TagHolder> children;
	const TagHierarchyItem& upHier;
	const TagHierarchyItem& downHier;
};

CTagsNavigator::CTagsNavigator(
	Navigator& p_navigator,
	Plugin::Editor& p_editor,
	std::unique_ptr<ITagsSelector> p_tagsSelector,
	std::unique_ptr<ITagHierarchySelector> p_hierSelector,
	std::shared_ptr<ITagsReader> p_tagsReader,
	const IConfiguration& p_config)
 : m_navigator(p_navigator),
   m_editor(p_editor),
   m_config(p_config),
   m_tagsSelector(std::move(p_tagsSelector)),
   m_hierSelector(std::move(p_hierSelector)),
   m_childrenTags(p_tagsReader, p_config),
   m_tagsHierarchy(p_config),
   m_tagsReader(p_tagsReader)
{
}

void CTagsNavigator::goTo(const std::string& p_tagName)
{
	LOG_INFO << "go to tag by name: " << p_tagName;
    goTo(selectTag(findTag(m_tagsReader, p_tagName)));
}

void CTagsNavigator::goToChildTag(const std::string& p_parentTagName)
{
	LOG_INFO << "go to child tag. Parent tag name: " << p_parentTagName;
    goTo(selectTag(getChildrenTags(selectTag(getComplexTags(p_parentTagName)))));
}

Location CTagsNavigator::getCurrentLocation() const
{
    Location l_currentlocation;
    l_currentlocation.filePath = m_editor.getFile();
    l_currentlocation.lineNumber = m_editor.getLine();
    l_currentlocation.columNumber = m_editor.getColumn();

    return l_currentlocation;
}

std::vector<TagHolder> CTagsNavigator::getComplexTags(const std::string& p_complexTagName)
{
    std::vector<TagHolder> items;
    boost::range::copy(
        findTag(m_tagsReader, p_complexTagName)
        | boost::adaptors::filtered([&](const Tag& t){ return t.isComplex(); }),
        std::back_inserter(items));
	LOG_DEBUG << "Found " << items.size() << " complex tags";

    return std::move(items);
}

std::vector<TagHolder> CTagsNavigator::getChildrenTags(const TagHolder& p_parentTag)
{
	auto items(std::move(m_childrenTags.get(p_parentTag)));
	if (items.empty())
		throw TagNotFoundException();
	boost::range::sort(items, &compareTags);
	LOG_DEBUG << "Found " << items.size() << " children tags";
	return std::move(items);
}

TagHolder CTagsNavigator::selectTag(const std::vector<TagHolder>& p_tags)
{
	if (p_tags.empty())
		throw TagNotFoundException();
    int l_selectedTagIndex = ( p_tags.size() == 1 ? 0 : m_tagsSelector->selectTag(p_tags) );
    if(l_selectedTagIndex == -1)
        throw TagNotFoundException();
	LOG_DEBUG << "Selected tag with index " << l_selectedTagIndex;
    return p_tags.at(l_selectedTagIndex);
}

void CTagsNavigator::goTo(const TagHolder& p_tag)
{
    m_navigator.addLocation(getCurrentLocation());
    m_navigator.addLocation(convertToLocation(p_tag));
    m_navigator.goToCurrentLocation();
}

void CTagsNavigator::goToTag(TagMatcher p_matcher)
{
	LOG_INFO << "go to tag by matcher";
	goTo(selectTag(m_tagsReader->findTag(p_matcher)));
}

TagHolder CTagsNavigator::selectTag(const TagHierarchy& p_hier)
{
	auto tag = m_hierSelector->select(p_hier);
	if(tag == boost::none)
		throw TagNotFoundException();
	LOG_DEBUG << "Tag from hierarchy selected";
	return tag.get();
}

void CTagsNavigator::goToTagInHierarchy(const std::string& p_currentTagName)
{
	LOG_INFO << "go to tag in hierarchy. Hierarchy for tag with name: " << p_currentTagName;

	ContainerTagsReader complexTags(m_tagsReader->findTag([&](const auto& t) { return t.isComplex(); }));
	goTo(selectTag(m_tagsHierarchy.get(selectTag(getComplexTags(p_currentTagName)), complexTags)));
}

void CTagsNavigator::onTagsLoaded()
{
	m_childrenTags.clear();
	m_tagsHierarchy.clear();
}

ClassDiagram::Class CTagsNavigator::classInDiagram(const Tag& p_tag)
{
	LOG_INFO << "export class " << p_tag.getName() << " to class diagram";
	Meas::ExecutionTimeSample<GenerateClassInDiagramTime> meas;

	ClassDiagram::Class tag(p_tag);
	for (const Tag& base : p_tag.baseTags(*m_tagsReader))
		tag.addBase(base);
	for (const Tag& child : m_childrenTags.get(p_tag))
		tag.addMember(child);
	return tag;
}

void CTagsNavigator::exportClassDiagram(std::ostream& p_out, TagMatcher p_tagsToInclude)
{
	LOG_INFO << "export class diagram";
	Meas::ExecutionTimeSample<GenerateClassDiagramTime> meas;

	ClassDiagram diagram(p_out);
	for (const Tag& tag : m_tagsReader->findTag(p_tagsToInclude))
		diagram.add(classInDiagram(tag));
}

void CTagsNavigator::exportClassDiagram(std::ostream& p_out, const std::string& p_tagName)
{
	LOG_INFO << "export class diagram for tag with name: " << p_tagName;
	Meas::ExecutionTimeSample<GenerateClassInDiagramTime> meas;

	ClassDiagram diagram(p_out);
	const auto tag = selectTag(getComplexTags(p_tagName));
	const auto hier = m_tagsHierarchy.get(tag, *m_tagsReader);
	ClassDiagramBuilder builder(m_config.getClassDiagramConfig(), diagram, tag, m_childrenTags.get(tag), hier);
	builder.append();
}

} /* namespace CTagsPlugin */
