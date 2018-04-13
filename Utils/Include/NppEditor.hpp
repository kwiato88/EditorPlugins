#pragma once

#include "Editor.hpp"
#include "WinApiTypes.hpp"

namespace NppPlugin
{

class Editor : public Plugin::Editor
{
public:
	WinApi::Handle npp = {};
	WinApi::Handle scintillaMain = {};
	WinApi::Handle scintillaSecond = {};

	WinApi::Handle getCurrentScintilla() const;

	std::string getFile() const;
	int getLine() const;
	int getColumn() const;

	void setFile(const std::string& p_filePath);
	void setLine(int p_line);
	void setColumn(int p_column);

	std::string getCurrentWord() const;
};

}
