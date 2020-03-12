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

TagsSelectorProxy::TagsSelectorProxy(ITagsSelector& p_selector)
	: selector(p_selector)
{}
int TagsSelectorProxy::selectTag(const std::vector<TagHolder>& p_tags)
{
	return selector.selectTag(p_tags);
}

void ConfigStub::loadConfigFile(const std::string&) {}
void ConfigStub::saveConfigFile(const std::string&) {}
std::string ConfigStub::getCtagsPath() const { return ""; }
std::string ConfigStub::getReadTagsPath() const { return ""; }
SelectTagsViewType ConfigStub::getSelectTagsViewType() const { return SelectTagsViewType(); }
TagReaderType ConfigStub::getTagsReaderType() const { return TagReaderType(); }
std::vector<std::string> ConfigStub::getTagsFilesPaths() const { return {}; }
void ConfigStub::setTagsFilesPaths(const std::vector<std::string>&) {}
Fields ConfigStub::getSupportedExtensionFileds() const { return Fields{}; }
bool ConfigStub::shouldFilterFileScopedTags() const { return false; }
bool ConfigStub::shouldCacheTags() const { return false; }
std::string ConfigStub::getPlantUmlPath() const { return ""; }
ClassDiagramConfig ConfigStub::getClassDiagramConfig() const
{
	ClassDiagramConfig config = {};
	config.derivedTags = ClassDiagramConfig::Hierarchy::none;
	config.inheritedTags = ClassDiagramConfig::Hierarchy::direct;
	config.includeMembers = true;
	return config;
}
bool ConfigStub::isLoggerEnabled() const { return false; }
Logger::Level ConfigStub::getLogSeverity() const { return Logger::Level(); }

void CTagsMT::expectGetAnyLocation()
{
	EXPECT_CALL(editor, getFile()).WillRepeatedly(Return(""));
	EXPECT_CALL(editor, getLine()).WillRepeatedly(Return(0));
	EXPECT_CALL(editor, getColumn()).WillRepeatedly(Return(0));
}
void CTagsMT::setPathToFileWithInvalidTags()
{
	tagsFilePath = rootPath + "nppCtagPlugin\\Tests\\TestSourceCode\\tmp_tagsFile_ModifiedWithInvalidTags.txt";
}

}
