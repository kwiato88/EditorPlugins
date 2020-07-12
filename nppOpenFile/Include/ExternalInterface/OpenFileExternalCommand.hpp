#pragma once

#include <string>
#include <vector>
#include "ExternalCommand.hpp"

namespace NppPlugin
{

class OpenFileExternalCommand
{
public:
	OpenFileExternalCommand(HWND p_npp, const std::string& p_source, const std::string& p_target = "nppOpenFile.dll");

	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	void setSearchDirs(const std::vector<std::string>& p_dirPaths);
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	std::vector<std::string> getSearchDirs();
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	std::vector<std::string> findFiles(const std::vector<std::string>& p_serachDirs,
		const std::string& p_fileNamePatter, bool p_caseSesitive);
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	void clearCache();

private:
    NppPlugin::ExternalCommand cmd;
};

}
