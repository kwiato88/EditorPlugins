#pragma once

#include <memory>
#include <map>

#include "NppPlugin.hpp"

#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "CTagsController.hpp"
#include "ConfigGetter.hpp"
#include "WinApiUIFileSystem.hpp"

namespace NppPlugin
{

class MsgTraits
{
public:
	void add(long p_msgId, const std::string& p_msgName)
	{
		idsToNames[p_msgId] = p_msgName;
	}
	std::string get(long p_msgId)
	{
		auto msg = idsToNames.find(p_msgId);
		if (msg == idsToNames.end())
			return "UnknownMessage";
		return msg->second;
	}
private:
	std::map<long, std::string> idsToNames;
};

const TCHAR NPP_PLUGIN_NAME[] = TEXT("CTags Plugin");

class TagsPlugin : public BasePluginWithUI<16>//13
{
public:
	TagsPlugin();

	void handleMsgToPlugin(CommunicationInfo& p_message) override;
	void detach() override;

	/**
	 * plugin test functions
	 */
	void test1();
	void test2();

	/**
	 * plugin menu functions
	 */
	void nextTag();
	void previousTag();
	void findTag();
    void tagInfo();
	void cppSearch();
	void tagHierarchy();
	void clearTags();
	void setTagsFiles();
	void generateTagsFile();
	void info();

protected:
	void init() override;
	void onNppHandleSet() override;
	void initMenu() override;

private:
	void createTagsController();
	void loadConfigFile();
	void setLoggerParams();
	std::unique_ptr<CTagsPlugin::ITagsSelector> buildListViewSelector();
	std::unique_ptr<CTagsPlugin::ITagsSelector> buildGridViewSelector();
	std::unique_ptr<CTagsPlugin::ITagsSelector> buildTagsSelector();
	std::unique_ptr<CTagsPlugin::ITagsReader> buildReadTagsProxy(const std::string& p_tagFilePath);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildTagFileReader(const std::string& p_tagFilePath);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildNativeTagReader(const std::string& p_tagFilePath);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildCachedTagFileReader(const std::string& p_tagFilePath, std::unique_ptr<CTagsPlugin::ITagsReader> p_reader);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildFileScopedTagFileReader(std::unique_ptr<CTagsPlugin::ITagsReader> p_reader);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildTagReader(const std::string& p_tagFilePath);

	WinApi::UIFileSystem files;
	CTagsPlugin::ConfigGetter config;
	std::string configFilePath;
	std::shared_ptr<CTagsPlugin::CTagsController> tagsController;
	MsgTraits messagesNames;

};

} // namespace NppPlugin
