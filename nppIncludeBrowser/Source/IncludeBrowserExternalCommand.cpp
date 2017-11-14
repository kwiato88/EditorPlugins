#include "IncludeBrowserExternalCommand.hpp"

namespace IncludeBrowser
{

ExternalCommand::ExternalCommand(
    HWND p_npp,
    const std::string& p_source, const std::string& p_target)
    : cmd(p_npp, p_target, p_source)
{}

Result::Basic ExternalCommand::invoke(const Command::Parse& p_com)
{
    return cmd.invokeWithResult<Command::Parse, Result::Basic>(p_com);
}

Result::Basic ExternalCommand::invoke(const Command::Clear& p_com)
{
    return cmd.invokeWithResult<Command::Clear, Result::Basic>(p_com);
}

}
