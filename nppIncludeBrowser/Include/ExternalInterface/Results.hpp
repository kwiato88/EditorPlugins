#pragma once

#include <boost/serialization/access.hpp>

namespace IncludeBrowser
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

	Result res;
};

}
}
