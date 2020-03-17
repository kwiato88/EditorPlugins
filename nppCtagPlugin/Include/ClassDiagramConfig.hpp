#pragma once

namespace CTagsPlugin
{

struct ClassDiagramConfig
{
	enum class Hierarchy
	{
		none,
		direct,
		full
	};

	Hierarchy inheritedTags;
	Hierarchy derivedTags;
	bool includeMembers;
};

}
