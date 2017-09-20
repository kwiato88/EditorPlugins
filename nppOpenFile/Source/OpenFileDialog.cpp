#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include <richedit.h>

#include "OpenFileDialog.hpp"
#include "OpenFileDialogDef.h"
#include "DialogMsgMatchers.hpp"
#include "findFile.hpp"

#include <iostream>

namespace WinApi
{

OpenFileDialog::OpenFileDialog(
        WinApi::InstanceHandle p_hInstance,
        WinApi::Handle p_parentWindow)
 : Dialog(p_hInstance, p_parentWindow, ResourceId(ID_OPEN_FILE_DIALOG))
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
    close(BUTTON_OK);
}

void OpenFileDialog::onCancleClick()
{
    close(BUTTON_CANCEL);
}

void OpenFileDialog::onGridDoubleClick()
{
    onOkClick();
}

void OpenFileDialog::onSearchClick()
{
	s_lastUsedNamePattern = m_fileNamePattern.getContent();
    m_gridRows = toSelectItems(findFiles(s_lastUsedNamePattern, m_searchDir, m_isCaseSensitiveSearch.isChecked()));
    m_gridControl.addRows(m_gridRows);
}

void OpenFileDialog::setSearchDir(const std::string& p_dir)
{
    m_searchDir = p_dir;
}

std::string OpenFileDialog::getSelectedFile() const
{
    return m_selectedFile;
}

std::string OpenFileDialog::s_lastUsedNamePattern = ".*";

} // namespace WinApi
