#include <memory>
#include <fstream>

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
#include "CachedTagsReader.hpp"
#include "MultipleTagFilesReader.hpp"
#include "FileScopedTagFilteringReader.hpp"
#include "CTagsController.hpp"
#include "ListViewTagsSelector.hpp"
#include "GridViewTagsSelector.hpp"
#include "TreeViewTagHierSelector.hpp"

#include <boost/assign/list_of.hpp>
#include "GridDialog.hpp"
#include "SelectPathsDialog.hpp"
#include "Logger.hpp"
#include "Log.hpp"

#include "CppSearchDialog.hpp"
#include "CppIsTagWithAtt.hpp"
#include "UserInputError.hpp"

#include <boost/lexical_cast.hpp>
#include "TreeDialog.hpp"
#include "TagHierarchyDialog.hpp"

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

//TODO: extract to different file
class CppSearchMatcherGetter
{
public:
	CppSearchMatcherGetter(WinApi::Handle& p_parrent, WinApi::InstanceHandle& p_hModule)
		: m_parrent(p_parrent), m_hModule(p_hModule)
	{}

	CTagsPlugin::TagMatcher operator()()
	{
		WinApi::CppSearchDialog dialog(m_hModule, m_parrent);
		if (dialog.show() == WinApi::CppSearchDialog::BUTTON_OK)
			return dialog.getTagMatcher();
		throw Plugin::UserInputError("No cpp tag attributes selected");
	}

private:
	WinApi::Handle& m_parrent;
	WinApi::InstanceHandle& m_hModule;
};

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
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
	Logger::init(getPluginsConfigDir() + "\\nppCTagPlugin.logs");
    loadConfigFile();
	setLoggerParams();
    initFunctionsTable();
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
    ::SendMessage(m_npp.npp, NPPM_GETPLUGINSCONFIGDIR , (WPARAM)_MAX_PATH, (LPARAM)l_configDirPath);
    char l_configDirPathBuff[_MAX_PATH];
    wcstombs(l_configDirPathBuff, l_configDirPath, _MAX_PATH - 1);
    return l_configDirPathBuff;
}

void TagsPlugin::createTagsController()
{
    m_printer.reset(new NppPlugin::NppMessagePrinter(m_npp.npp, m_hModule));

    m_tagsController.reset(new CTagsPlugin::CTagsController(
		std::make_shared<NppPlugin::NppLocationSetter>(m_npp),
		std::make_shared<NppPlugin::NppLocationGetter>(m_npp),
        m_printer,
		std::make_shared<NppPlugin::NppPathGetter>(m_npp.npp, m_hModule),
		std::bind(&TagsPlugin::getTagsSelector, this),
		std::make_unique<CTagsPlugin::TreeViewTagHierSelector>(m_hModule, m_npp.npp),
		std::make_shared<NppPlugin::NppTextReader>(m_npp),
		std::make_shared<NppPlugin::PathsSelector<SelectorType::Directory>>(m_npp.npp, m_hModule),
		std::make_shared<NppPlugin::PathsSelector<SelectorType::File>>(m_npp.npp, m_hModule),
		std::make_shared<CTagsPlugin::MultipleTagFilesReader>(std::bind(&TagsPlugin::buildTagReader, this, std::placeholders::_1), m_config),
        m_config,
		CppSearchMatcherGetter(m_npp.npp, m_hModule)));
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
		std::make_shared<NppPlugin::NppLocationGetter>(m_npp),
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
	return std::make_shared<CTagsPlugin::ListViewTagsSelector>(m_hModule, m_npp.npp);
}

std::shared_ptr<CTagsPlugin::ITagsSelector> TagsPlugin::buildGridViewSelector()
{
    return std::make_shared<CTagsPlugin::GridViewTagsSelector>(
        m_npp.npp, m_hModule, m_config);
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

void TagsPlugin::initFunctionsTable()
{
    setCommand(TEXT("Next tag"),           fun_nextTag,          &nextTagSk);
    setCommand(TEXT("Previous tag"),       fun_previousTag,      &previousTagSk);
	setCommand(TEXT("Clear tags queue"),   fun_clearTags,        &clearTagsSk);
	setSeparator();
	setCommand(TEXT("Find tag"),           fun_findTag,          &findTagSk);
	setCommand(TEXT("Tag info"),           fun_tagInfo,    &tagInfoSk);
	setCommand(TEXT("Tag hierarchy"),      fun_tagHier,          NULL);
	setCommand(TEXT("Cpp search"),         fun_cppSearch,        &cppSearchSk);
    setSeparator();
    setCommand(TEXT("Set tags files"),     fun_setTagsFiles,     &setTagFilesSk);
    setCommand(TEXT("Generate tags file"), fun_generateTagsFile, &generateTagSk);
    setSeparator();
    setCommand(TEXT("About"),              fun_info,             NULL);
	//setSeparator();
    //setCommand(TEXT("test 1"), myTest1, NULL);
	//setCommand(TEXT("test 2"), myTest2, NULL);
}

/**
* Clean up your plugin commands allocation (if any)
*/
void TagsPlugin::commandMenuCleanUp()
{
    m_config.saveConfigFile(m_configFilePath);
}

bool TagsPlugin::setCommand(TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (m_numberOfAddedFunctions >= TagsPlugin::s_funcNum)
        return false;

    lstrcpy(m_funcItems[m_numberOfAddedFunctions]._itemName, cmdName);
    m_funcItems[m_numberOfAddedFunctions]._pFunc = pFunc;
    m_funcItems[m_numberOfAddedFunctions]._init2Check = false;
    m_funcItems[m_numberOfAddedFunctions]._pShKey = sk;
	m_numberOfAddedFunctions++;

    return true;
}

void TagsPlugin::setSeparator()
{
    setCommand(TEXT("-----"), NULL, NULL);
}

void TagsPlugin::test1()
{
	std::string childACont = "A", childBCont = "B", childCCont = "C", childBACont = "BA", rootCont = "root", dummy = "dummy";
	std::vector<WinApi::Node> firstlevelChildren
	{
		WinApi::Node("Child a", &childACont),
		WinApi::Node("Child b 9012345", &childBCont),
		WinApi::Node("Child c", &childCCont),
		WinApi::Node("12345678901234567", &dummy)};
	firstlevelChildren[1].m_children.push_back(WinApi::Node("child ba 01234567", &childBACont));
	WinApi::Node root("root a", &rootCont);
	root.m_children = firstlevelChildren;

	WinApi::TagHierarchyDialog dialog(WinApi::InstanceHandle(m_hModule), m_npp.npp);
	dialog.setBaseTags(root);
	dialog.setDerivedTags(root);
	dialog.show();
	const std::string* selectedItemContext = (dialog.getSelectedContext<std::string>());
	std::string selectedPrint = (selectedItemContext == nullptr) ? "none" : *selectedItemContext;
	m_printer->printInfoMessage("her dialog test", selectedPrint);
}

void TagsPlugin::test2()
{
	std::string childACont = "A", childBCont = "B", childCCont = "C", childBACont = "BA", rootCont = "root", dummy = "dummy";
	std::vector<WinApi::Node> firstlevelChildren
	{
		WinApi::Node("Child a 9012345", &childACont),
		WinApi::Node("Child b", &childBCont),
		WinApi::Node("Child c", &childCCont),
		WinApi::Node("12345678901234567", &dummy) };
	firstlevelChildren[1].m_children.push_back(WinApi::Node("child ba 012345", &childBACont));
	WinApi::Node root("root a", &rootCont);
	root.m_children = firstlevelChildren;

	WinApi::TreeDialog tree(m_hModule, m_npp.npp);
	tree.setTreeNodes({ root });
	tree.show();
	const std::string* selected = tree.getSelectedItemContext<std::string>();
	std::string toPrint = (selected == nullptr) ? "NONE" : (*selected);
	std::string toPrint2 = tree.getSelectedItemContextRef<std::string>();
	m_printer->printInfoMessage("tree dialog test", "selected item index" + toPrint + toPrint2);
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
    std::string l_info = "Project:    CTags plugin for notepad++\n\nVersion:    1.6.1";
    m_printer->printInfoMessage("About", l_info);
}

} // namespace NppPlugin
