#pragma once

#include "Commands.hpp"
#include "Results.hpp"
#include "ExternalCommand.hpp"

namespace IncludeBrowser
{

class ExternalCommand
{
public:
    ExternalCommand(HWND p_npp, const std::string& p_source, const std::string& p_target = "nppIncludeBrowser.dll");

    Result::Basic invoke(const Command::Parse& p_com);
    Result::Basic invoke(const Command::Clear& p_com);

private:
    NppPlugin::ExternalCommand cmd;
};

}
