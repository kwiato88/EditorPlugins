#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>

#include "PluginInterface.h"

#include "EditorData.hpp"
#include "NppMessagePrinter.hpp"
#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "CTagsController.hpp"
#include "ConfigGetter.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

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
    void tagInfo_backgroundThread();
	void tagInfo_mainThread();
	void cppSearch();
	void tagHierarchy();
	void clearTags();
	void setTagsFiles();
	void generateTagsFile();
	void info();

	static const int s_funcNum = 13;

	bool m_isInitialized;
	WinApi::InstanceHandle m_hModule;
	FuncItem m_funcItems[s_funcNum];
	EditorData m_npp;

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
	std::shared_ptr<NppPlugin::NppMessagePrinter> m_printer;
	std::shared_ptr<CTagsPlugin::ITagsSelector> m_selector;
	CTagsPlugin::ConfigGetter m_config;
	std::string m_configFilePath;
	std::shared_ptr<CTagsPlugin::CTagsController> m_tagsController;

};

} // namespace NppPlugin

#endif // PLUGIN
