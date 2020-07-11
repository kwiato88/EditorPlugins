#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <string>
#include <vector>
#include <boost\shared_ptr.hpp>

#include "NppPlugin.hpp"

#include "WinApiUIFileSystem.hpp"
#include "MessageHandler.hpp"
#include "Commands.hpp"
#include "Results.hpp"
#include "findFile.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Open File");

class OpenFilePlugin : public BasePluginWithUI<2>
{
public:
	OpenFilePlugin();

	void open();
	void setDirsSafe();

protected:
	void init() override;
	void initMenu() override;
	void handleMsgToPlugin(CommunicationInfo& p_message) override;
	
private:
	void openFile();
	void setDirs();
	std::vector<std::string> getSerachDirs();
	OpenFileResult::Basic handleSetSearchDirs(const OpenFileCommand::SetSearchDirs&);
	OpenFileResult::SearchDirs  handleGetSearchDirs(const OpenFileCommand::GetSearchDirs&);
	OpenFileResult::Files handleFindFiles(const OpenFileCommand::FindFiles&);

	WinApi::UIFileSystem files;
	Dirs searchDirs;
    std::vector<std::string> searchDirsPaths;
	Messaging::Handlers handlers;
};

} // namespace NppPlugin

#endif // PLUGIN
