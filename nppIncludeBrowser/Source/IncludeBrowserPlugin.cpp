#include <memory>
#include <fstream>

#include "menuCmdID.h"

#include "IncludeBrowserPlugin.hpp"
#include "NppLocationGetter.hpp"
#include "NppLocationSetter.hpp"
#include "NppPathGetter.hpp"
#include "NppListViewSelector.hpp"
#include "NppMessagePrinter.hpp"

extern NppPlugin::IncludeBrowserPlugin g_plugin;

namespace NppPlugin
{

void fun_parse()
{
    g_plugin.parse();
}

void fun_showIncluders()
{
    g_plugin.showIncluders();
}

void fun_showIncluded()
{
    g_plugin.showIncluded();
}

void fun_clear()
{
    g_plugin.clear();
}

IncludeBrowserPlugin::IncludeBrowserPlugin()
 : m_isInitialized(false)
{
}

/**
* Initialize your plugin data here
* It will be called while plugin loading   
*/
void IncludeBrowserPlugin::init(HINSTANCE p_hModule)
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
void IncludeBrowserPlugin::cleanup()
{
}

/**
* Initialization of your plugin commands
*/
void IncludeBrowserPlugin::commandMenuInit(NppData p_nppData)
{
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
    initFunctionsTable();
}

void IncludeBrowserPlugin::create()
{
    m_includeBrowser.reset(new IncludeBrowser::Controller(
        std::make_shared<NppPlugin::NppLocationGetter>(m_npp),
        std::make_shared<NppPlugin::NppLocationSetter>(m_npp),
        std::make_shared<NppPlugin::NppPathGetter>(m_npp.npp, m_hModule),
        std::make_shared<NppPlugin::NppListViewSelector>(m_npp.npp, m_hModule),
        std::make_shared<NppPlugin::NppMessagePrinter>(m_npp.npp, m_hModule)));
}

void IncludeBrowserPlugin::initFunctionsTable()
{
    int i=0;
	setCommand(i++, TEXT("Parse"),          fun_parse,         NULL);
    setCommand(i++, TEXT("Show included"),  fun_showIncluded,  NULL);
    setCommand(i++, TEXT("Show includers"), fun_showIncluders, NULL);
    setCommand(i++, TEXT("Clear"),          fun_clear,         NULL);
}

/**
* Clean up your plugin commands allocation (if any)
*/
void IncludeBrowserPlugin::commandMenuCleanUp()
{
}

bool IncludeBrowserPlugin::setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (index >= IncludeBrowserPlugin::s_funcNum)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(m_funcItems[index]._itemName, cmdName);
    m_funcItems[index]._pFunc = pFunc;
    m_funcItems[index]._init2Check = false;
    m_funcItems[index]._pShKey = sk;

    return true;
}

void IncludeBrowserPlugin::parse()
{
    m_includeBrowser->parse();
}

void IncludeBrowserPlugin::showIncluded()
{
    m_includeBrowser->showIncluded();
}

void IncludeBrowserPlugin::showIncluders()
{
    m_includeBrowser->showIncluders();
}

void IncludeBrowserPlugin::clear()
{
    m_includeBrowser->clear();
}

} // namespace NppPlugin