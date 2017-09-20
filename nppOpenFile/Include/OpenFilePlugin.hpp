#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <boost\shared_ptr.hpp>

#include "PluginInterface.h"
#include "WinApiTypes.hpp"
#include "EditorData.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Open File");

class OpenFilePlugin
{
public:
	OpenFilePlugin();

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
	void open();
    void openFile();
    void setDir();

	static const int s_funcNum = 2;

	bool m_isInitialized;
	WinApi::InstanceHandle m_hModule;
	FuncItem m_funcItems[s_funcNum];
	EditorData m_npp;

private:
	void create();
	bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk);
	void initFunctionsTable();
	std::string getSerachDir();

	// boost::shared_ptr<Plugin::ILocationSetter> m_locationSetter;
	// boost::shared_ptr<Plugin::ILocationGetter> m_locationGetter;
    std::string m_searchDir;

};

} // namespace NppPlugin

#endif // PLUGIN
