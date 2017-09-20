#ifndef CTAGS_CONTROLLER_HPP
#define CTAGS_CONTROLLER_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <mutex>

#include "Navigator.hpp"
#include "ILocationSetter.hpp"
#include "ILocationGetter.hpp"
#include "IMessagePrinter.hpp"
#include "IPathGetter.hpp"
#include "ITagsSelector.hpp"
#include "ITextReader.hpp"
#include "ITagsReader.hpp"
#include "IConfiguration.hpp"
#include "IPathsSelector.hpp"
#include "CTagsNavigator.hpp"

namespace CTagsPlugin
{

class AsyncCall
{
public:
    AsyncCall(const std::function<void(void)>& p_call);
    bool operator()();

private:
    std::mutex m_lock;
    std::function<void(void)> m_call;
};

class CTagsController
{
public:
    CTagsController(
            boost::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
			boost::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
            boost::shared_ptr<Plugin::IMessagePrinter> p_messagePrinter,
            boost::shared_ptr<Plugin::IPathGetter> p_pathGetter,
            SelectorFactory p_selectorFactory,
            boost::shared_ptr<Plugin::ITextReader> p_textReader,
            boost::shared_ptr<Plugin::IPathsSelector> p_dirsSelector,
            boost::shared_ptr<Plugin::IPathsSelector> p_filesSelector,
            boost::shared_ptr<ITagsReader> p_tagsReader,
            IConfiguration& p_config);

    void next();
    void previous();
    void find();
    void tagInfo();
	void clear();
	void setTagFiles();
	void generateTagFile();

private:
    Navigator m_locationsNavigator;
	CTagsNavigator m_tagsNavigator;
	IConfiguration& m_config;

    boost::shared_ptr<Plugin::IMessagePrinter> m_messagePrinter;
    boost::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
    boost::shared_ptr<Plugin::IPathGetter> m_pathGetter;
    boost::shared_ptr<Plugin::ITextReader> m_textReader;
    boost::shared_ptr<Plugin::IPathsSelector> m_dirsSelector;
    boost::shared_ptr<Plugin::IPathsSelector> m_filesSelector;
    AsyncCall m_genTags;
    AsyncCall m_tagInfo;

    void showTagInfo();
	void handleReadTagsError();
    void gnerateTags();
    std::string getCurrentWord() const;
    std::vector<std::string> getSourceDirs() const;
    std::string getTargetFile() const;
};

} /* namespace CTagsPlugin */
#endif /* CTAGS_CONTROLLER_HPP_ */
