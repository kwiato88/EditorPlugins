#pragma once

#include <sstream>
#include "boost/program_options.hpp"

namespace Plugin
{

class CommandHandler
{
public:
	CommandHandler(const std::string& cmdName);

	std::string run(const std::string& cmd);
	std::string run(int argc, char** argv);
	virtual void run(const boost::program_options::variables_map& params);
protected:
	void printHelp();
	std::string runWithOutput(const boost::program_options::variables_map& params);

	std::stringstream output;
	boost::program_options::options_description descrition;
};

}
