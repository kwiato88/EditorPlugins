#include "Logger.hpp"

#ifndef NO_LOG
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#endif

namespace
{
#ifndef NO_LOG
boost::log::trivial::severity_level toBoostLogLevel(Logger::Level p_logLevel)
{
	switch (p_logLevel)
	{
		case Logger::Level::debug: return boost::log::trivial::debug;
		case Logger::Level::info: return boost::log::trivial::info;
		case Logger::Level::warning: return boost::log::trivial::warning;
		case Logger::Level::error: return boost::log::trivial::error;
	}
}
#endif
}

bool Logger::initialized = false;
bool Logger::enabled = false;

void Logger::enable() { enabled = true; }
void Logger::disable() { enabled = false; }
bool Logger::isEnabled() { return enabled; }

void Logger::init(const std::string& p_logFile)
{
#ifndef NO_LOG
	if (initialized) return;
	boost::log::add_common_attributes();
	boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
	boost::log::add_file_log
	(
		p_logFile,
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::auto_flush = true,
		boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%"
	);
	initialized = true;
#endif
}

void Logger::setLogLevel(Level p_levelSeverityLevel)
{
#ifndef NO_LOG
	auto logLevel = toBoostLogLevel(p_levelSeverityLevel);
	boost::log::core::get()->set_filter
	(
		boost::log::trivial::severity >= logLevel
	);
#endif
}
