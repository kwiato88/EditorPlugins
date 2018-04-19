#include "SwitcherFunction.hpp"
#include "LocationSettetException.hpp"
#include "SwitchFile.hpp"

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

void setLocation(const std::string& p_file, Plugin::Editor& p_editor)
{
	try
	{
		p_editor.setFile(p_file);
	}
	catch(Plugin::LocationSetterException&)
	{
	}
}

void switchFile(Plugin::Editor& p_editor)
{
	std::string l_currentFile = p_editor.getFile();
	std::string l_fileToSet = switchFile(l_currentFile, buildCCppSwitchInfo());
	setLocation(l_fileToSet, p_editor);
}

} // HeaderSourceSwitcher
