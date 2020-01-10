#include <memory>
#include <fstream>

#include "Plugin.hpp"

#include "ReadTagsProxy.hpp"
#include "TagFileReader.hpp"
#include "CachedTagsReader.hpp"
#include "MultipleTagFilesReader.hpp"
#include "FileScopedTagFilteringReader.hpp"
#include "CTagsController.hpp"
#include "ListViewTagsSelector.hpp"
#include "GridViewTagsSelector.hpp"
#include "LazyInitializedTagsSelector.hpp"
#include "TreeViewTagHierSelector.hpp"
#include "GetCppTagSearchMatcher.hpp"
#include "ShellCommand.hpp"
#include "WinCommand.hpp"

#include "Logger.hpp"
#include "Log.hpp"

#include "Commands.hpp"
#include "Results.hpp"
#include "MessageHandler.hpp"

#include "CtagsMeasTags.hpp"
#include "Samples.hpp"
#include "Counters.hpp"
#include "ObjectsCounter.hpp"
#include "Tag.hpp"
#include "CppTag.hpp"
#include "GenericKindTag.hpp"

extern NppPlugin::TagsPlugin g_plugin;

namespace NppPlugin
{

void myTest1()
{
	g_plugin.test1();
}
void myTest2()
{
    g_plugin.test2();
}
void fun_nextTag()
{
    g_plugin.nextTag();
}
void fun_previousTag()
{
    g_plugin.previousTag();
}
void fun_findTag()
{
    g_plugin.findTag();
}
void fun_tagInfo()
{
    g_plugin.tagInfo();
}
void fun_cppSearch()
{
	g_plugin.cppSearch();
}
void fun_tagHier()
{
	g_plugin.tagHierarchy();
}
void fun_classDiagram()
{
	g_plugin.matchedTagsClassDiagram();
}
void fun_tagClassDiagram()
{
	g_plugin.tagClassDiagram();
}
void fun_clearTags()
{
    g_plugin.clearTags();
}
void fun_setTagsFiles()
{
    g_plugin.setTagsFiles();
}

void fun_generateTagsFile()
{
    g_plugin.generateTagsFile();
}

void fun_info()
{
    g_plugin.info();
}

static ShortcutKey nextTagSk     = {false, true, false, VK_RIGHT};
static ShortcutKey previousTagSk = {false, true, false, VK_LEFT};
static ShortcutKey findTagSk     = {false, true, false, VK_SPACE};
static ShortcutKey clearTagsSk   = {true,  true, false, 'C'};
static ShortcutKey tagInfoSk     = {true,  true, false, 'T'};
static ShortcutKey setTagFilesSk = {true,  true, false, 'O'};
static ShortcutKey generateTagSk = {true,  true, false, 'G'};
static ShortcutKey cppSearchSk =   {true,  true, false, 'H'};


std::unique_ptr<Plugin::Command> buildBasicCommand(const std::string& p_cmd, const std::string& p_args)
{
	return std::make_unique<Plugin::ShellCommand>(p_cmd + " " + p_args);
}

std::unique_ptr<Plugin::Command> buildWinCommand(const std::string& p_cmd, const std::string& p_args)
{
	return std::make_unique<Plugin::WinCommand>(p_cmd, p_args);
}

std::unique_ptr<Plugin::Command> buildWinCommandWithOutput(const std::string& p_cmd, const std::string& p_args)
{
	return std::make_unique<Plugin::WinCommandWithInOut>(p_cmd + " " + p_args);
}

TagsPlugin::TagsPlugin()
	: files(npp.npp, hModule)
{}

void TagsPlugin::init()
{
    createTagsController();
	messagesNames.add(CTagsPlugin::Command::GenerateTags::Id(), "CTags::GenerateTags");
	messagesNames.add(CTagsPlugin::Command::GetTagFiles::Id(), "CTags::GetTagFiles");
	messagesNames.add(CTagsPlugin::Command::SetTagFiles::Id(), "CTags::SetTagFiles");
	messagesNames.add(CTagsPlugin::Command::Test::Id(), "CTags::Test");
}

void TagsPlugin::detach()
{
	config.saveConfigFile(configFilePath);
	
	LOG_INFO << "STATS " << Meas::Samples<CTagsPlugin::TagsLoadTime>::print();
	LOG_INFO << "STATS " << Meas::Samples<CTagsPlugin::FindChildrenTagsTime>::print();
	LOG_INFO << "STATS " << Meas::Samples<CTagsPlugin::ParseTagsHierarchyTime>::print();
	LOG_INFO << "STATS " << Meas::Samples<CTagsPlugin::GenerateClassInDiagramTime>::print();
	LOG_INFO << "STATS " << Meas::Samples<CTagsPlugin::GenerateClassDiagramTime>::print();
	LOG_INFO << "STATS " << Meas::Percent<CTagsPlugin::ChildrenTagsCacheHits, CTagsPlugin::ChildrenTagsCallCount>::print();
	LOG_INFO << "STATS " << Meas::Percent<CTagsPlugin::TagsHierarchyCacheHits, CTagsPlugin::TagsHierarchyCallCount>::print();
	LOG_INFO << "STATS " << Meas::ObjectsCounter<CTagsPlugin::Tag>::print();
	LOG_INFO << "STATS " << Meas::ObjectsCounter<CTagsPlugin::CppTag>::print();
	LOG_INFO << "STATS " << Meas::ObjectsCounter<CTagsPlugin::GenericKindTag>::print();
}

void TagsPlugin::onNppHandleSet()
{
	Logger::init(getPluginsConfigDir() + "\\nppCTagPlugin.logs");
	loadConfigFile();
	setLoggerParams();
}

void TagsPlugin::initMenu()
{
	setCommand(TEXT("Next tag"), fun_nextTag, &nextTagSk);
	setCommand(TEXT("Previous tag"), fun_previousTag, &previousTagSk);
	setCommand(TEXT("Clear tags queue"), fun_clearTags, &clearTagsSk);
	setSeparator();
	setCommand(TEXT("Find tag"), fun_findTag, &findTagSk);
	setCommand(TEXT("Tag info"), fun_tagInfo, &tagInfoSk);
	setCommand(TEXT("Tag hierarchy"), fun_tagHier, NULL);
	setCommand(TEXT("Cpp search"), fun_cppSearch, &cppSearchSk);
	setSeparator();
	setCommand(TEXT("Generate current tag class diagram"), fun_tagClassDiagram, NULL);
	setCommand(TEXT("Generate class diagram"), fun_classDiagram, NULL);
	setSeparator();
	setCommand(TEXT("Set tags files"), fun_setTagsFiles, &setTagFilesSk);
	setCommand(TEXT("Generate tags file"), fun_generateTagsFile, &generateTagSk);
	setSeparator();
	setCommand(TEXT("About"), fun_info, NULL);
	setSeparator();
	setCommand(TEXT("test 1"), myTest1, NULL);
	setCommand(TEXT("test 2"), myTest2, NULL);
}

void TagsPlugin::setLoggerParams()
{
	Logger::setLogLevel(config.getLogSeverity());
	if (config.isLoggerEnabled())
		Logger::enable();
	else
		Logger::disable();
}

void TagsPlugin::loadConfigFile()
{
    configFilePath = getPluginsConfigDir() + "\\nppCTagPlugin.ini";
    config.loadConfigFile(configFilePath);
}

void TagsPlugin::createTagsController()
{
    tagsController.reset(new CTagsPlugin::CTagsController(
		npp,
		ui,
		files,
		std::make_unique<CTagsPlugin::LazyInitializedTagsSelector>(std::bind(&TagsPlugin::buildTagsSelector, this)),
		std::make_unique<CTagsPlugin::TreeViewTagHierSelector>(hModule, npp.npp),
		std::make_shared<CTagsPlugin::MultipleTagFilesReader>(std::bind(&TagsPlugin::buildTagReader, this, std::placeholders::_1), config),
        config,
		WinApi::CppSearchMatcherGetter(npp.npp, hModule),
		&buildWinCommand));
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildReadTagsProxy(const std::string& p_tagFilePath)
{
    return std::make_unique<CTagsPlugin::ReadTagsProxy>(config,[=](){ return p_tagFilePath; }, &buildWinCommandWithOutput);
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildTagFileReader(const std::string& p_tagFilePath)
{
    return std::make_unique<CTagsPlugin::TagFileReader>([=](){ return p_tagFilePath; });
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildNativeTagReader(const std::string& p_tagFilePath)
{
    switch (config.getTagsReaderType())
    {
        case CTagsPlugin::TagReaderType::Internal   : return buildTagFileReader(p_tagFilePath);
        case CTagsPlugin::TagReaderType::ReadTagExe : return buildReadTagsProxy(p_tagFilePath);
    }
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildCachedTagFileReader(const std::string& p_tagFilePath, std::unique_ptr<CTagsPlugin::ITagsReader> p_reader)
{
	return std::make_unique<CTagsPlugin::CachedTagsReader>(
		std::move(p_reader),
		p_tagFilePath,
		[&]() { tagsLoaded(); });
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildFileScopedTagFileReader(std::unique_ptr<CTagsPlugin::ITagsReader> p_reader)
{
	return std::make_unique<CTagsPlugin::FileScopedTagFilteringReader>(
		npp,
		std::move(p_reader));
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildTagReader(const std::string& p_tagFilePath)
{
	LOG_DEBUG << "Build tags reader for file " << p_tagFilePath;
	std::unique_ptr<CTagsPlugin::ITagsReader> reader(std::move(buildNativeTagReader(p_tagFilePath)));
	if (config.shouldCacheTags())
		reader = std::move(buildCachedTagFileReader(p_tagFilePath, std::move(reader)));
	if (config.shouldFilterFileScopedTags())
		reader = std::move(buildFileScopedTagFileReader(std::move(reader)));
	return reader;
}

std::unique_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildListViewSelector()
{
	return std::make_unique<CTagsPlugin::ListViewTagsSelector>(hModule, npp.npp);
}

std::unique_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildGridViewSelector()
{
    return std::make_unique<CTagsPlugin::GridViewTagsSelector>(
        npp.npp, hModule, config);
}

std::unique_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildTagsSelector()
{
    switch(config.getSelectTagsViewType())
    {
        case CTagsPlugin::SelectTagsViewType::ListView : return buildListViewSelector();
        case CTagsPlugin::SelectTagsViewType::GridView : return buildGridViewSelector();
    }
}

void TagsPlugin::tagsLoaded()
{
	tagsController->onTagsLoaded();
}

void TagsPlugin::handleMsgToPlugin(CommunicationInfo& p_message)
{
	LOG_INFO << "Received message with internal Id " << p_message.internalMsg
		<< " [" << messagesNames.get(p_message.internalMsg) << "]"
		<< " from " << p_message.srcModuleName;
	try
	{
		tagsController->handleTransaction(p_message.internalMsg, *static_cast<Messaging::Transaction*>(p_message.info));
	}
	catch (std::exception& e)
	{
        Messaging::Transaction* transaction = static_cast<Messaging::Transaction*>(p_message.info);
        transaction->result.size = 0;
		LOG_ERROR << "Exception occured during hadling plugin message: " << typeid(e).name() << ". Details: " << e.what();
	}
}

void TagsPlugin::test1()
{
	tagsController->classDiagram();
}

void TagsPlugin::test2()
{
}

void TagsPlugin::nextTag()
{
    tagsController->next();
}

void TagsPlugin::previousTag()
{
    tagsController->previous();
}

void TagsPlugin::findTag()
{
    tagsController->find();
}
void TagsPlugin::tagInfo()
{
	tagsController->tagInfo();
}
void TagsPlugin::cppSearch()
{
	tagsController->tagsSearch();
}
void TagsPlugin::tagHierarchy()
{
	tagsController->tagHierarchy();
}
void TagsPlugin::tagClassDiagram()
{
	tagsController->tagClassDiagram();
}
void TagsPlugin::matchedTagsClassDiagram()
{
	tagsController->classDiagram();
}
void TagsPlugin::clearTags()
{
    tagsController->clear();
}

void TagsPlugin::setTagsFiles()
{
    tagsController->setTagFiles();
}

void TagsPlugin::generateTagsFile()
{
    tagsController->generateTags();
}

void TagsPlugin::info()
{
    std::string l_info = "Project:    CTags plugin for notepad++\n\nVersion:    1.8.1\n\nPage:    https://kwiato88.github.io/EditorPlugins/";
    ui.infoMessage("About", l_info);
}

} // namespace NppPlugin
