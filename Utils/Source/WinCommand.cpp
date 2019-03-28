#include "WinCommand.hpp"
#include "WinApiLastErrorException.hpp"
#include "ShellCommandException.hpp"

namespace Plugin
{

WinCommand::WinCommand(const std::string& p_cmd, const std::string& p_args)
	: cmd(p_cmd, p_args)
{}

std::string WinCommand::execute()
{
	try
	{
		cmd.execute();
		return "";
	}
	catch (WinApi::Exception& e)
	{
		throw ShellCommandException(e.what());
	}
}

WinCommandWithInOut::WinCommandWithInOut(const std::string& p_cmd)
	: cmd(p_cmd)
{}

std::string WinCommandWithInOut::execute()
{
	try
	{
		return cmd.execute();
	}
	catch (WinApi::Exception& e)
	{
		throw ShellCommandException(e.what());
	}
}

}
