#include "CTagsExternalCommand.hpp"
#include "Commands.hpp"
#include "Results.hpp"

namespace CTagsPlugin
{

ExternalCommand::ExternalCommand(
    HWND p_npp,
    const std::string& p_source, const std::string& p_target)
    : cmd(p_npp, p_target, p_source)
{}

void ExternalCommand::generateTags(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths)
{
	Command::GenerateTags gen;
	gen.tagFilePath = p_tagFilePath;
	gen.sourceDirsPaths = p_sourceDirsPaths;
	auto result = cmd.invokeWithResult<Command::GenerateTags, Result::Basic>(gen);
	if (result.res == Result::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to generate tag file. Received failure from plugin");
}

void ExternalCommand::setTagFiles(const std::vector<std::string>& p_filesPaths)
{
	Command::SetTagFiles command;
	command.filesPaths = p_filesPaths;
	auto result = cmd.invokeWithResult<Command::SetTagFiles, Result::Basic>(command);
	if(result.res == Result::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to set tag fils. Received failure from plugin");
}

std::vector<std::string> ExternalCommand::getTagFiles()
{
	return cmd.invokeWithResult<Command::GetTagFiles, Result::TagFiles>(Command::GetTagFiles{}).filesPaths;
}

Result::Test ExternalCommand::testCommand(const Command::Test& p_com)
{
    return cmd.invokeWithResult<Command::Test, Result::Test>(p_com);
}

}
