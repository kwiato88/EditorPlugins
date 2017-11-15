#pragma once

#include <string>
#include <boost/serialization/access.hpp>

namespace IncludeBrowser
{
namespace Command
{

struct Parse
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & dirPath;
	}

	static long Id() { return 1; }

    std::string dirPath;
};

struct Clear
{
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
	}

    static long Id() { return 2; }
};

}
}
