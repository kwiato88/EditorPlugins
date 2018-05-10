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
    WinApi::OpenFileDialog dialog(hModule, npp.npp);
    dialog.setSearchDirs(getSerachDirs());
    int result = dialog.show();
    if(result == WinApi::Dialog::RESULT_OK)
    {
        npp.setFile(dialog.getSelectedFile());
    }
}

std::vector<std::string> OpenFilePlugin::getSerachDirs()
{
	if (m_searchDirs.empty())
		setDirs();
	if (m_searchDirs.empty())
		throw std::runtime_error("No search dirs selected");
	return m_searchDirs;
}

void OpenFilePlugin::setDirs()
{
	m_searchDirs = files.selectDirsPaths(m_searchDirs, getFileDir(npp.getFile()));
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
	m_searchDirs = p_cmd.dirsPaths;
	return OpenFileResult::Basic{ OpenFileResult::Result::Success };
}
OpenFileResult::SearchDirs  OpenFilePlugin::handleGetSearchDirs(const OpenFileCommand::GetSearchDirs&)
{
	return OpenFileResult::SearchDirs{ m_searchDirs };
}
OpenFileResult::Files OpenFilePlugin::handleFindFiles(const OpenFileCommand::FindFiles& p_cmd)
{
	try
	{
		std::vector<std::string> searchDirs = p_cmd.dirsPaths.empty() ? m_searchDirs : p_cmd.dirsPaths;
		std::vector<std::string> foundFiles;

		for (const auto& dir : searchDirs)
		{
			auto files = findFiles(p_cmd.fileNamePattern, dir, p_cmd.caseSensitiveSearch, true);
			std::transform(files.begin(), files.end(), std::back_inserter(foundFiles),
				[&](const auto& f) { return f.string(); });
		}
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
