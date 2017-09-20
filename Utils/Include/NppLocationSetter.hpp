#ifndef NPP_LOCATION_SETTER_HPP
#define NPP_LOCATION_SETTER_HPP

#include "EditorData.hpp"
#include "Ilocationsetter.hpp"

namespace NppPlugin
{

class NppLocationSetter :
	public Plugin::ILocationSetter
{
public:
	NppLocationSetter(EditorData& p_npp);

	/**
	 * @throw LocationSetterException
	 */
	void setFile(std::string p_filePath);
	void setLine(int p_line);
    void setColumn(int p_column);

private:
	void scrollLines(HWND p_SciCurrHandle, int p_lines);
	int getFirstVisableLine(HWND p_SciCurrHandle);
    void scrollToLine(HWND p_SciCurrHandle, int p_line);

	EditorData& m_npp;
};

} // namespace NppPlugin

#endif // NPP_LOCATION_SETTER
