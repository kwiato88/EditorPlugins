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
			item.second.get<std::string>("source"),
			item.second.get<std::string>("target")));
	return dirConv;
}

FileSwitchInfo buildSwitchInfo(const boost::property_tree::ptree& p_config)
{
	FileSwitchInfo info;
	info.sourceExtensions = toStrVec(p_config.get_child("sourceExtensions"));
	info.headerExtensions = toStrVec(p_config.get_child("headerExtensions"));
	info.sourceToHeaderExt = toExtConv(p_config.get_child("sourceToHeaderExt"));
	info.headerToSourceExt = toExtConv(p_config.get_child("headerToSourceExt"));
	info.sourceToHeaderDir = toDirConv(p_config.get_child("sourceToHeaderDir"));
	info.headerToSourceDir = toDirConv(p_config.get_child("headerToSourceDir"));
	return info;
}

class SwitchInfoLoader
{
public:
	std::vector<FileSwitchInfo> result;
	std::vector<std::string> errors;

	void parse(const boost::property_tree::ptree& p_config)
	{
		try
		{
			auto config = buildSwitchInfo(p_config);
			assertInfoIsValid(config);
			result.push_back(config);
		}
		catch (std::exception& e)
		{
			errors.push_back(std::string("Ignoring config ") + std::to_string(configCount) + ". " + e.what());
		}
		configCount++;
	}

private:
	int configCount = 0;
};

std::pair<std::vector<FileSwitchInfo>, std::vector<std::string>> load(const Path& p_file)
{
	boost::property_tree::ptree config;
	boost::property_tree::json_parser::read_json(p_file, config);
	SwitchInfoLoader loader;
	for (const auto& item : config.get_child("items"))
		loader.parse(item.second);
	return std::make_pair(loader.result, loader.errors);
}

template<typename Container>
boost::property_tree::ptree toPtree(const Container& p_vec)
{
	boost::property_tree::ptree tree;
	for (const auto& item : p_vec)
		tree.push_back(std::make_pair("", toPtree(item)));
	return tree;
}

template<>
boost::property_tree::ptree toPtree<std::string>(const std::string& p_str)
{
	boost::property_tree::ptree tree;
	tree.put("", p_str);
	return tree;
}

template<>
boost::property_tree::ptree toPtree<ExtensionConvMap::value_type>(const ExtensionConvMap::value_type& p_conv)
{
	boost::property_tree::ptree tree;
	tree.put("sourceExtension", p_conv.first);
	tree.add_child("targetExtensions", toPtree(p_conv.second));
	return tree;
}

template<>
boost::property_tree::ptree toPtree<DirConvVector::value_type>(const DirConvVector::value_type& p_dirConv)
{
	boost::property_tree::ptree tree;
	tree.put("source", p_dirConv.first);
	tree.put("target", p_dirConv.second);
	return tree;
}

template<>
boost::property_tree::ptree toPtree<FileSwitchInfo>(const FileSwitchInfo& p_info)
{
	boost::property_tree::ptree tree;
	tree.add_child("sourceExtensions", toPtree(p_info.sourceExtensions));
	tree.add_child("headerExtensions", toPtree(p_info.headerExtensions));
	tree.add_child("sourceToHeaderExt", toPtree(p_info.sourceToHeaderExt));
	tree.add_child("headerToSourceExt", toPtree(p_info.headerToSourceExt));
	tree.add_child("sourceToHeaderDir", toPtree(p_info.sourceToHeaderDir));
	tree.add_child("headerToSourceDir", toPtree(p_info.headerToSourceDir));
	return tree;
}

void save(const std::vector<FileSwitchInfo>& p_info, const Path& p_file)
{
	boost::property_tree::ptree tree;
	tree.add_child("items", toPtree(p_info));
	boost::property_tree::json_parser::write_json(p_file, tree);
}

}
