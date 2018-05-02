#include "NppEditor.hpp"
#include "OpenFileException.hpp"
#include "Notepad_plus_msgs.h"
#include "Scintilla.h"

namespace NppPlugin
{
namespace
{
int getFirstVisableLine(WinApi::Handle p_SciCurrHandle)
{
    return (int)::SendMessage(p_SciCurrHandle, SCI_GETFIRSTVISIBLELINE, 0, 0);
}
void scrollLines(WinApi::Handle p_SciCurrHandle, int p_lines)
{
    ::SendMessage(p_SciCurrHandle, SCI_LINESCROLL, 0, p_lines);
}
void scrollToLine(WinApi::Handle p_SciCurrHandle, int p_line)
{
    int firstVisibleLineNo = getFirstVisableLine(p_SciCurrHandle);
	if (p_line > firstVisibleLineNo)
	    scrollLines(p_SciCurrHandle, p_line - firstVisibleLineNo - 5);
}
}

WinApi::Handle Editor::getCurrentScintilla() const
{
	int currentEdit;
	::SendMessage(npp, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);

	return (currentEdit == 0) ? scintillaMain : scintillaSecond;
}

std::string Editor::getFile() const
{
	TCHAR filePath[_MAX_PATH];
	char filePathBuff[_MAX_PATH];
	::SendMessage(npp, NPPM_GETFULLCURRENTPATH, 0, (LPARAM)filePath);
	wcstombs(filePathBuff, filePath, _MAX_PATH - 1);

	return filePathBuff;
}

int Editor::getLine() const
{
	auto scintilla = getCurrentScintilla();

	int currPos = (int)::SendMessage(scintilla, SCI_GETCURRENTPOS, 0, 0);
	int lineNum = (int)::SendMessage(scintilla, SCI_LINEFROMPOSITION, (WPARAM)currPos, 0);

	return lineNum;
}

int Editor::getColumn() const
{
	return (int)::SendMessage(npp, NPPM_GETCURRENTCOLUMN, 0, 0);
}

void Editor::setFile(const std::string& p_filePath)
{
	TCHAR filePath[_MAX_PATH];
	mbstowcs(filePath, p_filePath.c_str(), _MAX_PATH - 1);
	if (!::SendMessage(npp, NPPM_DOOPEN, 0, (LPARAM)filePath))
		throw Plugin::OpenFileException("Editor can't open file: " + p_filePath);
}

void Editor::setLine(int p_line)
{
	auto scintilla = getCurrentScintilla();

	::SendMessage(scintilla, SCI_GOTOLINE, (WPARAM)p_line, 0);
	scrollToLine(scintilla, p_line);
}

void Editor::setColumn(int p_column)
{
	auto scintilla = getCurrentScintilla();

	int currentLine = (int)::SendMessage(npp, NPPM_GETCURRENTLINE, 0, 0);
	int destinationPosition = (int)::SendMessage(scintilla, SCI_FINDCOLUMN, (WPARAM)currentLine, (LPARAM)p_column);
	::SendMessage(scintilla, SCI_GOTOPOS, (WPARAM)destinationPosition, 0);
	scrollToLine(scintilla, currentLine);
}

std::string Editor::getCurrentWord() const
{
	auto scintilla = getCurrentScintilla();
	int wordStart = 0, wordEnd = 0, currPos = 0;
	currPos = (int)::SendMessage(scintilla, SCI_GETCURRENTPOS, 0, 0);
	wordStart = (int)::SendMessage(scintilla, SCI_WORDSTARTPOSITION, (WPARAM)currPos, true);
	wordEnd = (int)::SendMessage(scintilla, SCI_WORDENDPOSITION, (WPARAM)currPos, true);

	char currentWord[512] = "";
	struct TextRange tr = { { wordStart, wordEnd }, currentWord };
	::SendMessage(scintilla, SCI_GETTEXTRANGE, 0, (LPARAM)&tr);

	return currentWord;
}

}
