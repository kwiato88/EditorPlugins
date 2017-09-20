#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <boost\shared_ptr.hpp>

#include "PluginInterface.h"

#include "NppMessagePrinter.hpp"
#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "CTagsController.hpp"
#include "ConfigGetter.hpp"

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
    void tagInfo();
	void clearTags();
	void setTagsFiles();
	void generateTagsFile();
	void info();

	static const int s_funcNum = 10;//10 functions (12 with test functions);

	bool m_isInitialized;
	HINSTANCE m_hModule;
	FuncItem m_funcItems[s_funcNum];
	NppData m_nppData;

private:
	bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk);
	void setSeparator(size_t index);
	void createTagsController();
	void initFunctionsTable();
	void loadConfigFile();
	std::string getPluginsConfigDir();
    boost::shared_ptr<CTagsPlugin::ITagsSelector> buildListViewSelector();
    boost::shared_ptr<CTagsPlugin::ITagsSelector> buildGridViewSelector();
    boost::shared_ptr<CTagsPlugin::ITagsSelector> buildTagsSelector();
    boost::shared_ptr<CTagsPlugin::ITagsSelector> getTagsSelector();
    boost::shared_ptr<CTagsPlugin::ITagsReader> buildReadTagsProxy(const std::string& p_tagFilePath);
    boost::shared_ptr<CTagsPlugin::ITagsReader> buildTagFileReader(const std::string& p_tagFilePath);
    boost::shared_ptr<CTagsPlugin::ITagsReader> buildNativeTagReader(const std::string& p_tagFilePath);
	boost::shared_ptr<CTagsPlugin::ITagsReader> buildTagReader(const std::string& p_tagFilePath);

    boost::shared_ptr<NppPlugin::NppMessagePrinter> m_printer;
    boost::shared_ptr<CTagsPlugin::ITagsSelector> m_selector;
	CTagsPlugin::ConfigGetter m_config;
	std::string m_configFilePath;
    boost::shared_ptr<CTagsPlugin::CTagsController> m_tagsController;

};

} // namespace NppPlugin

#endif // PLUGIN
