#include <iostream>
#include <stdexcept>

#include "Project.hpp"

namespace ProjectMgmt
{

namespace
{
std::vector<Elem> getProjcetItems(const boost::property_tree::ptree& p_data, ITags& p_tags)
{
    std::vector<Elem> items;
    for(const auto& item : p_data.get_child("items"))
        items.push_back(Elem(item.second, p_tags));
    return items;
}
bool enbleStatetoBool(const std::string& p_value)
{
	return p_value == "enabled";
}
}

Elem::Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath)
	: Elem(p_sourcePath, p_ctagsFilePath, g_noTags)
{}

Elem::Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath, ITags& p_tags,
	bool p_genTags, bool p_tagsNavigation)
 : sourcePath(p_sourcePath), ctagsFilePath(p_ctagsFilePath), tags(p_tags),
	shouldGenerateTags(p_genTags), shouldIncludeTagsInNavigation(p_tagsNavigation)
{
    if(sourcePath.empty() && ctagsFilePath.empty())
        throw std::runtime_error("Given source path and tag file path is empty");
}

Elem::Elem(const boost::property_tree::ptree& p_data)
	: Elem(p_data, g_noTags)
{}

Elem::Elem(const boost::property_tree::ptree& p_data, ITags& p_tags)
	: Elem(p_data.get<std::string>("sourcePath", ""),
		p_data.get<std::string>("tagFilePath", ""),
		p_tags,
		enbleStatetoBool(p_data.get<std::string>("tagsGeneration", "enabled")),
		enbleStatetoBool(p_data.get<std::string>("tagsNavigation", "enabled")))
{}

void Elem::refresh()
{
	if(!ctagsFilePath.empty() && !sourcePath.empty() && shouldGenerateTags)
		tags.generateTags(ctagsFilePath, { sourcePath });
}

void Elem::appendNavigationTagFile(std::vector<std::string>& p_tagFiles) const
{
	if (!ctagsFilePath.empty() && shouldIncludeTagsInNavigation)
		p_tagFiles.push_back(ctagsFilePath);
}

boost::property_tree::ptree Elem::exportData() const
{
    boost::property_tree::ptree data;
    data.put("sourcePath", sourcePath);
    data.put("tagFilePath", ctagsFilePath);
	data.put("tagsGeneration", shouldGenerateTags ? "enabled" : "disabled");
	data.put("tagsNavigation", shouldIncludeTagsInNavigation ? "enabled" : "disabled");
	//data.put("includesBrowsing", "enabled");
	//data.put("fileSearching", "enabled");
    return data;
}

bool Elem::operator==(const Elem& p_other) const
{
	return sourcePath == p_other.sourcePath && ctagsFilePath == p_other.ctagsFilePath
		&& shouldGenerateTags == p_other.shouldGenerateTags
		&& shouldIncludeTagsInNavigation == p_other.shouldIncludeTagsInNavigation;
}

Project::~Project()
{}

Project::Project(const std::string& p_name, const std::vector<Elem>& p_items)
	: Project(p_name, p_items, g_noTags)
{}

Project::Project(const std::string& p_name, const std::vector<Elem>& p_items, ITags& p_tags)
 : name(p_name), items(p_items), tags(p_tags)
{
	if (name.empty())
		throw std::runtime_error("Given project name is empty");
}

Project::Project(const boost::property_tree::ptree& p_data)
	: Project(p_data, g_noTags)
{}

Project::Project(const boost::property_tree::ptree& p_data, ITags& p_tags)
	: Project(p_data.get<std::string>("name", ""), getProjcetItems(p_data, p_tags), p_tags)
{}

void Project::refresh()
{
    for(auto& item : items)
        item.refresh();
}

void Project::refershCodeNavigation()
{
	std::vector<std::string> tagFiles;
	for (const auto& item : items)
		item.appendNavigationTagFile(tagFiles);
	tags.setTagFiles(tagFiles);
}

std::string Project::getName() const
{
    return name;
}

boost::property_tree::ptree Project::exportData() const
{
    boost::property_tree::ptree itemsData;
    for(const auto& item : items)
        itemsData.push_back(std::make_pair("", item.exportData()));
    boost::property_tree::ptree projectData;
    projectData.put("name", name);
    projectData.add_child("items", itemsData);
    return projectData;
}

bool Project::operator==(const Project& p_other) const
{
	return name == p_other.name && items == p_other.items;
}
 
}
