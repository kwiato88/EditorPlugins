#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

namespace ProjectMgmt
{

class Elem
{
public:
    Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath);
    Elem(const boost::property_tree::ptree& p_data);
    void refresh();
    boost::property_tree::ptree exportData() const;
	bool operator==(const Elem& p_other) const;

    std::string sourcePath;
    std::string ctagsFilePath;
};

class Project
{
public:
	Project(const std::string& p_name, const std::vector<Elem>& p_items);
	Project(const boost::property_tree::ptree& p_data);
	boost::property_tree::ptree exportData() const;
	void refresh();
	std::string getName() const;
	bool operator==(const Project& p_other) const;
private:
	std::string name;
	std::vector<Elem> items;
};

}
