#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <string>
#include <vector>
#include <boost\shared_ptr.hpp>

#include "NppPlugin.hpp"

#include "MessageHandler.hpp"
#include "Commands.hpp"
#include "Results.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Open File");

class OpenFilePlugin : public BasePluginWithUI<2>
{
public:
	void open();
	void setDirsSafe();

protected:
	void onInstanceHandleSet() override;
	void initMenu() override;
	void handleMsgToPlugin(CommunicationInfo& p_message) override;
	
private:
	void openFile();
	void setDirs();
	std::vector<std::string> getSerachDirs();
	OpenFileResult::Basic handleSetSearchDirs(const OpenFileCommand::SetSearchDirs&);
	OpenFileResult::SearchDirs  handleGetSearchDirs(const OpenFileCommand::GetSearchDirs&);
	OpenFileResult::Files handleFindFiles(const OpenFileCommand::FindFiles&);

    std::vector<std::string> m_searchDirs;
	Messaging::Handlers handlers;
};

} // namespace NppPlugin

#endif // PLUGIN
