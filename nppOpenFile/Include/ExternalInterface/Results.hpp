#pragma once

#include <boost/serialization/access.hpp>

namespace OpenFileResult
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

	Result res;
};

struct SearchDirs
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & paths;
	}

	std::vector<std::string> paths;
};

struct Files
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
        p_ar & res;
		p_ar & paths;
	}

    Result res;
	std::vector<std::string> paths;
};

}
