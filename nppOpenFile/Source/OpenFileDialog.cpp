#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include <richedit.h>
#include <iterator>
#include <algorithm>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>

#include "OpenFileDialog.hpp"
#include "OpenFileDialogDef.h"
#include "DialogMsgMatchers.hpp"
#include "findFile.hpp"
#include "ContextMenu.hpp"
#include "Clipboard.hpp"

#include <iostream>

namespace WinApi
{

std::vector<std::vector<std::string>> toSelectItems(const std::vector<boost::filesystem::path>& p_files)
{
    std::vector<std::vector<std::string>> selectionItems;
    boost::range::transform(p_files, std::back_inserter(selectionItems),
        [](const boost::filesystem::path& p)
        {
			return std::vector<std::string>{ p.filename().string(), p.string() };
        });
    std::sort(selectionItems.begin(), selectionItems.end(),
        [](const std::vector<std::string>& lhs, const std::vector<std::string>& rhs) { return lhs.at(1) < rhs.at(1); } );
    return selectionItems;
}

OpenFileDialog::OpenFileDialog(
        WinApi::InstanceHandle p_hInstance,
        WinApi::Handle p_parentWindow,
        Dirs& p_searchDirs)
 : Dialog(p_hInstance, p_parentWindow, ResourceId(ID_OPEN_FILE_DIALOG)), m_searchDirs(p_searchDirs)
{
	m_gridLabels = { "File", "Path" };
    registerHandler(MsgMatchers::ButtonClick(IDOK),     std::bind(&OpenFileDialog::onOkClick, this));
    registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&OpenFileDialog::onCancleClick, this));
    registerHandler(MsgMatchers::ButtonClick(ID_OPEN_FILE_SEARCH_BUTTON), std::bind(&OpenFileDialog::onSearchClick, this));
    registerHandler(MsgMatchers::DoubleClick(m_gridControl), std::bind(&OpenFileDialog::onGridDoubleClick, this));
}

void OpenFileDialog::onInit()
{
    m_gridControl.init(getItem(ResourceId(ID_OPEN_FILE_FILES)));
    m_gridControl.addColumns(m_gridLabels);
	m_fileNamePattern.init(getItem(ResourceId(ID_OPEN_FILE_PATTERN)));
	m_fileNamePattern.setContent(s_lastUsedNamePattern);
	m_fileNamePattern.setFocus();
	m_fileNamePattern.setSelection(0, m_fileNamePattern.getLineLength(0));
	m_isCaseSensitiveSearch.init(getItem(ResourceId(ID_OPEN_FILE_IS_CASE_SENSITIVE)));
	m_useRegeq.init(getItem(ResourceId(ID_OPEN_FILE_IS_REGEQ)));
	setTitle("Open file");
}

void OpenFileDialog::onOkClick()
{
    try
    {
        m_selectedFile = m_gridRows.at(m_gridControl.getSelectedRowIndex()).at(1);
    }
    catch(const std::exception&)
    {
        m_selectedFile = "";
    }
    close(RESULT_OK);
}

void OpenFileDialog::onCancleClick()
{
    close(RESULT_CANCEL);
}

void OpenFileDialog::onGridDoubleClick()
{
    onOkClick();
}

void OpenFileDialog::onSearchClick()
{
	s_lastUsedNamePattern = m_fileNamePattern.getContent();
	m_gridRows.clear();
	for (const auto& dir : m_searchDirsPaths)
		boost::range::copy(
			toSelectItems(m_searchDirs.getFiles(Dirs::Pattern{ s_lastUsedNamePattern, m_isCaseSensitiveSearch.isChecked(), m_useRegeq.isChecked() }, dir)),
			std::back_inserter(m_gridRows));
    m_gridControl.addRows(m_gridRows);
	redraw();
}

void OpenFileDialog::setSearchDirs(const std::vector<std::string>& p_paths)
{
	m_searchDirsPaths = p_paths;
}

std::string OpenFileDialog::getSelectedFile() const
{
    return m_selectedFile;
}

bool OpenFileDialog::showContextMenu(int p_xPos, int p_yPos)
{
	if (m_gridControl.isWithin(p_xPos, p_yPos))
	{
		ContextMenu menu(m_self);
		menu.add(ContextMenu::Item{ "Copy selected file name", std::bind(&OpenFileDialog::copySelectedFileName, this) });
		menu.add(ContextMenu::Item{ "Copy selected file path", std::bind(&OpenFileDialog::copySelectedFilePath, this) });
		menu.add(ContextMenu::Item{ "Copy all files names", std::bind(&OpenFileDialog::copyAllFilesNames, this) });
		menu.add(ContextMenu::Item{ "Copy all files paths", std::bind(&OpenFileDialog::copyAllFilesPaths, this) });
		menu.show(p_xPos, p_yPos);
		return true;
	}
	return false;
}
void OpenFileDialog::copySelectedFileName()
{
	auto selectedIdx = m_gridControl.getSelectedRowIndex();
	if (selectedIdx != -1)
		Clipboard::set(Clipboard::String(m_gridRows.at(selectedIdx).at(0)));
}
void OpenFileDialog::copySelectedFilePath()
{
	auto selectedIdx = m_gridControl.getSelectedRowIndex();
	if (selectedIdx != -1)
		Clipboard::set(Clipboard::String(m_gridRows.at(selectedIdx).at(1)));
}
void OpenFileDialog::copyAllFilesNames()
{
	std::string toCopy;
	for (const auto& item : m_gridRows)
		toCopy += item.at(0) + '\n';
	Clipboard::set(Clipboard::String(toCopy));
}
void OpenFileDialog::copyAllFilesPaths()
{
	std::string toCopy;
	for (const auto& item : m_gridRows)
		toCopy += item.at(1) + '\n';
	Clipboard::set(Clipboard::String(toCopy));
}

std::string OpenFileDialog::s_lastUsedNamePattern = ".*";

} // namespace WinApi
