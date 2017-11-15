#include "IncludeBrowserExternalCommand.hpp"
#include "Commands.hpp"
#include "Results.hpp"

namespace IncludeBrowser
{

ExternalCommand::ExternalCommand(
    HWND p_npp,
    const std::string& p_source, const std::string& p_target)
    : cmd(p_npp, p_target, p_source)
{}

void ExternalCommand::parse(const std::string& p_dirPath)
{
	Command::Parse command;
	command.dirPath = p_dirPath;
	auto result = cmd.invokeWithResult<Command::Parse, Result::Basic>(command);
	if (result.res == Result::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to parse includes. Received failure from plugin");
}

void ExternalCommand::clear()
{
	auto result = cmd.invokeWithResult<Command::Clear, Result::Basic>(Command::Clear{});
	if (result.res == Result::Result::Failure)
		throw NppPlugin::ExternalCommandFailure("Failed to clear parsed includes. Received failure from plugin");
}

}
