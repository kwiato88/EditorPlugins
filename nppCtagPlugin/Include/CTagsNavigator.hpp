#ifndef CTAGSNAVIGATOR_HPP
#define CTAGSNAVIGATOR_HPP

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "Navigator.hpp"
#include "ITagsReader.hpp"
#include "ITagsSelector.hpp"
#include "TagHierarchy.hpp"
#include "ITagHierarchySelector.hpp"
#include "Editor.hpp"

namespace CTagsPlugin
{

typedef std::function<std::shared_ptr<ITagsSelector>()> SelectorFactory;

class CTagsNavigator
{
public:
    CTagsNavigator(Navigator& p_navigator,
				   Plugin::Editor& p_editor,
                   SelectorFactory p_selectorFactory,
		           std::unique_ptr<ITagHierarchySelector> p_hierSelector,
				   std::shared_ptr<ITagsReader> p_tagsReader);

    /*
     * @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
     */
    void goTo(const std::string& p_tagName);
    /*
     * @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
     */
    void goToChildTag(const std::string& p_parentTagName);
	/*
	* @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
	*/
	void goToTag(TagMatcher p_matcher);
	/*
	* @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
	*/
	void goToTagInHierarchy(const std::string& p_currentTagName);

private:
    Location getCurrentLocation() const;
    TagHolder selectTag(const std::vector<TagHolder>& p_tags);
	TagHolder selectTag(const TagHierarchy& p_hier);
    std::vector<TagHolder> getChildrenTags(const TagHolder& p_parentTag);
    std::vector<TagHolder> getComplexTags(const std::string& p_complexTagName);
    void goTo(const TagHolder& p_tag);

    Navigator& m_navigator;
	Plugin::Editor& m_editor;
    SelectorFactory m_selectorFactory;
	std::unique_ptr<ITagHierarchySelector> m_hierSelector;
	std::shared_ptr<ITagsReader> m_tagsReader;
};

} /* namespace CTagsPlugin */
#endif /* CTAGSNAVIGATOR_HPP */
