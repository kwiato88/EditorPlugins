#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <boost\shared_ptr.hpp>

#include "PluginInterface.h"

#include "ILocationSetter.hpp"
#include "ILocationGetter.hpp"
#include "WinApiTypes.hpp"
#include "EditorData.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Header Source Switcher");

class SwitcherPlugin
{
public:
	SwitcherPlugin();

	/**
	 * plugin interface
	 */
	void init(HINSTANCE p_hModule);
	void cleanup();
	void commandMenuInit(NppData p_nppData);
	void commandMenuCleanUp();

	/**
	 * plugin menu functions
	 */
	void switchFile();

	static const int s_funcNum = 1;

	bool m_isInitialized;
	WinApi::InstanceHandle m_hModule;
	FuncItem m_funcItems[s_funcNum];
	EditorData m_npp;

private:
	void create();
	bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk);
	void initFunctionsTable();

	boost::shared_ptr<Plugin::ILocationSetter> m_locationSetter;
	boost::shared_ptr<Plugin::ILocationGetter> m_locationGetter;

};

} // namespace NppPlugin

#endif // PLUGIN
