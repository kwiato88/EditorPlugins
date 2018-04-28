#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>

#include "NppPlugin.hpp"

#include "IncludeBrowserController.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Include Browser");

class IncludeBrowserPlugin : public BasePluginWithUI<4>
{
public:
	void parse();
    void showIncluded();
    void showIncluders();
    void clear();

protected:
	void onInstanceHandleSet() override;
	void initMenu() override;
	void handleMsgToPlugin(CommunicationInfo& p_message) override;

private:
    std::shared_ptr<IncludeBrowser::Controller> includeBrowser;
};

} // namespace NppPlugin

#endif // PLUGIN
