#pragma once

#include <vector>
#include <string>
#include <boost/serialization/access.hpp>
#include "PtreeUtils.hpp"

namespace CTagsPlugin
{
namespace Result
{

enum class Result
{
    Success,
    Failure
};

struct Basic
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & res;
	}
	inline boost::property_tree::ptree exportMsg() const
	{
		PtreeUtils::ToPtree printer;
		return printer.add("result", res == Result::Success ? "Success" : "Failure").get();
	}
	inline void importMsg(const boost::property_tree::ptree& p_msg)
	{
		PtreeUtils::FromPtree printer(p_msg);
		auto result = printer.get<std::string>("result");
		if (result == "Success")
			res = Result::Success;
		else
			res = Result::Failure;
	}
	inline bool operator==(const Basic& other) const
	{
		return res == other.res;
	}

	Result res;
};

struct TagFiles
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
	inline bool operator==(const TagFiles& other) const
	{
		return filesPaths == other.filesPaths;
	}

	std::vector<std::string> filesPaths;
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

    int value;
};

}
}
