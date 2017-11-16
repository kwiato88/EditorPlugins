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
    if(p_lhs.name != p_rhs.name)
        return p_lhs.name < p_rhs.name;
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

CTagsNavigator::CTagsNavigator(
	std::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
    Navigator& p_navigator,
    SelectorFactory p_selectorFactory,
	std::unique_ptr<ITagHierarchySelector> p_hierSelector,
	std::shared_ptr<ITagsReader> p_tagsReader)
 : m_navigator(p_navigator),
   m_locationGetter(p_locationGetter),
   m_selectorFactory(p_selectorFactory),
   m_hierSelector(std::move(p_hierSelector)),
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
    l_currentlocation.filePath = m_locationGetter->getFile();
    l_currentlocation.lineNumber = m_locationGetter->getLine();
    l_currentlocation.columNumber = m_locationGetter->getColumn();

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
	auto items(std::move(findTag(m_tagsReader, [&](const Tag& t) { return t.isChild(p_parentTag); })));
	LOG_DEBUG << "Found " << items.size() << " children tags";
	return std::move(items);
}

TagHolder CTagsNavigator::selectTag(const std::vector<TagHolder>& p_tags)
{
	if (p_tags.empty())
		throw TagNotFoundException();
    int l_selectedTagIndex = ( p_tags.size() == 1 ? 0 : m_selectorFactory()->selectTag(p_tags) );
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
	TagHierarchy hier(*m_tagsReader, selectTag(getComplexTags(p_currentTagName)));
	hier.parse();
	goTo(selectTag(hier));
}

} /* namespace CTagsPlugin */
