#pragma once

#include "Command.hpp"
#include "WinApiShellCommand.hpp"
#include "WinApiProcess.hpp"

namespace Plugin
{

class WinCommand : public Command
{
public:
	WinCommand(const std::string& p_cmd, const std::string& p_args);

	std::string execute() override;

private:
	WinApi::ShellCommand cmd;
};

class WinCommandWithInOut : public Command
{
public:
	WinCommandWithInOut(const std::string& p_cmd);

	std::string execute() override;

private:
	WinApi::Process cmd;
};

}