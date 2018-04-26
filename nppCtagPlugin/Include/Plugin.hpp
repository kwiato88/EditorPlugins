#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <map>

#include "PluginInterface.h"

#include "NppEditor.hpp"
#include "WinApiUI.hpp"
#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "CTagsController.hpp"
#include "ConfigGetter.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

class MesgTraits
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

class TagsPlugin
{
public:
	TagsPlugin();

	/**
	 * plugin interface
	 */
	void init(HINSTANCE p_hModule);
	void cleanup();
	void commandMenuInit(NppData p_nppData);
	void commandMenuCleanUp();
	void handleMsgToPlugin(CommunicationInfo& p_message);

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

	static const int s_funcNum = 13;// 16;

	bool m_isInitialized;
	WinApi::InstanceHandle m_hModule;
	FuncItem m_funcItems[s_funcNum];
	Editor m_npp;

private:
	bool setCommand(TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk);
	void setSeparator();
	void createTagsController();
	void initFunctionsTable();
	void loadConfigFile();
	void setLoggerParams();
	std::string getPluginsConfigDir();
	std::shared_ptr<CTagsPlugin::ITagsSelector> buildListViewSelector();
	std::shared_ptr<CTagsPlugin::ITagsSelector> buildGridViewSelector();
	std::shared_ptr<CTagsPlugin::ITagsSelector> buildTagsSelector();
	std::shared_ptr<CTagsPlugin::ITagsSelector> getTagsSelector();
	std::unique_ptr<CTagsPlugin::ITagsReader> buildReadTagsProxy(const std::string& p_tagFilePath);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildTagFileReader(const std::string& p_tagFilePath);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildNativeTagReader(const std::string& p_tagFilePath);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildCachedTagFileReader(const std::string& p_tagFilePath, std::unique_ptr<CTagsPlugin::ITagsReader> p_reader);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildFileScopedTagFileReader(std::unique_ptr<CTagsPlugin::ITagsReader> p_reader);
	std::unique_ptr<CTagsPlugin::ITagsReader> buildTagReader(const std::string& p_tagFilePath);

	int m_numberOfAddedFunctions = 0;
	WinApi::UI m_ui;
	std::shared_ptr<CTagsPlugin::ITagsSelector> m_selector;
	CTagsPlugin::ConfigGetter m_config;
	std::string m_configFilePath;
	std::shared_ptr<CTagsPlugin::CTagsController> m_tagsController;
	MesgTraits messagesNames;

};

} // namespace NppPlugin

#endif // PLUGIN
