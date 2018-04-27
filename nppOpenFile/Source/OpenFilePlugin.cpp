#include <memory>
#include <fstream>

#include "menuCmdID.h"
#include "OpenFilePlugin.hpp"
#include "OpenFileDialog.hpp"
#include "NppPathsSelector.hpp"
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
 : m_isInitialized(false), ui(m_npp.npp, m_hModule)
{
}

/**
* Initialize your plugin data here
* It will be called while plugin loading   
*/
void OpenFilePlugin::init(HINSTANCE p_hModule)
{
    if(!m_isInitialized)
    {
		m_isInitialized = true;
        m_hModule = p_hModule;
        create();
    }
}

/**
* Cleaning of your plugin
* It will be called while plugin unloading
*/
void OpenFilePlugin::cleanup()
{
}

/**
* Initialization of your plugin commands
*/
void OpenFilePlugin::commandMenuInit(NppData p_nppData)
{
	m_npp.npp = WinApi::Handle(p_nppData._nppHandle);
	m_npp.scintillaMain = WinApi::Handle(p_nppData._scintillaMainHandle);
	m_npp.scintillaSecond = WinApi::Handle(p_nppData._scintillaSecondHandle);
    initFunctionsTable();
}

void OpenFilePlugin::create()
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

void OpenFilePlugin::initFunctionsTable()
{
	setCommand(0, TEXT("Open File"),       fun_open,   NULL);
    setCommand(1, TEXT("Set search dirs"), fun_setDirs, NULL);
}

/**
* Clean up your plugin commands allocation (if any)
*/
void OpenFilePlugin::commandMenuCleanUp()
{
}

bool OpenFilePlugin::setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk)
{
	if (index >= OpenFilePlugin::s_funcNum)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(m_funcItems[index]._itemName, cmdName);
    m_funcItems[index]._pFunc = pFunc;
    m_funcItems[index]._init2Check = false;
    m_funcItems[index]._pShKey = sk;

    return true;
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
    WinApi::OpenFileDialog dialog(m_hModule, m_npp.npp);
    dialog.setSearchDirs(getSerachDirs());
    int result = dialog.show();
    if(result == WinApi::Dialog::RESULT_OK)
    {
        m_npp.setFile(dialog.getSelectedFile());
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
	PathsSelector<SelectorType::Directory> dirs(m_npp.npp, m_hModule);
	m_searchDirs = dirs.select(m_searchDirs, getFileDir(m_npp.getFile()));
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
			auto files = findFiles(p_cmd.fileNamePattern, dir, p_cmd.caseSensitiveSearch);
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
