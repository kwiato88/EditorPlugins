#pragma once

#include <string>

namespace CTagsPlugin
{

class Tag;
class ExtensionFields;
void fillBaseTag(Tag& p_outTag, const std::string& p_TagFileLine, const ExtensionFields& p_fields);

}
