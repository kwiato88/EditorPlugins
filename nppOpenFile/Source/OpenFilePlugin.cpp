#include <memory>
#include <fstream>

#include "menuCmdID.h"
#include "OpenFilePlugin.hpp"
#include "OpenFileDialog.hpp"
#include "NppPathGetter.hpp"
#include "NppLocationGetter.hpp"
#include "NppLocationSetter.hpp"
#include "NppMessagePrinter.hpp"

extern NppPlugin::OpenFilePlugin g_plugin;

namespace NppPlugin
{

void fun_open()
{
    g_plugin.open();
}

void fun_setDir()
{
    g_plugin.setDir();
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
 : m_isInitialized(false)
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
    // TODO: set shortKey
	setCommand(0, TEXT("Open File"),      fun_open,   NULL);
    setCommand(1, TEXT("Set search dir"), fun_setDir, NULL);
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
        NppMessagePrinter printer(m_npp.npp, m_hModule);
        printer.printErrorMessage("Open File", e.what());
    }
}

void OpenFilePlugin::openFile()
{
    WinApi::OpenFileDialog dialog(m_hModule, m_npp.npp);
    dialog.setSearchDir(getSerachDir());
    int result = dialog.show();
    if(result == WinApi::OpenFileDialog::BUTTON_OK)
    {
        NppLocationSetter setter(m_npp);
        setter.setFile(dialog.getSelectedFile());
    }
}

std::string OpenFilePlugin::getSerachDir()
{
	if (m_searchDir.empty())
		setDir();
	return m_searchDir;
}

void OpenFilePlugin::setDir()
{
    NppPathGetter pathGetter(m_npp.npp, m_hModule);
    NppLocationGetter locationGetter(m_npp);
    
    m_searchDir = pathGetter.getDirPath("Select search dir", getFileDir(locationGetter.getFile()));
}

} // namespace NppPlugin
