#include "SwitcherFunction.hpp"
#include "OpenFileException.hpp"
#include "SwitchFile.hpp"
#include "SavelLoadSwitchInfo.hpp"

namespace HeaderSourceSwitcher
{

FileSwitchInfo buildCCppSwitchInfo()
{
	FileSwitchInfo c_cpp_switch_info;

	std::vector<std::string> cppSourcesEx = { "cpp", "cc" };
	std::vector<std::string> cSourcesEx = { "c" };
	std::vector<std::string> cppHeaderEx = { "hpp", "hh" };
	std::vector<std::string> cHeaderEx = { "h" };
	
	c_cpp_switch_info.soureExtensions = { "cpp", "cc", "c" };
	c_cpp_switch_info.headerExtensions = {"hpp", "hh", "h"};

	c_cpp_switch_info.sourceToHeaderExt = {
		{ "cpp", cppHeaderEx },
		{ "cc", cppHeaderEx },
		{ "c", cHeaderEx} };
	c_cpp_switch_info.headerToSourceExt = {
		{ "hpp", cppSourcesEx },
		{ "hh", cppSourcesEx },
		{ "h", cSourcesEx }};

	c_cpp_switch_info.sourceToHeaderDir = {
		{ "\\Source\\", "\\Include\\" },
		{ "\\src\\", "\\inc\\" }};

	c_cpp_switch_info.headerToSourceDir = {
		{ "\\Include\\", "\\Source\\" },
		{ "\\inc\\", "\\src\\" } };

	return c_cpp_switch_info;
}


Switcher::Switcher(Plugin::Editor& p_editor, Plugin::UI& p_ui)
	: editor(p_editor), ui(p_ui), config({ buildCCppSwitchInfo() })
{}

void Switcher::switchFile()
{
	try
	{
		//TODO: allow to use multiple config items
		std::string fileToSet = HeaderSourceSwitcher::switchFile(editor.getFile(), config.at(0));
		editor.setFile(fileToSet);
	}
	catch (Plugin::OpenFileException&)
	{
	}
	catch (std::exception& e)
	{
		ui.errorMessage("H<->S", std::string("Failed to switch file. Detail: ") + e.what());
	}
}

void Switcher::processErrors(const std::vector<std::string>& p_errors)
{
	if(!p_errors.empty())
		ui.infoMessage("H<->S",
			std::string("Config file loaded with errors. Number of ignored items ") + std::to_string(p_errors.size()));
	for (const auto& error : p_errors)
		ui.infoMessage("H<->S", error);
}

void Switcher::applyConfig(const std::vector<FileSwitchInfo>& p_config)
{
	if (p_config.empty())
		ui.infoMessage("H<->S", "Loaded config is empty. Using default config.");
	else
		config = p_config;
}

void Switcher::loadConfig(const std::string& p_filePath)
{
	try
	{
		auto loaded = load(p_filePath);
		processErrors(loaded.second);
		applyConfig(loaded.first);
	}
	catch (std::exception& e)
	{
		ui.errorMessage("H<->S", std::string("Failed to load config file. Detail: ") + e.what());
	}
}

void Switcher::saveConfig(const std::string& p_filePath)
{
	try
	{
		save(config, p_filePath);
	}
	catch (std::exception&)
	{
	}
}

} // HeaderSourceSwitcher
