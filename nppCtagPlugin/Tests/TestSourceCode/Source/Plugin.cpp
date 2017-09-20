#include <memory>
#include <fstream>
#include <boost/make_shared.hpp>

#include "menuCmdID.h"
#include "Plugin.hpp"

#include "NppLocationSetter.hpp"
#include "NppLocationGetter.hpp"
#include "NppMessagePrinter.hpp"
#include "NppListViewSelector.hpp"
#include "NppPathGetter.hpp"
#include "NppTextReader.hpp"
#include "NppPathsSelector.hpp"
#include "ReadTagsProxy.hpp"
#include "TagFileReader.hpp"
#include "MultipleTagFilesReader.hpp"
#include "FileScopedTagFilteringReader.hpp"
#include "CTagsController.hpp"
#include "TagsSelector.hpp"
#include "GridViewTagsSelector.hpp"
#include "CompositeTagPrinter.hpp"
#include "TagPrinters.hpp"
#include "getCurrentHScintilla.hpp"

#include <boost/assign/list_of.hpp>
#include "GridDialog.hpp"

#include "TagMatchers.hpp"
#include "SelectPathsDialog.hpp"
//#include <sstream>
//#include <boost/timer.hpp>
//#include <boost/lexical_cast.hpp>
//#include <numeric>
#include "Logger.hpp"
//#include <fstream>

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

TagsPlugin::TagsPlugin()
 : m_isInitialized(false)
{
}

/**
* Initialize your plugin data here
* It will be called while plugin loading   
*/
void TagsPlugin::init(HINSTANCE p_hModule)
{
    if(!m_isInitialized)
    {
        m_hModule = p_hModule;
        createTagsController();
        m_isInitialized = true;
    }
}

/**
* Cleaning of your plugin
* It will be called while plugin unloading
*/
void TagsPlugin::cleanup()
{
}

/**
* Initialization of your plugin commands
*/
void TagsPlugin::commandMenuInit(NppData p_nppData)
{
    m_nppData = p_nppData;
    loadConfigFile();
    initFunctionsTable();
    Logger::init(getPluginsConfigDir() + "\\nppCTagPluginLOG.txt");
}

void TagsPlugin::loadConfigFile()
{
    m_configFilePath = getPluginsConfigDir() + "\\nppCTagPlugin.ini";
    m_config.loadConfigFile(m_configFilePath);
}

std::string TagsPlugin::getPluginsConfigDir()
{
    TCHAR l_configDirPath[_MAX_PATH];
    ::SendMessage(m_nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR , (WPARAM)_MAX_PATH, (LPARAM)l_configDirPath);
    char l_configDirPathBuff[_MAX_PATH];
    wcstombs(l_configDirPathBuff, l_configDirPath, _MAX_PATH - 1);
    return l_configDirPathBuff;
}

void TagsPlugin::createTagsController()
{
    m_printer.reset(new NppPlugin::NppMessagePrinter(m_nppData, m_hModule));

    m_tagsController.reset(new CTagsPlugin::CTagsController(
        boost::make_shared<NppPlugin::NppLocationSetter>(m_nppData),
        boost::make_shared<NppPlugin::NppLocationGetter>(m_nppData),
        m_printer,
        boost::make_shared<NppPlugin::NppPathGetter>(m_nppData,m_hModule),
        boost::bind(&TagsPlugin::getTagsSelector, this),
        boost::make_shared<NppPlugin::NppTextReader>(m_nppData),
        boost::make_shared<NppPlugin::PathsSelector<SelectorType::Directory>>(m_nppData,m_hModule),
        boost::make_shared<NppPlugin::PathsSelector<SelectorType::File>>(m_nppData,m_hModule),
        boost::make_shared<CTagsPlugin::MultipleTagFilesReader>(boost::bind(&TagsPlugin::buildTagReader, this, _1), m_config),
        m_config));
}

boost::shared_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildReadTagsProxy(const std::string& p_tagFilePath)
{
    return boost::make_shared<CTagsPlugin::ReadTagsProxy>(m_config,[=](){ return p_tagFilePath; } , m_printer);
}

boost::shared_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildTagFileReader(const std::string& p_tagFilePath)
{
    return boost::make_shared<CTagsPlugin::TagFileReader>([=](){ return p_tagFilePath; });
}

boost::shared_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildNativeTagReader(const std::string& p_tagFilePath)
{
    switch (m_config.getTagsReaderType())
    {
        case CTagsPlugin::TagReaderType::Internal   : return buildTagFileReader(p_tagFilePath);
        case CTagsPlugin::TagReaderType::ReadTagExe : return buildReadTagsProxy(p_tagFilePath);
    }
}

boost::shared_ptr<CTagsPlugin::ITagsReader> TagsPlugin::buildTagReader(const std::string& p_tagFilePath)
{
	boost::shared_ptr<CTagsPlugin::ITagsReader> reader = buildNativeTagReader(p_tagFilePath);
	if (m_config.shouldFilterFileScopedTags())
		return boost::make_shared<CTagsPlugin::FileScopedTagFilteringReader>(
			boost::make_shared<NppPlugin::NppLocationGetter>(m_nppData),
			reader);
	return reader;
}

boost::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildListViewSelector()
{
    return boost::make_shared<CTagsPlugin::TagsSelector>(
                   boost::make_shared<NppPlugin::NppListViewSelector>(m_nppData, m_hModule));
}

boost::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildGridViewSelector()
{
    return boost::make_shared<CTagsPlugin::GridViewTagsSelector>(
        m_nppData, m_hModule,
        std::make_shared<CTagsPlugin::CompositeTagPrinter>(m_config, CTagsPlugin::buildTagPrinters(m_config)));
}

boost::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildTagsSelector()
{
    switch(m_config.getSelectTagsViewType())
    {
        case CTagsPlugin::SelectTagsViewType::ListView : return buildListViewSelector();
        case CTagsPlugin::SelectTagsViewType::GridView : return buildGridViewSelector();
    }
}

boost::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::getTagsSelector()
{
    if(!m_selector)
        m_selector = buildTagsSelector();
    return m_selector;
}

void TagsPlugin::initFunctionsTable()
{
    size_t i = 0;
    setCommand(i++, TEXT("Next tag"),           fun_nextTag,          &nextTagSk);
    setCommand(i++, TEXT("Previous tag"),       fun_previousTag,      &previousTagSk);
    setCommand(i++, TEXT("Find tag"),           fun_findTag,          &findTagSk);
    setCommand(i++, TEXT("Clear tags queue"),   fun_clearTags,        &clearTagsSk);
    setCommand(i++, TEXT("Tag info"),           fun_tagInfo,          &tagInfoSk);
    setSeparator(i++);
    setCommand(i++, TEXT("Set tags files"),     fun_setTagsFiles,     &setTagFilesSk);
    setCommand(i++, TEXT("Generate tags file"), fun_generateTagsFile, &generateTagSk);
    setSeparator(i++);
    setCommand(i++, TEXT("About"),              fun_info,             NULL);

    //setCommand(i++, TEXT("test 1"), myTest1, NULL);
    //setCommand(i++, TEXT("test 2"), myTest2, NULL);
}

/**
* Clean up your plugin commands allocation (if any)
*/
void TagsPlugin::commandMenuCleanUp()
{
    m_config.saveConfigFile(m_configFilePath);
}

bool TagsPlugin::setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (index >= TagsPlugin::s_funcNum)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(m_funcItems[index]._itemName, cmdName);
    m_funcItems[index]._pFunc = pFunc;
    m_funcItems[index]._init2Check = false;
    m_funcItems[index]._pShKey = sk;

    return true;
}

void TagsPlugin::setSeparator(size_t index)
{
    setCommand(index, TEXT("-----"), NULL, NULL);
}
/*
double measureExecutionTime(std::function<void(void)> functionToMeasure,
                            CTagsPlugin::TagMatcher matcher)
{
    boost::timer tmr;
    functionToMeasure();
    return tmr.elapsed();
}
*/
void TagsPlugin::test1()
{
}

void TagsPlugin::test2()
{
    /*
    std::stringstream log;
    auto reader = boost::make_shared<CTagsPlugin::MultipleTagFilesReader>(boost::bind(&TagsPlugin::buildTagReader, this, _1), m_config);
    auto allTags = reader->findTag([](const CTagsPlugin::Tag&){ return true;});
    log << "tags count: " << tags.size();
    m_printer->printInfoMessage("size", log.str());
    */
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
    m_tagsController->generateTagFile();
}

void TagsPlugin::info()
{
    std::string l_info = "Project:    CTags plugin for notepad++\n\nVersion:    1.5";
    m_printer->printInfoMessage("About", l_info);
}

} // namespace NppPlugin
