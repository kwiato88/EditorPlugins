#ifndef GRID_DIALOG_HPP
#define GRID_DIALOG_HPP

#include <windows.h>
#include <string>
#include <vector>

#include "Dialog.hpp"
#include "ControlGrid.hpp"
#include "ControlEdit.hpp"
#include "ControlCheckBox.hpp"

namespace WinApi
{

class OpenFileDialog : public Dialog
{
public:
    static const int BUTTON_OK = 1;
    static const int BUTTON_CANCEL = 0;

    OpenFileDialog(WinApi::InstanceHandle p_hInstance, WinApi::Handle p_parentWindow);

    void setSearchDirs(const std::vector<std::string>& p_dirs);
    std::string getSelectedFile() const;

private:
    void onInit();

    void onOkClick();
    void onCancleClick();
    void onGridDoubleClick();
    void onSearchClick();

    Control::RcFileGrid m_gridControl;
	Control::Edit m_fileNamePattern;
	Control::CheckBox m_isCaseSensitiveSearch;

    std::vector<std::string> m_gridLabels;
    std::vector<std::vector<std::string> > m_gridRows;
    std::vector<std::string> m_searchDirs;
    std::string m_selectedFile;

	static std::string s_lastUsedNamePattern;
};

} // namespace WinApi

#endif /* GRID_DIALOG_HPP */
