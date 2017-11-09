#include "CTagsExternalCommand.hpp"

namespace CTagsPlugin
{

ExternalCommand::ExternalCommand(
    HWND p_npp,
    const std::string& p_source, const std::string& p_target = "nppCtagPlugin.dll")
    : cmd(p_npp, p_target, p_source)
{}

Result::Basic ExternalCommand::invoke(const Command::GenerateTags& p_com)
{
    return cmd.invokeWithResult(p_com);
}

Result::Basic ExternalCommand::invoke(const Command::SetTagFiles& p_com)
{
    return cmd.invokeWithResult(p_com);
}

Result::TagFiles ExternalCommand::invoke(const Command::GetTagFiles& p_com)
{
    return cmd.invokeWithResult(p_com);
}

Result::Test ExternalCommand::invoke(const Command::Test& p_com)
{
    return cmd.invokeWithResult(p_com);
}

}
