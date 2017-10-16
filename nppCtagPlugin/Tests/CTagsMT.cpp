#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/replace.hpp>
#include <exception>
#include <typeinfo>
#include <sstream>
#include <memory>
#include <algorithm>
#include "TestsGlobals.hpp"
#include "CTagsMT.hpp"
#include "MatcherHasMoreThanOneElem.hpp"
#include "MatcherEndsWith.hpp"
#include "MatcherTags.hpp"

#include "CppTag.hpp"
#include "CppIsTagWithAtt.hpp"
#include "CTagsNavigator.hpp"
#include "Navigator.hpp"
#include "TagFileReader.hpp"
#include "CachedTagsReader.hpp"
#include "TagNotFoundException.hpp"
#include "TagsReaderException.hpp"
#include "OpenFileException.hpp"
#include "LocationGetterMock.hpp"
#include "LocationSetterMock.hpp"
#include "TagsSelectorMock.hpp"
#include "TagHierarchySelectorMock.hpp"
#include "TagParser.hpp"

#include "Logger.hpp"
#include "Log.hpp"

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
	EXPECT_CALL(*locationGetter, getFile()).WillRepeatedly(Return(""));
	EXPECT_CALL(*locationGetter, getLine()).WillRepeatedly(Return(0));
	EXPECT_CALL(*locationGetter, getColumn()).WillRepeatedly(Return(0));
}
void CTagsMT::setPathToFileWithInvalidTags()
{
	tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tagsFile_ModifiedWithInvalidTags.txt";
}

}
