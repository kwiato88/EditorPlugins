#include "NppIncludes.hpp"

namespace NppPlugin
{

Includes::Includes(HWND p_npp, const std::string& p_sourcePluginName, Plugin::UI& p_ui)
	: cmd(p_npp, p_sourcePluginName), ui(p_ui)
{}

void Includes::parse(const std::string& p_dirPath)
{
	try
	{
		cmd.parse(p_dirPath);
	}
	catch (std::exception& e)
	{
		ui.errorMessage("ERROR", std::string("Failed to parse includes. ") + e.what());
	}
}

void Includes::clear()
{
	try
	{
		cmd.clear();
	}
	catch (std::exception& e)
	{
		ui.errorMessage("ERROR", std::string("Failed to clear includes. ") + e.what());
	}
}

}
