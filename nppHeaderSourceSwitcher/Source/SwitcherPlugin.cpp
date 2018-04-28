#include <memory>
#include <fstream>

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

void SwitcherPlugin::initMenu()
{
	setCommand(TEXT("Header <-> Source"), fun_switchFile, &switchFileSk);
}

void SwitcherPlugin::switchFile()
{
	HeaderSourceSwitcher::switchFile(npp);
}

} // namespace NppPlugin
