#include <memory>
#include <fstream>

#include "menuCmdID.h"
#include "OpenFilePlugin.hpp"
#include "OpenFileDialog.hpp"
#include "NppPathsSelector.hpp"

extern NppPlugin::OpenFilePlugin g_plugin;

namespace NppPlugin
{

void fun_open()
{
    g_plugin.open();
}

void fun_setDirs()
{
    g_plugin.setDirsSafe();
}

//static ShortcutKey switchFileSk = {false, false, false, VK_F9};

namespace
{
std::string getFileDir(std::string p_filePath)
{
    return p_filePath.substr(0, p_filePath.find_last_of("\\"));
}
}

OpenFilePlugin::OpenFilePlugin()
 : m_isInitialized(false), ui(m_npp.npp, m_hModule)
{
}

/**
* Initialize your plugin data here
* It will be called while plugin loading   
*/
void OpenFilePlugin::init(HINSTANCE p_hModule)
{
    if(!m_isInitialized)
    {
        m_hModule = p_hModule;
        create();
        m_isInitialized = true;
    }
}

/**
* Cleaning of your plugin
* It will be called while plugin unloading
*/
void OpenFilePlugin::cleanup()
{
}

/**
* Initialization of your plugin commands
*/
void OpenFilePlugin::commandMenuInit(NppData p_nppData)
{
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
    initFunctionsTable();
}

void OpenFilePlugin::create()
{
}

void OpenFilePlugin::initFunctionsTable()
{
	setCommand(0, TEXT("Open File"),       fun_open,   NULL);
    setCommand(1, TEXT("Set search dirs"), fun_setDirs, NULL);
}

/**
* Clean up your plugin commands allocation (if any)
*/
void OpenFilePlugin::commandMenuCleanUp()
{
}

bool OpenFilePlugin::setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (index >= OpenFilePlugin::s_funcNum)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(m_funcItems[index]._itemName, cmdName);
    m_funcItems[index]._pFunc = pFunc;
    m_funcItems[index]._init2Check = false;
    m_funcItems[index]._pShKey = sk;

    return true;
}

void OpenFilePlugin::open()
{
    try
    {
        openFile();
    }
    catch(const std::exception& e)
    {
		ui.errorMessage("Open File", e.what());
    }
}

void OpenFilePlugin::openFile()
{
    WinApi::OpenFileDialog dialog(m_hModule, m_npp.npp);
    dialog.setSearchDirs(getSerachDirs());
    int result = dialog.show();
    if(result == WinApi::Dialog::RESULT_OK)
    {
        m_npp.setFile(dialog.getSelectedFile());
    }
}

std::vector<std::string> OpenFilePlugin::getSerachDirs()
{
	if (m_searchDirs.empty())
		setDirs();
	if (m_searchDirs.empty())
		throw std::runtime_error("No search dirs selected");
	return m_searchDirs;
}

void OpenFilePlugin::setDirs()
{
	PathsSelector<SelectorType::Directory> dirs(m_npp.npp, m_hModule);
	m_searchDirs = dirs.select(m_searchDirs, getFileDir(m_npp.getFile()));
}

void OpenFilePlugin::setDirsSafe()
{
	try
	{
		setDirs();
	}
	catch (const std::exception& e)
	{
		ui.errorMessage("Set dir", e.what());
	}
}

} // namespace NppPlugin
