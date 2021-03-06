#ifndef CTAGSNAVIGATOR_HPP
#define CTAGSNAVIGATOR_HPP

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <ostream>

#include "Navigator.hpp"
#include "ITagsReader.hpp"
#include "IConfiguration.hpp"
#include "ITagsSelector.hpp"
#include "ITagHierarchySelector.hpp"
#include "Editor.hpp"
#include "ChildrenTags.hpp"
#include "TagsHierarchy.hpp"
#include "ClassDiagram.hpp"

namespace CTagsPlugin
{

class CTagsNavigator
{
public:
    CTagsNavigator(Navigator& p_navigator,
				   Plugin::Editor& p_editor,
				   std::unique_ptr<ITagsSelector> p_tagsSelector,
		           std::unique_ptr<ITagHierarchySelector> p_hierSelector,
				   std::shared_ptr<ITagsReader> p_tagsReader,
                   const IConfiguration& p_config);

    /*
     * @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
     */
    void goTo(const std::string& p_tagName);
	Location tagLocation(const std::string& p_tagName);
    /*
     * @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
     */
    void goToChildTag(const std::string& p_parentTagName);
	Location childTagLocation(const std::string& p_parentTagName);
	/*
	* @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
	*/
	void goToTag(TagMatcher p_matcher);
	Location tagLocation(TagMatcher p_matcher);
	/*
	* @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
	*/
	void goToTagInHierarchy(const std::string& p_currentTagName);
	Location tagInHierarchyLocation(const std::string& p_currentTagName);

	void onTagsLoaded();

	void exportClassDiagram(std::ostream& p_out, TagMatcher p_tagsToInclude);
	void exportClassDiagram(std::ostream& p_out, const std::string& p_tagName);

private:
    Location getCurrentLocation() const;
    TagHolder selectTag(const std::vector<TagHolder>& p_tags);
	TagHolder selectTag(const TagHierarchy& p_hier);
    std::vector<TagHolder> getChildrenTags(const TagHolder& p_parentTag);
    std::vector<TagHolder> getComplexTags(const std::string& p_complexTagName);
    void goTo(const TagHolder& p_tag);
	void appendClassToDiagram(const Tag& p_tag, ClassDiagram& p_diagram);

    Navigator& m_navigator;
	Plugin::Editor& m_editor;
	const IConfiguration& m_config;
    std::unique_ptr<ITagsSelector> m_tagsSelector;
	std::unique_ptr<ITagHierarchySelector> m_hierSelector;
	ChildrenTags m_childrenTags;
	TagsHierarchy m_tagsHierarchy;
	std::shared_ptr<ITagsReader> m_tagsReader;
};

} /* namespace CTagsPlugin */
#endif /* CTAGSNAVIGATOR_HPP */
