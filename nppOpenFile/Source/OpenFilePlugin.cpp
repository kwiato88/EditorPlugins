#include <memory>
#include <fstream>

#include "OpenFilePlugin.hpp"
#include "OpenFileDialog.hpp"
#include "findFile.hpp"

extern NppPlugin::OpenFilePlugin g_plugin;

namespace NppPlugin
{

void fun_open()
{
    g_plugin.open();
}

void fun_setDirs()
{
    g_plugin.setDirsSafe();
}

//static ShortcutKey switchFileSk = {false, false, false, VK_F9};

namespace
{
std::string getFileDir(std::string p_filePath)
{
    return p_filePath.substr(0, p_filePath.find_last_of("\\"));
}
}

OpenFilePlugin::OpenFilePlugin()
	: files(npp.npp, hModule)
{}

void OpenFilePlugin::init()
{
	handlers.addHandler<OpenFileCommand::SetSearchDirs, OpenFileResult::Basic>(
		OpenFileCommand::SetSearchDirs::Id(),
		[&](const auto& p) {return handleSetSearchDirs(p); });
	handlers.addHandler<OpenFileCommand::GetSearchDirs, OpenFileResult::SearchDirs>(
		OpenFileCommand::GetSearchDirs::Id(),
		[&](const auto& p) {return handleGetSearchDirs(p); });
	handlers.addHandler<OpenFileCommand::FindFiles, OpenFileResult::Files>(
		OpenFileCommand::FindFiles::Id(),
		[&](const auto& p) {return handleFindFiles(p); });
	handlers.addHandler<OpenFileCommand::ClearCache, OpenFileResult::Basic>(
		OpenFileCommand::ClearCache::Id(),
		[&](const auto& p) {return handleClearCache(p); });
}

void OpenFilePlugin::detach()
{
	config.save(configFilePath);
}

void OpenFilePlugin::onNppHandleSet()
{
	configFilePath = getPluginsConfigDir() + "\\nppOpenFilePlugin.ini";
	config.load(configFilePath);
	if (config.shouldUseCache())
		searchDirs.withCache();
	else
		searchDirs.withoutCache();
}

void OpenFilePlugin::initMenu()
{
	setCommand(TEXT("Open File"),       fun_open,   NULL);
    setCommand(TEXT("Set search dirs"), fun_setDirs, NULL);
}

void OpenFilePlugin::open()
{
    try
    {
        openFile();
    }
    catch(const std::exception& e)
    {
		ui.errorMessage("Open File", e.what());
    }
}

void OpenFilePlugin::openFile()
{
	ensureSearchDirsAreNotEmpty();
    WinApi::OpenFileDialog dialog(hModule, npp.npp, searchDirs);
    int result = dialog.show();
    if(result == WinApi::Dialog::RESULT_OK)
    {
        npp.setFile(dialog.getSelectedFile());
    }
}

void OpenFilePlugin::ensureSearchDirsAreNotEmpty()
{
	if (searchDirs.getDirsPaths().empty())
		setDirs();
}

void OpenFilePlugin::setDirs()
{
	auto oldPaths = searchDirs.getDirsPaths();
	auto newPaths = files.selectDirsPaths(std::vector<std::string>(oldPaths.begin(), oldPaths.end()), getFileDir(npp.getFile()));
	if (newPaths.empty())
		throw std::runtime_error("No search dirs selected");
	searchDirs.applyDirs({});
	searchDirs.applyDirs(std::set<std::string>(newPaths.begin(), newPaths.end()));
}

void OpenFilePlugin::setDirsSafe()
{
	try
	{
		setDirs();
	}
	catch (const std::exception& e)
	{
		ui.errorMessage("Set dir", e.what());
	}
}

OpenFileResult::Basic OpenFilePlugin::handleSetSearchDirs(const OpenFileCommand::SetSearchDirs& p_cmd)
{
	searchDirs.applyDirs({});
	searchDirs.applyDirs(std::set<std::string>(p_cmd.dirsPaths.begin(), p_cmd.dirsPaths.end()));
	return OpenFileResult::Basic{ OpenFileResult::Result::Success };
}
OpenFileResult::SearchDirs  OpenFilePlugin::handleGetSearchDirs(const OpenFileCommand::GetSearchDirs&)
{
	auto dirs = searchDirs.getDirsPaths();
	return OpenFileResult::SearchDirs{ std::vector<std::string>(dirs.begin(), dirs.end()) };
}
OpenFileResult::Basic OpenFilePlugin::handleClearCache(const OpenFileCommand::ClearCache&)
{
	auto paths = searchDirs.getDirsPaths();
	searchDirs.applyDirs({});
	searchDirs.applyDirs(paths);
	return OpenFileResult::Basic{ OpenFileResult::Result::Success };
}
OpenFileResult::Files OpenFilePlugin::handleFindFiles(const OpenFileCommand::FindFiles& p_cmd)
{
	try
	{
		if (!p_cmd.dirsPaths.empty())
			searchDirs.applyDirs(std::set<std::string>(p_cmd.dirsPaths.begin(), p_cmd.dirsPaths.end()));
		auto files = searchDirs.getFiles(Dirs::Pattern{ p_cmd.fileNamePattern, p_cmd.caseSensitiveSearch, true });
		std::vector<std::string> foundFiles;
		std::transform(files.begin(), files.end(), std::back_inserter(foundFiles),
			[&](const auto& f) { return f.string(); });

		return OpenFileResult::Files{ OpenFileResult::Result::Success, foundFiles };
	}
	catch (std::exception&)
	{
		return OpenFileResult::Files{ OpenFileResult::Result::Failure, {} };
	}
}

void OpenFilePlugin::handleMsgToPlugin(CommunicationInfo& p_message)
{
	try
	{
		handlers.handle(p_message.internalMsg, *static_cast<Messaging::Transaction*>(p_message.info));
	}
	catch (std::exception& e)
	{
		Messaging::Transaction* transaction = static_cast<Messaging::Transaction*>(p_message.info);
		transaction->result.size = 0;
		ui.errorMessage("Open File", std::string("Error during message to plugin handling: ") + e.what());
	}
	
}

} // namespace NppPlugin
