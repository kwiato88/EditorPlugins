#include "SavelLoadSwitchInfo.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
namespace HeaderSourceSwitcher
{

std::vector<std::string> toStrVec(const boost::property_tree::ptree& p_config)
{
	std::vector<std::string> items;
	for (const auto& item : p_config)
		items.push_back(item.second.get_value<std::string>());
	return items;
}

ExtensionConvMap toExtConv(const boost::property_tree::ptree& p_config)
{
	ExtensionConvMap extConv = {};
	for (const auto& item : p_config)
		extConv[item.second.get<std::string>("sourceExtension")] = toStrVec(item.second.get_child("targetExtensions"));
	return extConv;
}

DirConvVector toDirConv(const boost::property_tree::ptree& p_config)
{
	DirConvVector dirConv = {};
	for (const auto& item : p_config)
		dirConv.push_back(std::make_pair(
			item.second.get<std::string>("name1"),
			item.second.get<std::string>("name2")));
	return dirConv;
}

FileSwitchInfo buildSwitchInfo(const boost::property_tree::ptree& p_config)
{
	FileSwitchInfo info;
	info.soureExtensions = toStrVec(p_config.get_child("sourceExtensions"));
	info.headerExtensions = toStrVec(p_config.get_child("headerExtensions"));
	info.sourceToHeaderExt = toExtConv(p_config.get_child("sourceToHeaderExt"));
	info.headerToSourceExt = toExtConv(p_config.get_child("headerToSourceExt"));
	info.sourceToHeaderDir = toDirConv(p_config.get_child("sourceToHeaderDir"));
	info.headerToSourceDir = toDirConv(p_config.get_child("headerToSourceDir"));
	return info;
}

void addInfo(std::vector<FileSwitchInfo>& p_out, const boost::property_tree::ptree& p_config, int p_idx)
{
	try
	{
		auto config = buildSwitchInfo(p_config);
		assertInfoIsValid(config);
		p_out.push_back(config);
	}
	catch (std::exception& e)
	{
		//TODO: print error
	}
}
std::vector<FileSwitchInfo> load(const Path& p_file)
{
	boost::property_tree::ptree config;
	boost::property_tree::json_parser::read_json(p_file, config);
	std::vector<FileSwitchInfo> switchInfo;
	int configIndex = 0;
	for (const auto& item : config.get_child("items"))
		addInfo(switchInfo, item.second, configIndex++);
	return switchInfo;
}

boost::property_tree::ptree toPtree(const std::string& p_str)
{
	boost::property_tree::ptree tree;
	tree.put("", p_str);
	return tree;
}

boost::property_tree::ptree toPtree(const std::vector<std::string>& p_vec)
{
	boost::property_tree::ptree tree;
	for (const auto& item : p_vec)
		tree.push_back(std::make_pair("", toPtree(item)));
	return tree;
}

boost::property_tree::ptree toPtree(const ExtensionConvMap::value_type& p_conv)
{
	boost::property_tree::ptree tree;
	tree.put("sourceExtension", p_conv.first);
	tree.add_child("targetExtensions", toPtree(p_conv.second));
	return tree;
}

boost::property_tree::ptree toPtree(const ExtensionConvMap& p_conv)
{
	boost::property_tree::ptree tree;
	for (const auto& item : p_conv)
		tree.push_back(std::make_pair("", toPtree(item)));
	return tree;
}

boost::property_tree::ptree toPtree(const DirConvVector::value_type& p_dirConv)
{
	boost::property_tree::ptree tree;
	tree.put("name1", p_dirConv.first);
	tree.put("name2", p_dirConv.second);
	return tree;
}

boost::property_tree::ptree toPtree(const DirConvVector& p_conv)
{
	boost::property_tree::ptree tree;
	for (const auto& item : p_conv)
		tree.push_back(std::make_pair("", toPtree(item)));
	return tree;
}

boost::property_tree::ptree toPtree(const FileSwitchInfo& p_info)
{
	boost::property_tree::ptree tree;
	tree.add_child("sourceExtensions", toPtree(p_info.soureExtensions));
	tree.add_child("headerExtensions", toPtree(p_info.headerExtensions));
	tree.add_child("sourceToHeaderExt", toPtree(p_info.sourceToHeaderExt));
	tree.add_child("headerToSourceExt", toPtree(p_info.headerToSourceExt));
	tree.add_child("sourceToHeaderDir", toPtree(p_info.sourceToHeaderDir));
	tree.add_child("headerToSourceDir", toPtree(p_info.headerToSourceDir));
	return tree;
}

boost::property_tree::ptree toPtree(const std::vector<FileSwitchInfo>& p_info)
{
	boost::property_tree::ptree tree;
	for (const auto& item : p_info)
		tree.push_back(std::make_pair("", toPtree(item)));
	return tree;
}

void save(const std::vector<FileSwitchInfo>& p_info, const Path& p_file)
{
	boost::property_tree::ptree tree;
	tree.add_child("items", toPtree(p_info));
	boost::property_tree::json_parser::write_json(p_file, tree);
}

}
