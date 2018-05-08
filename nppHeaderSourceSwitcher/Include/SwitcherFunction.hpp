#ifndef NPP_FILE_SWITCHER_HPP
#define NPP_FILE_SWITCHER_HPP

#include "Editor.hpp"
#include "UI.hpp"
#include "SwitchFile.hpp"

namespace HeaderSourceSwitcher
{

class Switcher
{
public:
	Switcher(Plugin::Editor& p_editor, Plugin::UI& p_ui);

	void loadConfig(const std::string& p_filePath);
	void saveConfig(const std::string& p_filePath);
	void switchFile();

private:

	void processErrors(const std::vector<std::string>& p_errors);
	void applyConfig(const std::vector<FileSwitchInfo>& p_config);
	FileSwitchInfo getConfig(const std::string& p_fileExtension);

	Plugin::Editor& editor;
	Plugin::UI& ui;
	std::vector<FileSwitchInfo> config;
};

} // HeaderSourceSwitcher

#endif NPP_FILE_SWITCHER_HPP
