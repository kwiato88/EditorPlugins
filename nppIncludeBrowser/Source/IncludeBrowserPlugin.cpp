#include <memory>
#include <fstream>

#include "IncludeBrowserPlugin.hpp"
#include "TreeViewFileHierarchySelector.hpp"
#include "Transaction.hpp"

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
	: files(npp.npp, hModule)
{}

void IncludeBrowserPlugin::init()
{
    includeBrowser.reset(new IncludeBrowser::Controller(
		npp,
		ui,
		files,
        std::make_shared<IncludeBrowser::TreeViewFileHierarchySelector>(hModule, npp.npp)));
}

void IncludeBrowserPlugin::initMenu()
{
	setCommand(TEXT("Parse"),          fun_parse,         NULL);
    setCommand(TEXT("Show included"),  fun_showIncluded,  NULL);
    setCommand(TEXT("Show includers"), fun_showIncluders, NULL);
    setCommand(TEXT("Clear"),          fun_clear,         NULL);
}

void IncludeBrowserPlugin::handleMsgToPlugin(CommunicationInfo& p_message)
{
	try
	{
		includeBrowser->handleTransaction(p_message.internalMsg, *static_cast<Messaging::Transaction*>(p_message.info));
	}
	catch (std::exception& e)
	{
		Messaging::Transaction* transaction = static_cast<Messaging::Transaction*>(p_message.info);
		transaction->result.size = 0;
		ui.errorMessage("Include Brower", std::string("Error during message to plugin handling: ") + e.what());
	}
}

void IncludeBrowserPlugin::parse()
{
    includeBrowser->parse();
}

void IncludeBrowserPlugin::showIncluded()
{
    includeBrowser->showIncluded();
}

void IncludeBrowserPlugin::showIncluders()
{
    includeBrowser->showIncluders();
}

void IncludeBrowserPlugin::clear()
{
    includeBrowser->clear();
}

} // namespace NppPlugin
