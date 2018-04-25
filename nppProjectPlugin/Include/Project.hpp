#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

#include "ITags.hpp"
#include "IIncludes.hpp"

namespace ProjectMgmt
{

class DisabledTags : public ITags
{
public:
	void generateTags(const std::string&, const std::vector<std::string>&) {}
	void setTagFiles(const std::vector<std::string>&) {}
	std::vector<std::string> getTagFiles() { return {}; }
};

class DisabledIncludes : public IIncludes
{
public:
	void parse(const std::string&) { }
	void clear() {}
};

static DisabledTags g_noTags;
static DisabledIncludes g_noIncludes;

class Elem
{
public:
    Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath);
	Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath,
		ITags& p_tags, IIncludes& p_includes,
		bool p_genTags = true, bool p_tagsNavigation = true, bool p_parseInc = true);
	Elem(const boost::property_tree::ptree& p_data);
	Elem(const boost::property_tree::ptree& p_data, ITags& p_tags, IIncludes& p_includes);

    boost::property_tree::ptree exportData() const;
	bool operator==(const Elem& p_other) const;

	void refresh();
	void appendNavigationTagFile(std::vector<std::string>& p_tagFiles) const;

private:
	ITags& tags;
	IIncludes& includes;

	std::string sourcePath;
	std::string ctagsFilePath;
	bool shouldGenerateTags;
	bool shouldIncludeTagsInNavigation;
	bool shouldParseIncludes;
};

class Project
{
public:
	virtual ~Project();
	Project(const std::string& p_name, const std::vector<Elem>& p_items);
	Project(const std::string& p_name, const std::vector<Elem>& p_items, ITags& p_tags, IIncludes& p_includes);
	Project(const boost::property_tree::ptree& p_data);
	Project(const boost::property_tree::ptree& p_data, ITags& p_tags, IIncludes& p_includes);

	boost::property_tree::ptree exportData() const;
	bool operator==(const Project& p_other) const;

	std::string getName() const;
	void refresh();
	void refershCodeNavigation();

private:
	std::string name;
	std::vector<Elem> items;
	ITags& tags;
	IIncludes& includes;
};

}
