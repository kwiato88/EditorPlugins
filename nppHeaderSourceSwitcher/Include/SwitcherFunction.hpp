#ifndef NPP_FILE_SWITCHER_HPP
#define NPP_FILE_SWITCHER_HPP

#include "NppLocationGetter.hpp"
#include "NppLocationSetter.hpp"

namespace HeaderSourceSwitcher
{

void switchFile(Plugin::ILocationGetter& p_getter, Plugin::ILocationSetter& p_setter);

} // HeaderSourceSwitcher

#endif NPP_FILE_SWITCHER_HPP
