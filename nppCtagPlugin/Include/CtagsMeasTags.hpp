#pragma once

namespace CTagsPlugin
{

struct TagsLoadTime
{
	static inline const char* name()
	{
		return "TagsLoadTimePer1000Tags";
	}
};

struct FindChildrenTagsTime
{
	static inline const char* name()
	{
		return "FindChildrenTagsTime";
	}
};

struct ParseTagsHierarchyTime
{
	static inline const char* name()
	{
		return "ParseTagsHierarchyTime";
	}
};

struct GenerateClassInDiagramTime
{
	static inline const char* name()
	{
		return "GenerateClassDescriptionInDiagram";
	}
};

struct GenerateClassDiagramTime
{
	static inline const char* name()
	{
		return "GenerateClassDiagramTime";
	}
};

struct ChildrenTagsCallCount
{
	static inline const char* name()
	{
		return "ChildrenTagsCallCount";
	}
};
struct ChildrenTagsCacheHits
{
	static inline const char* name()
	{
		return "ChildrenTagsCacheHits";
	}
};

struct TagsHierarchyCallCount
{
	static inline const char* name()
	{
		return "TagsHierarchyCallCount";
	}
};
struct TagsHierarchyCacheHits
{
	static inline const char* name()
	{
		return "TagsHierarchyCacheHits";
	}
};

}
