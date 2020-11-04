#pragma once

#include <memory>
#include <map>
#include "CommandHandler.hpp"

namespace Plugin
{

class Commands
{
public:
	void add(const std::string& cmdName, std::unique_ptr<CommandHandler> cmd);
	std::string run(const std::string& cmd);
	std::string run(int argc, char** argv);
private:
	std::map<std::string, std::unique_ptr<CommandHandler>> commands;
};

}
