#ifndef CTAGS_CONTROLLER_HPP
#define CTAGS_CONTROLLER_HPP

#include <string>
#include <memory>
#include <functional>

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
#include "Transaction.hpp"
#include "MessageHandler.hpp"
#include "ExternalInterface/Commands.hpp"
#include "ExternalInterface/Results.hpp"

namespace CTagsPlugin
{

typedef std::function<TagMatcher()> GetTagSearchMatcher;

class CTagsController
{
public:
    CTagsController(
		std::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
		std::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
		std::shared_ptr<Plugin::IMessagePrinter> p_messagePrinter,
		std::shared_ptr<Plugin::IPathGetter> p_pathGetter,
        SelectorFactory p_selectorFactory,
		std::unique_ptr<ITagHierarchySelector> p_hierSelector,
		std::shared_ptr<Plugin::ITextReader> p_textReader,
		std::shared_ptr<Plugin::IPathsSelector> p_dirsSelector,
		std::shared_ptr<Plugin::IPathsSelector> p_filesSelector,
		std::shared_ptr<ITagsReader> p_tagsReader,
        IConfiguration& p_config,
		GetTagSearchMatcher p_tagSearchMatcherFactory);

    void next();
    void previous();
    void find();
    void tagInfo();
	void clear();
	void setTagFiles();
	void generateTags();
	void tagsSearch();
	void tagHierarchy();

	void handleTransaction(long p_id, Messaging::Transaction& p_trans);

private:
    Navigator m_locationsNavigator;
	CTagsNavigator m_tagsNavigator;
	IConfiguration& m_config;
	Messaging::Handlers m_handlers;

	std::shared_ptr<Plugin::IMessagePrinter> m_messagePrinter;
	std::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
	std::shared_ptr<Plugin::IPathGetter> m_pathGetter;
	std::shared_ptr<Plugin::ITextReader> m_textReader;
	std::shared_ptr<Plugin::IPathsSelector> m_dirsSelector;
	std::shared_ptr<Plugin::IPathsSelector> m_filesSelector;
	GetTagSearchMatcher m_tagSearchMatcherFactory;

    void showTagInfo(const std::string& p_tagName);
	void handleReadTagsError();
    void gnerateTags(std::string p_outFile,std::vector<std::string> p_sourceDirs);

	Result::Basic handleGenerateTags(const Command::GenerateTags&);
    Result::Basic handleSetTagFiles(const Command::SetTagFiles&);
    Result::TagFiles handleGetTagFiles(const Command::GetTagFiles&);

    std::string getCurrentWord() const;
    std::vector<std::string> getSourceDirs() const;
    std::string getTargetFile() const;
};

} /* namespace CTagsPlugin */
#endif /* CTAGS_CONTROLLER_HPP_ */
