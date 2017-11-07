#pragma once

#include <string>
#include <vector>
#include <boost/serialization/access.hpp>

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

    static long Id() { return 3; }
};

struct Dummy
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & p_ar, const unsigned int version)
    {
      p_ar & value;
    }
    
    int value;
};

}
}
