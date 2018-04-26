#include <iostream>
#include <stdexcept>

#include "Project.hpp"

namespace ProjectMgmt
{

namespace
{
std::vector<Elem> getProjcetItems(const boost::property_tree::ptree& p_data, ITags& p_tags, IIncludes& p_includes)
{
    std::vector<Elem> items;
    for(const auto& item : p_data.get_child("items"))
        items.push_back(Elem(item.second, p_tags, p_includes));
    return items;
}
bool enbleStateToBool(const std::string& p_value)
{
	return p_value == "enabled";
}
std::string toEnbleState(bool p_value)
{
	return p_value ? "enabled" : "disabled";
}
}

Elem::Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath)
	: Elem(p_sourcePath, p_ctagsFilePath, g_noTags, g_noIncludes)
{}

Elem::Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath,
	ITags& p_tags, IIncludes& p_includes,
	bool p_genTags, bool p_tagsNavigation, bool p_parseInc)
 : sourcePath(p_sourcePath), ctagsFilePath(p_ctagsFilePath),
	tags(p_tags), includes(p_includes), currentInc(&g_noIncludes),
	shouldGenerateTags(p_genTags), shouldIncludeTagsInNavigation(p_tagsNavigation)
{
	if (p_parseInc)
		currentInc = &includes;
	if (isIncludesParsingEnabled() && sourcePath.empty())
		throw std::runtime_error("Source for elem with includes navigation can not be empty");
	if (p_tagsNavigation && ctagsFilePath.empty())
		throw std::runtime_error("Tag file for elem with tags navigation can not be empty");
	if(p_genTags && (ctagsFilePath.empty() || sourcePath.empty()))
		throw std::runtime_error("Tag file and source for elem with tags generation can not be empty");
}

Elem::Elem(const boost::property_tree::ptree& p_data)
	: Elem(p_data, g_noTags, g_noIncludes)
{}

Elem::Elem(const boost::property_tree::ptree& p_data, ITags& p_tags, IIncludes& p_includes)
	: Elem(p_data.get<std::string>("sourcePath", ""),
		p_data.get<std::string>("tagFilePath", ""),
		p_tags,
		p_includes,
		enbleStateToBool(p_data.get<std::string>("tagsGeneration", "enabled")),
		enbleStateToBool(p_data.get<std::string>("tagsNavigation", "enabled")),
		enbleStateToBool(p_data.get<std::string>("includesBrowsing", "enabled")))
{}

void Elem::refresh()
{
	if(shouldGenerateTags)
		tags.generateTags(ctagsFilePath, { sourcePath });
}

void Elem::appendNavigationTagFile(std::vector<std::string>& p_tagFiles) const
{
	if(shouldIncludeTagsInNavigation)
		p_tagFiles.push_back(ctagsFilePath);
}

void Elem::refreshIncludesNavigation()
{
	currentInc->parse(sourcePath);
}

boost::property_tree::ptree Elem::exportData() const
{
    boost::property_tree::ptree data;
    data.put("sourcePath", sourcePath);
    data.put("tagFilePath", ctagsFilePath);
	data.put("tagsGeneration", toEnbleState(shouldGenerateTags));
	data.put("tagsNavigation", toEnbleState(shouldIncludeTagsInNavigation));
	data.put("includesBrowsing", toEnbleState(isIncludesParsingEnabled()));
	//data.put("fileSearching", "enabled");
    return data;
}

bool Elem::operator==(const Elem& p_other) const
{
	return sourcePath == p_other.sourcePath && ctagsFilePath == p_other.ctagsFilePath
		&& shouldGenerateTags == p_other.shouldGenerateTags
		&& shouldIncludeTagsInNavigation == p_other.shouldIncludeTagsInNavigation
		&& isIncludesParsingEnabled() == p_other.isIncludesParsingEnabled();
}

bool Elem::isIncludesParsingEnabled() const
{
	return currentInc == &includes;
}

Project::~Project()
{}

Project::Project(const std::string& p_name, const std::vector<Elem>& p_items)
	: Project(p_name, p_items, g_noTags, g_noIncludes)
{}

Project::Project(const std::string& p_name, const std::vector<Elem>& p_items,
	ITags& p_tags, IIncludes& p_includes)
 : name(p_name), items(p_items), tags(p_tags), includes(p_includes)
{
	if (name.empty())
		throw std::runtime_error("Given project name is empty");
}

Project::Project(const boost::property_tree::ptree& p_data)
	: Project(p_data, g_noTags, g_noIncludes)
{}

Project::Project(const boost::property_tree::ptree& p_data,
	ITags& p_tags, IIncludes& p_includes)
	: Project(p_data.get<std::string>("name", ""), getProjcetItems(p_data, p_tags, p_includes), p_tags, p_includes)
{}

void Project::refresh()
{
    for(auto& item : items)
        item.refresh();
	refreshIncludesNavigation();
}

void Project::refreshTagsNavigation()
{
	std::vector<std::string> tagFiles;
	for (const auto& item : items)
		item.appendNavigationTagFile(tagFiles);
	tags.setTagFiles(tagFiles);
}

void Project::refreshIncludesNavigation()
{
	includes.clear();
	for (auto& item : items)
		item.refreshIncludesNavigation();
}

void Project::refershCodeNavigation()
{
	refreshTagsNavigation();
	refreshIncludesNavigation();
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
