#include "CTagsMT.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

TagHierarchySelectorProxy::TagHierarchySelectorProxy(ITagHierarchySelector& p_selector)
    : m_selector(p_selector)
{}
boost::optional<TagHolder> TagHierarchySelectorProxy::select(const TagHierarchy& p_hier)
{
	return m_selector.select(p_hier);
}

void CTagsMT::expectGetAnyLocation()
{
	EXPECT_CALL(editor, getFile()).WillRepeatedly(Return(""));
	EXPECT_CALL(editor, getLine()).WillRepeatedly(Return(0));
	EXPECT_CALL(editor, getColumn()).WillRepeatedly(Return(0));
}
void CTagsMT::setPathToFileWithInvalidTags()
{
	tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile_ModifiedWithInvalidTags.txt";
}

}
