#include "OpenFileExternalCommand.hpp"
#include "Commands.hpp"
#include "Results.hpp"

namespace NppPlugin
{

OpenFileExternalCommand::OpenFileExternalCommand(
    HWND p_npp,
    const std::string& p_source, const std::string& p_target)
    : cmd(p_npp, p_target, p_source)
{}

void OpenFileExternalCommand::setSearchDirs(const std::vector<std::string>& p_dirPaths)
{
	OpenFileCommand::SetSearchDirs command = {};
	command.dirsPaths = p_dirPaths;
	auto result = cmd.invokeWithResult<OpenFileCommand::SetSearchDirs, OpenFileResult::Basic>(command);
	if (result.res == OpenFileResult::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to set search dirs. Received failure from plugin");
}

std::vector<std::string> OpenFileExternalCommand::getSearchDirs()
{
	OpenFileCommand::GetSearchDirs command = {};
	auto result = cmd.invokeWithResult<OpenFileCommand::GetSearchDirs, OpenFileResult::SearchDirs>(command);
	return result.paths;
}

void OpenFileExternalCommand::clearCache()
{
	OpenFileCommand::ClearCache command = {};
	auto result = cmd.invokeWithResult<OpenFileCommand::ClearCache, OpenFileResult::Basic>(command);
	if (result.res == OpenFileResult::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to set search dirs. Received failure from plugin");
}

std::vector<std::string> OpenFileExternalCommand::findFiles(
	const std::vector<std::string>& p_serachDirs,
	const std::string& p_fileNamePatter, bool p_caseSesitive)
{
	OpenFileCommand::FindFiles command = {};
	command.dirsPaths = p_serachDirs;
	command.fileNamePattern = p_fileNamePatter;
	command.caseSensitiveSearch = p_caseSesitive;
	auto result = cmd.invokeWithResult<OpenFileCommand::FindFiles, OpenFileResult::Files>(command);
	if (result.res == OpenFileResult::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to find files. Received failure from plugin");
	return result.paths;
}

}
