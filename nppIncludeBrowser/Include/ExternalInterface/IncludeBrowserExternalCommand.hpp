#pragma once

#include <string>
#include "ExternalCommand.hpp"

namespace IncludeBrowser
{

class ExternalCommand
{
public:
    ExternalCommand(HWND p_npp, const std::string& p_source, const std::string& p_target = "nppIncludeBrowser.dll");

	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	void parse(const std::string& p_dirPath);
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	void clear();

private:
    NppPlugin::ExternalCommand<Messaging::Codec> cmd;
};

}
