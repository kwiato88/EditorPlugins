#ifndef EXTENSIONFIELDSPARSER_HPP
#define EXTENSIONFIELDSPARSER_HPP

#include <string>
#include <vector>
#include "TagHolder.hpp"
#include "BuildTag.hpp"
#include "FillBaseTag.hpp"

namespace CTagsPlugin
{

std::vector<Field> parseExtensionFields(const std::string& p_tagFileLineComment);

TagHolder parseTag(const std::string& p_TagFileLine);

} // CTagsPlugin

#endif // EXTENSIONFIELDSPARSER_HPP
