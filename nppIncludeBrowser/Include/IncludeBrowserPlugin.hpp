#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>

#include "NppPlugin.hpp"

#include "IncludeBrowserController.hpp"
#include "WinApiUIFileSystem.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Include Browser");

class IncludeBrowserPlugin : public BasePluginWithUI<4>
{
public:
	IncludeBrowserPlugin();

	void parse();
    void showIncluded();
    void showIncluders();
    void clear();

protected:
	void init() override;
	void initMenu() override;
	void handleMsgToPlugin(CommunicationInfo& p_message) override;

private:
	WinApi::UIFileSystem files;
    std::shared_ptr<IncludeBrowser::Controller> includeBrowser;
};

} // namespace NppPlugin

#endif // PLUGIN
