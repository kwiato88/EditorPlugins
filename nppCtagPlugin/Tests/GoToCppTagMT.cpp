#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CppTag.hpp"
#include "CppIsTagWithAtt.hpp"

#include "CTagsMT.hpp"
#include "MatcherEndsWith.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

struct GoToCppTagMT : public CTagsMT
{
	GoToCppTagMT()
	{
		expectGetAnyLocation();
	}
};

TEST_F(GoToCppTagMT, goToClassWithName)
{
	Cpp::IsTagWithAtt matcher(
		".*CTagsNa.*",
		{ CppTag::Kind::Class },
		{ CppTag::Access::None, CppTag::Access::Private, CppTag::Access::Protected, CppTag::Access::Public });

	EXPECT_CALL(editor, setFile(EndsWith("TestSourceCode\\\\Include\\\\CTagsNavigator.hpp")));
	EXPECT_CALL(editor, setLine(18));
	EXPECT_CALL(editor, setColumn(0));
	tagsNavigator.goToTag(matcher);
}

}
