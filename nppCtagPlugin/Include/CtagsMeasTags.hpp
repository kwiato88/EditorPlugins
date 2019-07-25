#pragma once

namespace CTagsPlugin
{

struct TagsLoadTime
{
	static inline char* name()
	{
		return "TagsLoadTimePer1000Tags";
	}
};

struct FindChildrenTagsTime
{
	static inline char* name()
	{
		return "FindChildrenTagsTime";
	}
};

struct ParseTagsHierarchyTime
{
	static inline char* name()
	{
		return "ParseTagsHierarchyTime";
	}
};

}
