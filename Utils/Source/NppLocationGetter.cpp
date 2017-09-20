#include "NppLocationGetter.hpp"
#include "Notepad_plus_msgs.h"
#include "Scintilla.h"

namespace NppPlugin
{

NppLocationGetter::NppLocationGetter(EditorData& p_npp)
	: m_npp(p_npp)
{
}

std::string NppLocationGetter::getFile() const
{
	TCHAR l_filePath[_MAX_PATH];
	char l_filePathBuff[_MAX_PATH];
	::SendMessage(m_npp.npp, NPPM_GETFULLCURRENTPATH, 0, (LPARAM) l_filePath);
	wcstombs(l_filePathBuff, l_filePath, _MAX_PATH - 1);
	
	return l_filePathBuff;
}

int NppLocationGetter::getLine() const
{
	HWND l_hSciCurrHandle = m_npp.getCurrentScintilla();

	int l_currPos = (int)::SendMessage(l_hSciCurrHandle, SCI_GETCURRENTPOS, 0, 0);
	int l_lineNum = (int)::SendMessage(l_hSciCurrHandle, SCI_LINEFROMPOSITION, (WPARAM)l_currPos, 0);

	return l_lineNum;
}

int NppLocationGetter::getColumn() const
{
    return (int)::SendMessage(m_npp.npp, NPPM_GETCURRENTCOLUMN, 0, 0);
}

} // namespace NppPlugin
