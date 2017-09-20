#include <stdlib.h>
#include "NppLocationSetter.hpp"
#include "LocationSettetException.hpp"
#include "Notepad_plus_msgs.h"
#include "Scintilla.h"

namespace NppPlugin
{

NppLocationSetter::NppLocationSetter(EditorData& p_npp)
	: m_npp(p_npp)
{
}

void NppLocationSetter::setFile(std::string p_filePath)
{
	TCHAR l_filePath[_MAX_PATH];
	mbstowcs(l_filePath, p_filePath.c_str(), _MAX_PATH - 1);
	if (!::SendMessage(m_npp.npp, NPPM_DOOPEN, 0, (LPARAM)l_filePath))
	{
	    throw Plugin::LocationSetterException("Can't open file: " + p_filePath);
	}
}

void NppLocationSetter::setLine(int p_line)
{
	HWND l_hSciCurrHandle = m_npp.getCurrentScintilla();

	::SendMessage(l_hSciCurrHandle, SCI_GOTOLINE, (WPARAM)p_line, 0);
    scrollToLine(l_hSciCurrHandle, p_line);
}

int NppLocationSetter::getFirstVisableLine(HWND p_SciCurrHandle)
{
    return (int)::SendMessage(p_SciCurrHandle, SCI_GETFIRSTVISIBLELINE, 0, 0);
}

void NppLocationSetter::scrollLines(HWND p_SciCurrHandle, int p_lines)
{
    ::SendMessage(p_SciCurrHandle, SCI_LINESCROLL, 0, p_lines);
}

void NppLocationSetter::scrollToLine(HWND p_SciCurrHandle, int p_line)
{
    int l_firstVisibleLineNo = getFirstVisableLine(p_SciCurrHandle);
	if (p_line > l_firstVisibleLineNo)
	    scrollLines(p_SciCurrHandle, p_line - l_firstVisibleLineNo - 5);
}

void NppLocationSetter::setColumn(int p_column)
{
    HWND sciCurrHandle = m_npp.getCurrentScintilla();

    int currentLine = (int)::SendMessage(m_npp.npp, NPPM_GETCURRENTLINE, 0, 0);
    int destinationPosition = (int)::SendMessage(sciCurrHandle,SCI_FINDCOLUMN, (WPARAM)currentLine, (LPARAM)p_column);
    ::SendMessage(sciCurrHandle, SCI_GOTOPOS, (WPARAM)destinationPosition, 0);
    scrollToLine(sciCurrHandle, currentLine);
}

} // namespace NppPlugin
