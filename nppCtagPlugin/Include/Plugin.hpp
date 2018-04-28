#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <memory>
#include <map>

#include "NppPlugin.hpp"

#include "ITagsSelector.hpp"
#include "ITagsReader.hpp"
#include "CTagsController.hpp"
#include "ConfigGetter.hpp"

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

class TagsPlugin : public BasePluginWithUI<13>
{
public:
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
	void onInstanceHandleSet() override;
	void onNppHandleSet() override;
	void initMenu() override;

private:
	void createTagsController();
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

	std::shared_ptr<CTagsPlugin::ITagsSelector> m_selector;
	CTagsPlugin::ConfigGetter m_config;
	std::string m_configFilePath;
	std::shared_ptr<CTagsPlugin::CTagsController> m_tagsController;
	MesgTraits messagesNames;

};

} // namespace NppPlugin

#endif // PLUGIN
