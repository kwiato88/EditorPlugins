#ifndef CTAGSNAVIGATOR_HPP
#define CTAGSNAVIGATOR_HPP

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "Navigator.hpp"
#include "ILocationGetter.hpp"
#include "ITagsReader.hpp"
#include "ITagsSelector.hpp"

namespace CTagsPlugin
{

typedef boost::function<boost::shared_ptr<ITagsSelector>()> SelectorFactory;

class CTagsNavigator
{
public:
    CTagsNavigator(boost::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
                   Navigator& p_navigator,
                   SelectorFactory p_selectorFactory,
                   boost::shared_ptr<ITagsReader> p_tagsReader);

    /*
     * @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
     */
    void goTo(const std::string& p_tagName);
    /*
     * @throws: TagNotFoundException, OpenFileException, TagsReaderException, LocationSetterException
     */
    void goToChildTag(const std::string& p_parentTagName);

private:
    Location getCurrentLocation() const;
    TagHolder selectTag(const std::vector<TagHolder>& p_tags);
    std::vector<TagHolder> getChildrenTags(const TagHolder& p_parentTag);
    std::vector<TagHolder> getComplexTags(const std::string& p_complexTagName);
    void goTo(const TagHolder& p_tag);

    Navigator& m_navigator;
    boost::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
    SelectorFactory m_selectorFactory;
    boost::shared_ptr<ITagsReader> m_tagsReader;
};

} /* namespace CTagsPlugin */
#endif /* CTAGSNAVIGATOR_HPP */
