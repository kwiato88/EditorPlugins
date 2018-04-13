#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

#include "ITags.hpp"

namespace ProjectMgmt
{

class DisabledTags : public ITags
{
public:
	void generateTags(const std::string&, const std::vector<std::string>&) {}
	void setTagFiles(const std::vector<std::string>&) {}
	std::vector<std::string> getTagFiles() { return {}; }
};

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
	virtual ~Project();
	Project(const std::string& p_name, const std::vector<Elem>& p_items);
	Project(const std::string& p_name, const std::vector<Elem>& p_items, ITags& p_tags);
	Project(const boost::property_tree::ptree& p_data);
	Project(const boost::property_tree::ptree& p_data, ITags& p_tags);
	boost::property_tree::ptree exportData() const;
	void refresh();
	void refershCodeNavigation();
	std::string getName() const;
	bool operator==(const Project& p_other) const;
private:
	std::string name;
	std::vector<Elem> items;
	ITags& tags;
	DisabledTags noTags;
};

}
