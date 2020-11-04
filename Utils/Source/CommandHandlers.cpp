#include "CommandHandlers.hpp"

namespace Plugin
{
namespace
{
std::string commandName(const std::string& cmd)
{
	auto nameEnd = cmd.find(' ');
	return cmd.substr(0, nameEnd);
}
std::string commandName(const char* const argv[])
{
	return argv[0];
}
}
void Commands::add(const std::string& cmdName, std::unique_ptr<CommandHandler> cmd)
{
	commands.emplace(cmdName, std::move(cmd));
}

std::string Commands::run(const std::string& cmd)
{
	auto found = commands.find(commandName(cmd));
	if(found == commands.end())
		return "";
	return found->second->run(cmd);
}
std::string Commands::run(int argc, char** argv)
{
	auto found = commands.find(commandName(argv));
	if (found == commands.end())
		return "";
	return found->second->run(argc, argv);
}

}