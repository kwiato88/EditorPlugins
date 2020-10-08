#pragma once

#include <string>
#include <vector>
#include "Commands.hpp"
#include "Results.hpp"
#include "ExternalCommand.hpp"

namespace CTagsPlugin
{

class ExternalCommand
{
public:
    ExternalCommand(HWND p_npp, const std::string& p_source, const std::string& p_target = "nppCTagPlugin.dll");

	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	void generateTags(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths);
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	void setTagFiles(const std::vector<std::string>& p_filesPaths);
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
	std::vector<std::string> getTagFiles();
	/**
	* @throws NppPlugin::ExternalCommandFailure
	*/
    Result::Test testCommand(const Command::Test& p_com);

private:
    NppPlugin::ExternalCommand<Messaging::Codec> cmd;
};

}
