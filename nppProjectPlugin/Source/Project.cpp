#include <iostream>
#include <stdexcept>

#include "Project.hpp"

namespace ProjectMgmt
{

namespace
{
std::vector<Elem> getProjcetItems(const boost::property_tree::ptree& p_data)
{
    std::vector<Elem> items;
    for(const auto& item : p_data.get_child("items"))
        items.push_back(Elem(item.second));
    return items;
}
}

Elem::Elem(const std::string& p_sourcePath, const std::string& p_ctagsFilePath)
 : sourcePath(p_sourcePath), ctagsFilePath(p_ctagsFilePath)
{
    if(sourcePath.empty() && ctagsFilePath.empty())
        throw std::runtime_error("Given source path and tag file path is empty");
}

Elem::Elem(const boost::property_tree::ptree& p_data)
 : Elem(p_data.get<std::string>("sourcePath", ""), p_data.get<std::string>("tagFilePath", ""))
{
}

void Elem::refresh()
{
    std::cout << "Refresh source: " << sourcePath << " with tags file: " << ctagsFilePath << std::endl;
}

boost::property_tree::ptree Elem::exportData() const
{
    boost::property_tree::ptree data;
    data.put("sourcePath", sourcePath);
    data.put("tagFilePath", ctagsFilePath);
    return data;
}

bool Elem::operator==(const Elem& p_other) const
{
	return sourcePath == p_other.sourcePath && ctagsFilePath == p_other.ctagsFilePath;
}

Project::Project(const std::string& p_name, const std::vector<Elem>& p_items)
 : name(p_name), items(p_items)
{}

Project::Project(const boost::property_tree::ptree& p_data)
 : name(p_data.get<std::string>("name", "")), items(getProjcetItems(p_data))
{
	if (name.empty())
		throw std::runtime_error("Given project name is empty");
}

void Project::refresh()
{
    for(auto& item : items)
        item.refresh();
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