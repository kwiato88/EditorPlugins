#include <memory>
#include <fstream>

#include "Plugin.hpp"

#include "NppPathGetter.hpp"
#include "NppPathsSelector.hpp"
#include "ReadTagsProxy.hpp"
#include "TagFileReader.hpp"
#include "CachedTagsReader.hpp"
#include "MultipleTagFilesReader.hpp"
#include "FileScopedTagFilteringReader.hpp"
#include "CTagsController.hpp"
#include "ListViewTagsSelector.hpp"
#include "GridViewTagsSelector.hpp"
#include "TreeViewTagHierSelector.hpp"
#include "GetCppTagSearchMatcher.hpp"

#include "Logger.hpp"
#include "Log.hpp"

#include "Commands.hpp"
#include "Results.hpp"
#include "MessageHandler.hpp"

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
	m_config.saveConfigFile(m_configFilePath);
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
	setCommand(TEXT("Set tags files"), fun_setTagsFiles, &setTagFilesSk);
	setCommand(TEXT("Generate tags file"), fun_generateTagsFile, &generateTagSk);
	setSeparator();
	setCommand(TEXT("About"), fun_info, NULL);
	//setSeparator();
	//setCommand(TEXT("test 1"), myTest1, NULL);
	//setCommand(TEXT("test 2"), myTest2, NULL);
}

void TagsPlugin::setLoggerParams()
{
	Logger::setLogLevel(m_config.getLogSeverity());
	if (m_config.isLoggerEnabled())
		Logger::enable();
	else
		Logger::disable();
}

void TagsPlugin::loadConfigFile()
{
    m_configFilePath = getPluginsConfigDir() + "\\nppCTagPlugin.ini";
    m_config.loadConfigFile(m_configFilePath);
}

std::string TagsPlugin::getPluginsConfigDir()
{
    TCHAR l_configDirPath[_MAX_PATH];
    ::SendMessage(npp.npp, NPPM_GETPLUGINSCONFIGDIR , (WPARAM)_MAX_PATH, (LPARAM)l_configDirPath);
    char l_configDirPathBuff[_MAX_PATH];
    wcstombs(l_configDirPathBuff, l_configDirPath, _MAX_PATH - 1);
    return l_configDirPathBuff;
}

void TagsPlugin::createTagsController()
{
    m_tagsController.reset(new CTagsPlugin::CTagsController(
		npp,
		ui,
        std::make_shared<NppPlugin::NppPathGetter>(npp.npp, hModule),
		std::bind(&TagsPlugin::getTagsSelector, this),
		std::make_unique<CTagsPlugin::TreeViewTagHierSelector>(hModule, npp.npp),
		std::make_shared<NppPlugin::PathsSelector<SelectorType::Directory>>(npp.npp, hModule),
		std::make_shared<NppPlugin::PathsSelector<SelectorType::File>>(npp.npp, hModule),
		std::make_shared<CTagsPlugin::MultipleTagFilesReader>(std::bind(&TagsPlugin::buildTagReader, this, std::placeholders::_1), m_config),
        m_config,
		WinApi::CppSearchMatcherGetter(npp.npp, hModule)));
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildReadTagsProxy(const std::string& p_tagFilePath)
{
    return std::make_unique<CTagsPlugin::ReadTagsProxy>(m_config,[=](){ return p_tagFilePath; });
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildTagFileReader(const std::string& p_tagFilePath)
{
    return std::make_unique<CTagsPlugin::TagFileReader>([=](){ return p_tagFilePath; });
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildNativeTagReader(const std::string& p_tagFilePath)
{
    switch (m_config.getTagsReaderType())
    {
        case CTagsPlugin::TagReaderType::Internal   : return buildTagFileReader(p_tagFilePath);
        case CTagsPlugin::TagReaderType::ReadTagExe : return buildReadTagsProxy(p_tagFilePath);
    }
}

std::unique_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildCachedTagFileReader(const std::string& p_tagFilePath, std::unique_ptr<CTagsPlugin::ITagsReader> p_reader)
{
	return std::make_unique<CTagsPlugin::CachedTagsReader>(
		std::move(p_reader),
		p_tagFilePath);
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
	if (m_config.shouldCacheTags())
		reader = std::move(buildCachedTagFileReader(p_tagFilePath, std::move(reader)));
	if (m_config.shouldFilterFileScopedTags())
		reader = std::move(buildFileScopedTagFileReader(std::move(reader)));
	return reader;
}

std::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildListViewSelector()
{
	return std::make_shared<CTagsPlugin::ListViewTagsSelector>(hModule, npp.npp);
}

std::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildGridViewSelector()
{
    return std::make_shared<CTagsPlugin::GridViewTagsSelector>(
        npp.npp, hModule, m_config);
}

std::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildTagsSelector()
{
    switch(m_config.getSelectTagsViewType())
    {
        case CTagsPlugin::SelectTagsViewType::ListView : return buildListViewSelector();
        case CTagsPlugin::SelectTagsViewType::GridView : return buildGridViewSelector();
    }
}

std::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::getTagsSelector()
{
    if(!m_selector)
        m_selector = buildTagsSelector();
    return m_selector;
}

void TagsPlugin::handleMsgToPlugin(CommunicationInfo& p_message)
{
	LOG_INFO << "Received message with internal Id " << p_message.internalMsg
		<< " [" << messagesNames.get(p_message.internalMsg) << "]"
		<< " from " << p_message.srcModuleName;
	try
	{
		m_tagsController->handleTransaction(p_message.internalMsg, *static_cast<Messaging::Transaction*>(p_message.info));
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

}

void TagsPlugin::test2()
{

}

void TagsPlugin::nextTag()
{
    m_tagsController->next();
}

void TagsPlugin::previousTag()
{
    m_tagsController->previous();
}

void TagsPlugin::findTag()
{
    m_tagsController->find();
}
void TagsPlugin::tagInfo()
{
	m_tagsController->tagInfo();
}
void TagsPlugin::cppSearch()
{
	m_tagsController->tagsSearch();
}
void TagsPlugin::tagHierarchy()
{
	m_tagsController->tagHierarchy();
}
void TagsPlugin::clearTags()
{
    m_tagsController->clear();
}

void TagsPlugin::setTagsFiles()
{
    m_tagsController->setTagFiles();
}

void TagsPlugin::generateTagsFile()
{
    m_tagsController->generateTags();
}

void TagsPlugin::info()
{
    std::string l_info = "Project:    CTags plugin for notepad++\n\nVersion:    1.7.1\n\nPage:    https://kwiato88.github.io/EditorPlugins/";
    ui.infoMessage("About", l_info);
}

} // namespace NppPlugin
