#pragma once

#include "SwitchFile.hpp"

namespace HeaderSourceSwitcher
{

std::pair<std::vector<FileSwitchInfo>, std::vector<std::string>> load(const Path& p_file);
void save(const std::vector<FileSwitchInfo>& p_info, const Path& p_file);

}
