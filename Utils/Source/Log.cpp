#include <iomanip>
#include <sstream>
#include <boost/log/trivial.hpp>
#include "Log.hpp"

namespace
{
std::string getFileBaseName(const std::string& p_path)
{
	size_t l_pos = p_path.find_last_of('\\');
	if (l_pos != std::string::npos)
		return p_path.substr(l_pos + 1);
	else
		return p_path;
}

std::string getIndent(boost::log::trivial::severity_level p_level)
{
	if (p_level == boost::log::trivial::severity_level::debug) return "  ";
	if (p_level == boost::log::trivial::severity_level::info) return "   ";
	if (p_level == boost::log::trivial::severity_level::warning) return "";
	if (p_level == boost::log::trivial::severity_level::error) return "  ";
	return "";
}
}

std::string getPrefix(boost::log::trivial::severity_level p_level, const std::string& p_file, unsigned int p_line)
{
	std::ostringstream prefix;
	prefix << getIndent(p_level) << "[" << getFileBaseName(p_file) << ":"
		<< std::setfill('0') << std::setw(3) << p_line << "] ";
	return prefix.str();
}
