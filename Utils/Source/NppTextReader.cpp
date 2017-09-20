#include "NppTextReader.hpp"
#include "Scintilla.h"

namespace NppPlugin
{

NppTextReader::NppTextReader(EditorData& p_npp)
 : m_npp(p_npp)
{
}

std::string NppTextReader::getCurrentWord() const
{
    HWND l_currentHScintilla = m_npp.getCurrentScintilla();
    int l_wordStart=0, l_wordEnd=0, l_currPos = 0;
    l_currPos = (int)::SendMessage(l_currentHScintilla, SCI_GETCURRENTPOS, 0, 0);
    l_wordStart = (int)::SendMessage(l_currentHScintilla, SCI_WORDSTARTPOSITION, (WPARAM)l_currPos, true);
    l_wordEnd = (int)::SendMessage(l_currentHScintilla, SCI_WORDENDPOSITION, (WPARAM)l_currPos, true);

    char l_currentWord[512] = "";
    struct TextRange tr = {{l_wordStart, l_wordEnd}, l_currentWord};
    ::SendMessage(l_currentHScintilla, SCI_GETTEXTRANGE, 0, (LPARAM)&tr);

    return l_currentWord;
}

} /* namespace NppPlugin */
