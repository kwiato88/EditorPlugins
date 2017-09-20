#ifndef ITAGSREADER_HPP
#define ITAGSREADER_HPP

#include <string>
#include <vector>
#include <functional>

#include "TagHolder.hpp"

namespace CTagsPlugin
{

typedef std::function<bool (const Tag&)> TagMatcher;

struct ITagsReader
{
	virtual ~ITagsReader() {}

	virtual std::vector<TagHolder> findTag(const std::string& p_tagName) const = 0;
    virtual std::vector<TagHolder> findTag(TagMatcher p_matcher) const = 0;
};

} // namespace CTagsPlugin

#endif
