#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <boost\shared_ptr.hpp>

#include "NppPlugin.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Header Source Switcher");

class SwitcherPlugin : public BasePlugin<1>
{
public:
	void switchFile();

protected:
	void initMenu() override;
};

} // namespace NppPlugin

#endif // PLUGIN
