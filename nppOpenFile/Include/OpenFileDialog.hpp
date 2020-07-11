#ifndef GRID_DIALOG_HPP
#define GRID_DIALOG_HPP

#include <windows.h>
#include <string>
#include <vector>

#include "Dialog.hpp"
#include "ControlGrid.hpp"
#include "ControlEdit.hpp"
#include "ControlCheckBox.hpp"
#include "findFile.hpp"

namespace WinApi
{

class OpenFileDialog : public Dialog
{
public:
    OpenFileDialog(WinApi::InstanceHandle p_hInstance, WinApi::Handle p_parentWindow, Dirs& p_searchDirs);

    void setSearchDirs(const std::vector<std::string>& p_paths);
    std::string getSelectedFile() const;

private:
    void onInit() override;
	bool showContextMenu(int p_xPos, int p_yPos) override;

    void onOkClick();
    void onCancleClick();
    void onGridDoubleClick();
    void onSearchClick();
	void copySelectedFileName();
	void copySelectedFilePath();
	void copyAllFilesNames();
	void copyAllFilesPaths();

    Control::RcFileGrid m_gridControl;
	Control::Edit m_fileNamePattern;
	Control::CheckBox m_isCaseSensitiveSearch;
	Control::CheckBox m_useRegeq;

    std::vector<std::string> m_gridLabels;
    std::vector<std::vector<std::string> > m_gridRows;
    std::vector<std::string> m_searchDirsPaths;
    std::string m_selectedFile;
	Dirs m_searchDirs;

	static std::string s_lastUsedNamePattern;
};

} // namespace WinApi

#endif /* GRID_DIALOG_HPP */
