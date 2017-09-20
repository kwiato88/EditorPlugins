#ifndef NPP_LOCATION_GETTER_HPP
#define NPP_LOCATION_GETTER_HPP

#include "EditorData.hpp"
#include "ILocationGetter.hpp"

namespace NppPlugin
{

class NppLocationGetter :
	public Plugin::ILocationGetter
{
public:
	NppLocationGetter(EditorData& p_npp);

	std::string getFile() const;
	int getLine() const;
    int getColumn() const;

private:
	EditorData& m_npp;

};

} // namespace NppPlugin

#endif // NPP_LOCATION_GETTER
