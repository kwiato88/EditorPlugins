#include <gtest/gtest.h>

#include "Commands.hpp"
#include "Results.hpp"
#include "JsonCodec.hpp"

namespace CTagsPlugin
{
using namespace ::testing;

namespace
{
template<typename Msg>
Msg imported(const Msg& original)
{
	Msg importedMsg = {};
	importedMsg.importMsg(original.exportMsg());
	return importedMsg;
}
template<typename Msg>
Msg decoded(const Msg& original)
{
	auto buff = Messaging::JsonCodec::encode(original);
	Msg decodedMsg = Messaging::JsonCodec::decode<Msg>(buff);
	return decodedMsg;
}
}

TEST(CodecTS, importExportGenerateTags)
{
	Command::GenerateTags msg;
	msg.tagFilePath = "myPath\\file.txt";
	msg.sourceDirsPaths = { "path1", "path2\\path3" };

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportSetTagFiles)
{
	Command::SetTagFiles msg;
	msg.filesPaths = { "path1", "path2\\path3" };

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportGetTagFiles)
{
	Command::GetTagFiles msg;

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportCommandTest)
{
	Command::Test msg;
	msg.value = 42;

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportBasic)
{
	Result::Basic msg;
	msg.res = Result::Result::Success;

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportTagFiles)
{
	Result::TagFiles msg;
	msg.filesPaths = { "path1.txt", "path2\\path3.txt" };

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportResultTest)
{
	Result::Test msg;
	msg.value = 209;

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportGetTagLocation)
{
	Command::GetTagLocation msg;
	msg.tagName = "name1";
	msg.mode = Command::GetTagLocation::Search::ByParentName;

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

TEST(CodecTS, importExportLocation)
{
	Result::Location msg;
	msg.filePath = "dir\\file.cpp";
	msg.line = 102;

	ASSERT_EQ(msg, imported(msg));
	ASSERT_EQ(msg, decoded(msg));
}

}
