#pragma once

#include <string>
#include <vector>
#include <map>
#include <boost/serialization/access.hpp>
#include <boost/property_tree/ptree.hpp>
#include "PtreeUtils.hpp"

namespace CTagsPlugin
{
namespace Command
{

struct GenerateTags
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & tagFilePath;
		p_ar & sourceDirsPaths;
	}
	inline boost::property_tree::ptree exportMsg() const
	{
		PtreeUtils::ToPtree printer;
		return printer.add("tagFilePath", tagFilePath).add("sourceDirsPaths", sourceDirsPaths).get();
	}
	inline void importMsg(const boost::property_tree::ptree& p_msg)
	{
		PtreeUtils::FromPtree printer(p_msg);
		tagFilePath = printer.get<std::string>("tagFilePath");
		sourceDirsPaths = printer.get<std::vector<std::string>>("sourceDirsPaths");
	}
	inline bool operator==(const GenerateTags& other) const
	{
		return tagFilePath == other.tagFilePath && sourceDirsPaths == other.sourceDirsPaths;
	}

	static long Id() { return 1; }

    std::string tagFilePath;
	std::vector<std::string> sourceDirsPaths;
};

struct SetTagFiles
{
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & filesPaths;
	}
	inline boost::property_tree::ptree exportMsg() const
	{
		PtreeUtils::ToPtree printer;
		return printer.add("filesPaths", filesPaths).get();
	}
	inline void importMsg(const boost::property_tree::ptree& p_msg)
	{
		PtreeUtils::FromPtree printer(p_msg);
		filesPaths = printer.get<std::vector<std::string>>("filesPaths");
	}
	inline bool operator==(const SetTagFiles& other) const
	{
		return filesPaths == other.filesPaths;
	}

    static long Id() { return 2; }

    std::vector<std::string> filesPaths;
};

struct GetTagFiles
{
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
	}
	inline boost::property_tree::ptree exportMsg() const
	{
		return boost::property_tree::ptree();
	}
	inline void importMsg(const boost::property_tree::ptree&)
	{}
	inline bool operator==(const GetTagFiles&) const
	{
		return true;
	}

    static long Id() { return 3; }
};

struct GetTagLocation
{
	enum class Search
	{
		ByName,
		ByParentName,
		InTagHierachy
	};
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & tagName;
		p_ar & mode;
	}
	inline boost::property_tree::ptree exportMsg() const
	{
		PtreeUtils::ToPtree printer;
		return printer.add("tagName", tagName).add("mode", modeStr()).get();
	}
	inline void importMsg(const boost::property_tree::ptree& p_msg)
	{
		PtreeUtils::FromPtree printer(p_msg);
		tagName = printer.get<std::string>("tagName");
		mode = modeFromStr(printer.get<std::string>("mode"));
	}
	inline bool operator==(const GetTagLocation& other) const
	{
		return tagName == other.tagName && mode == other.mode;
	}

	static long Id() { return 4; }

	std::string tagName;
	Search mode;

	inline std::string modeStr() const
	{
		switch (mode)
		{
		case Search::ByName: return "ByName";
		case Search::ByParentName: return "ByParentName";
		case Search::InTagHierachy: return "InTagHierachy";
		default: return "ByName";
		}
	}
	inline Search modeFromStr(const std::string& p_mode) const
	{
		static const std::map<std::string, Search> conv{
			{"ByName", Search::ByName}, {"ByParentName", Search::ByParentName}, {"InTagHierachy", Search::InTagHierachy} };
		auto found = conv.find(p_mode);
		if (found != conv.end())
			return found->second;
		return Search::ByName;
	}
};

struct Test
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & p_ar, const unsigned int version)
    {
      p_ar & value;
    }
	inline boost::property_tree::ptree exportMsg() const
	{
		PtreeUtils::ToPtree printer;
		return printer.add("value", value).get();
	}
	inline void importMsg(const boost::property_tree::ptree& p_msg)
	{
		PtreeUtils::FromPtree printer(p_msg);
		value = printer.get<int>("value");
	}
	inline bool operator==(const Test& other) const
	{
		return value == other.value;
	}

	static long Id() { return 255; }

    int value;
};

}
}
