#include <memory>
#include <fstream>

#include "menuCmdID.h"
#include "SwitcherPlugin.hpp"

#include "SwitcherFunction.hpp"

extern NppPlugin::SwitcherPlugin g_plugin;

namespace NppPlugin
{

void fun_switchFile()
{
    g_plugin.switchFile();
}

static ShortcutKey switchFileSk = {false, false, false, VK_F9};

SwitcherPlugin::SwitcherPlugin()
 : m_isInitialized(false)
{
}

/**
* Initialize your plugin data here
* It will be called while plugin loading   
*/
void SwitcherPlugin::init(HINSTANCE p_hModule)
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
void SwitcherPlugin::cleanup()
{
}

/**
* Initialization of your plugin commands
*/
void SwitcherPlugin::commandMenuInit(NppData p_nppData)
{
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
    initFunctionsTable();
}

void SwitcherPlugin::create()
{
}

void SwitcherPlugin::initFunctionsTable()
{
	setCommand(0, TEXT("Header <-> Source"), fun_switchFile, &switchFileSk);
}

/**
* Clean up your plugin commands allocation (if any)
*/
void SwitcherPlugin::commandMenuCleanUp()
{
}

bool SwitcherPlugin::setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (index >= SwitcherPlugin::s_funcNum)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(m_funcItems[index]._itemName, cmdName);
    m_funcItems[index]._pFunc = pFunc;
    m_funcItems[index]._init2Check = false;
    m_funcItems[index]._pShKey = sk;

    return true;
}

void SwitcherPlugin::switchFile()
{
	HeaderSourceSwitcher::switchFile(m_npp);
}

} // namespace NppPlugin
