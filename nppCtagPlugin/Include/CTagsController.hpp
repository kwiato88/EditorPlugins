#ifndef CTAGS_CONTROLLER_HPP
#define CTAGS_CONTROLLER_HPP

#include <string>
#include <memory>
#include <functional>

#include "Navigator.hpp"
#include "Editor.hpp"
#include "UI.hpp"
#include "UIFileSystem.hpp"
#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "IConfiguration.hpp"
#include "CTagsNavigator.hpp"
#include "Transaction.hpp"
#include "MessageHandler.hpp"
#include "Commands.hpp"
#include "Results.hpp"
#include "Command.hpp"

namespace CTagsPlugin
{

typedef std::function<TagMatcher()> GetTagSearchMatcher;

class CTagsController
{
public:
    CTagsController(
		Plugin::Editor& p_editor,
		Plugin::UI& p_ui,
		Plugin::UIFileSystem& p_files,
		std::unique_ptr<ITagsSelector> p_tagsSelector,
		std::unique_ptr<ITagHierarchySelector> p_hierSelector,
		std::shared_ptr<ITagsReader> p_tagsReader,
        IConfiguration& p_config,
		GetTagSearchMatcher p_tagSearchMatcherFactory,
		Plugin::CommandFactory p_cmdFactory);

    void next();
    void previous();
    void find();
    void tagInfo();
	void clear();
	void setTagFiles();
	void generateTags();
	void tagsSearch();
	void tagHierarchy();
	void tagClassDiagram();
	void classDiagram();

	void handleTransaction(long p_id, Messaging::Transaction& p_trans);
	void onTagsLoaded();

private:
	Plugin::Editor& m_editor;
	Plugin::UI& m_ui;
	Plugin::UIFileSystem& m_files;
	IConfiguration& m_config;

	GetTagSearchMatcher m_tagSearchMatcherFactory;
    Navigator m_locationsNavigator;
	CTagsNavigator m_tagsNavigator;
	Plugin::CommandFactory m_cmdFactory;
	Messaging::HandlersWithDefaultCodec m_handlers;

    void showTagInfo(const std::string& p_tagName);
    void gnerateTags(std::string p_outFile,std::vector<std::string> p_sourceDirs);

	Result::Basic handleGenerateTags(const Command::GenerateTags&);
    Result::Basic handleSetTagFiles(const Command::SetTagFiles&);
    Result::TagFiles handleGetTagFiles(const Command::GetTagFiles&);
	Result::Test handleTestCommand(const Command::Test&);

    std::string getCurrentWord() const;
    std::vector<std::string> getSourceDirs() const;
    std::string getTargetFile() const;
	std::string getPlantUmlScriptPath() const;
	std::string getTagNamePattern() const;
};

} /* namespace CTagsPlugin */
#endif /* CTAGS_CONTROLLER_HPP_ */
