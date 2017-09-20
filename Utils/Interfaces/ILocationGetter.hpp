#ifndef I_LOCATION_GETTER_HPP
#define I_LOCATION_GETTER_HPP

#include <cstdio>
#include <string>

namespace Plugin
{
	struct ILocationGetter
	{
		virtual ~ILocationGetter() {}

		virtual std::string getFile() const = 0;
		virtual int getLine() const = 0;
        virtual int getColumn() const = 0;
	};
} // namespace Plugin

#endif // I_LOCATION_GETTER
