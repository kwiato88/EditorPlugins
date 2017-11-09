#pragma once

#include "ExternalInterface/Command.hpp"
#include "ExternalInterface/Result.hpp"
#include "ExternalInterface/ExternalCommand.hpp"

namespace CTagsPlugin
{

class ExternalCommand
{
public:
    ExternalCommand(HWND p_npp, const std::string& p_source, const std::string& p_target = "nppCtagPlugin.dll");

    Result::Basic invoke(const Command::GenerateTags& p_com);
    Result::Basic invoke(const Command::SetTagFiles& p_com);
    Result::TagFiles invoke(const Command::GetTagFiles& p_com);
    
    Result::Test invoke(const Command::Test& p_com);

private:
    NppPlugin::ExternalCommand cmd;
};

}
