#pragma once

#include <string>
#include <vector>
#include <functional>

#include "Tree.hpp"
#include "IIncludeBrowser.hpp"

namespace IncludeBrowser
{

using FileHierarchy = Plugin::Tree<std::string>;

FileHierarchy buildIncludedHierarchy(const std::string& p_file, const IBrowser& p_files);
FileHierarchy buildIncludersHierarchy(const std::string& p_file, const IBrowser& p_files);

}
