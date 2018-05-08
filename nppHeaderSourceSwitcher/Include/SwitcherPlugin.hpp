#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <boost\shared_ptr.hpp>

#include "NppPlugin.hpp"
#include "SwitcherFunction.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Header Source Switcher");

class SwitcherPlugin : public BasePluginWithUI<1>
{
public:
	SwitcherPlugin();

	void switchFile();

protected:
	void initMenu() override;
	void onShoutdown() override;
	void onNppHandleSet() override;

	HeaderSourceSwitcher::Switcher switcher;
	std::string configFilePath;
};

} // namespace NppPlugin

#endif // PLUGIN
