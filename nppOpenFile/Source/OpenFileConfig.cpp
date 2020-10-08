#include "OpenFileConfig.hpp"
#include <boost/property_tree/ini_parser.hpp>

void Config::load(const std::string& p_filePath)
{
	try
	{
		boost::property_tree::ini_parser::read_ini(p_filePath, options);
	}
	catch (boost::property_tree::ini_parser::ini_parser_error& e)
	{
	}
	auto useCache = options.get_optional<std::string>(useCacheOption);
	if (!useCache.is_initialized())
		options.put(useCacheOption, "true");
}

void Config::save(const std::string& p_filePath)
{
	try
	{
		boost::property_tree::ini_parser::write_ini(p_filePath, options);
	}
	catch (boost::property_tree::ini_parser::ini_parser_error& e)
	{
	}
}

bool Config::shouldUseCache() const
{
	return options.get<std::string>(useCacheOption, "true") == "true";
}
