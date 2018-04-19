#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>

#include "PluginInterface.h"

#include "IncludeBrowserController.hpp"
#include "WinApiTypes.hpp"
#include "NppEditor.hpp"
#include "WinApiUI.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Include Browser");

class IncludeBrowserPlugin
{
public:
	IncludeBrowserPlugin();

	/**
	 * plugin interface
	 */
	void init(HINSTANCE p_hModule);
	void cleanup();
	void commandMenuInit(NppData p_nppData);
	void commandMenuCleanUp();
	void handleMsgToPlugin(CommunicationInfo& p_message);

	/**
	 * plugin menu functions
	 */
	void parse();
    void showIncluded();
    void showIncluders();
    void clear();

	static const int s_funcNum = 4;

	bool m_isInitialized;
	WinApi::InstanceHandle m_hModule;
	FuncItem m_funcItems[s_funcNum];
	Editor m_npp;

private:
	void create();
	bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk);
	void initFunctionsTable();

	WinApi::UI m_ui;
    std::shared_ptr<IncludeBrowser::Controller> m_includeBrowser;
};

} // namespace NppPlugin

#endif // PLUGIN
