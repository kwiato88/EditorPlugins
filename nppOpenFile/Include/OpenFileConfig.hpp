#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

class Config
{
public:
	void load(const std::string& p_filePath);
	void save(const std::string& p_filePath);
	bool shouldUseCache() const;

private:
	boost::property_tree::ptree options;
	const std::string useCacheOption = "OpenFile.useCache";
};
