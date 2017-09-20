#ifndef I_LOCATION_SETTER_HPP
#define I_LOCATION_SETTER_HPP

#include <cstdio>
#include <string>

namespace Plugin
{
	struct ILocationSetter
	{
		virtual ~ILocationSetter() {}

		virtual void setFile(std::string p_filePath) = 0;
		virtual void setLine(int p_line) = 0;
        virtual void setColumn(int p_column) = 0;
	};
} // namespace Plugin

#endif // I_LOCATION_SETTER
