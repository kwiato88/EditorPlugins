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
#include "OpenFileConfig.hpp"

namespace NppPlugin
{

const TCHAR NPP_PLUGIN_NAME[] = TEXT("Open File");

class OpenFilePlugin : public BasePluginWithUI<2>
{
public:
	OpenFilePlugin();
	void detach() override;

	void open();
	void setDirsSafe();

protected:
	void init() override;
	void initMenu() override;
	void onNppHandleSet() override;
	void handleMsgToPlugin(CommunicationInfo& p_message) override;
	
private:
	void openFile();
	void setDirs();
	void ensureSearchDirsAreNotEmpty();
	OpenFileResult::Basic handleSetSearchDirs(const OpenFileCommand::SetSearchDirs&);
	OpenFileResult::SearchDirs  handleGetSearchDirs(const OpenFileCommand::GetSearchDirs&);
	OpenFileResult::Files handleFindFiles(const OpenFileCommand::FindFiles&);
	OpenFileResult::Basic handleClearCache(const OpenFileCommand::ClearCache&);

	WinApi::UIFileSystem files;
	Dirs searchDirs;
	Messaging::HandlersWithDefaultCodec handlers;
	std::string configFilePath;
	Config config;
};

} // namespace NppPlugin

#endif // PLUGIN
