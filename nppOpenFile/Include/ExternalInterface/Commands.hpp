#pragma once

#include <string>
#include <vector>
#include <boost/serialization/access.hpp>

namespace OpenFileCommand
{

struct SetSearchDirs
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & dirsPaths;
	}

	static long Id() { return 1; }

    std::vector<std::string> dirsPaths;
};

struct GetSearchDirs
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
	}

	static long Id() { return 2; }
};

struct FindFiles
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
		p_ar & dirsPaths;
        p_ar & fileNamePattern;
		p_ar & caseSensitiveSearch;
	}

	static long Id() { return 3; }

    std::vector<std::string> dirsPaths;
    std::string fileNamePattern;
	bool caseSensitiveSearch;
};

struct ClearCache
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & p_ar, const unsigned int version)
	{
	}

	static long Id() { return 4; }
};

}
