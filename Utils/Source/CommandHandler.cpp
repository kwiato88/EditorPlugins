#include <sstream>
#include <vector>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include "CommandHandler.hpp"

namespace po = boost::program_options;

namespace Plugin
{
namespace
{
std::vector<std::string> tokenize(const std::string& cmd)
{
	using SeparatorType = boost::escaped_list_separator<char>;
	SeparatorType separator("\\", "= ", "\"\'");
	boost::tokenizer<SeparatorType> tokens(cmd, separator);
	std::vector<std::string> args;
	std::copy_if(tokens.begin(), tokens.end(), std::back_inserter(args), [](const auto& s) { return !s.empty(); });

	return args;
}
}

CommandHandler::CommandHandler(const std::string& cmdName)
	: descrition(cmdName)
{
	descrition.add_options()("help,h", "print help");
}

std::string CommandHandler::run(const std::string& cmd)
{
	boost::program_options::variables_map variables;
	po::store(po::command_line_parser(tokenize(cmd)).options(descrition).run(), variables);
	po::notify(variables);

	return runWithOutput(variables);
}

std::string CommandHandler::run(int argc, char** argv)
{
	boost::program_options::variables_map variables;
	po::store(po::parse_command_line(argc, argv, descrition), variables);
	po::notify(variables);
	
	return runWithOutput(variables);
}

std::string CommandHandler::runWithOutput(const boost::program_options::variables_map& params)
{
	output = std::stringstream();
	if (params.count("help"))
		printHelp();
	else
		run(params);

	return output.str();
}

void CommandHandler::printHelp()
{
	output << "Usage:\n" << descrition << "\n";
}

void CommandHandler::run(const boost::program_options::variables_map&)
{
}

}
