#pragma once

#include "WinApiTypes.hpp"

namespace NppPlugin
{

class EditorData
{
public:
	WinApi::Handle npp = {};
	WinApi::Handle getCurrentScintilla();
	WinApi::Handle scintillaMain = {};
	WinApi::Handle scintillaSecond = {};
};

}
