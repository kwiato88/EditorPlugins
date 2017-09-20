#include "EditorData.hpp"
#include "Notepad_plus_msgs.h"

namespace NppPlugin
{

WinApi::Handle EditorData::getCurrentScintilla()
{
	int currentEdit;
	::SendMessage(npp, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);

	return (currentEdit == 0) ? scintillaMain : scintillaSecond;
}

}
