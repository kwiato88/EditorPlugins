#include <string>
#include <algorithm>
#include <iterator>

#include <boost/algorithm/string/trim.hpp>

#include "TagsSelector.hpp"

#include <boost/lexical_cast.hpp>
#include <sstream>

namespace CTagsPlugin
{

namespace
{
class ToStringConverter
{
public:
	ToStringConverter(size_t p_maxNameLength, size_t p_maxPathLength)
		: m_maxNameLength(p_maxNameLength), m_maxPathLength(p_maxPathLength) {}

	std::string operator()(const Tag& p_tag);
private:
	void appendName(const Tag& p_tag);
	void appendPath(const Tag& p_tag);
	void appendAddr(const Tag& p_tag);

	std::string m_tag;
	size_t m_maxNameLength;
	size_t m_maxPathLength;
};

std::string ToStringConverter::operator()(const Tag& p_tag)
{
	m_tag.clear();
	appendName(p_tag);
	appendPath(p_tag);
	appendAddr(p_tag);

	return m_tag;
}

void ToStringConverter::appendName(const Tag& p_tag)
{
	std::string l_spaces(m_maxNameLength + 1 - p_tag.name.size(), ' ');
	m_tag += p_tag.name;
	m_tag += l_spaces;
}

void ToStringConverter::appendPath(const Tag& p_tag)
{
	std::string l_spaces(m_maxPathLength + 1 - p_tag.path.size(), ' ');
	m_tag += p_tag.path;
	m_tag += l_spaces;
}

std::string trim(std::string p_toTrim)
{
    boost::trim(p_toTrim);
    return p_toTrim;
}

void ToStringConverter::appendAddr(const Tag& p_tag)
{
	m_tag += trim(p_tag.addr);
}

Tag getFileName(const Tag& p_tag)
{
	Tag l_ret = p_tag;
	size_t l_pos = p_tag.path.find_last_of('\\');
	if(l_pos != std::string::npos)
		l_ret.path = l_ret.path.substr(l_pos + 1);
	return l_ret;
}

struct MaxLengthGetter
{
	MaxLengthGetter() { nameMaxLength = 0; pathMaxLength = 0; }

	void operator()(const Tag& p_tag)
	{
		nameMaxLength = ( p_tag.name.size() > nameMaxLength ? p_tag.name.size() : nameMaxLength );
		pathMaxLength = ( p_tag.path.size() > pathMaxLength ? p_tag.path.size() : pathMaxLength );
	}

	size_t nameMaxLength;
	size_t pathMaxLength;
};
} // namespace

TagsSelector::TagsSelector(boost::shared_ptr<Plugin::IItemsSelector> p_selector)
	: m_selector(p_selector)
{
}

int TagsSelector::selectTag(const std::vector<TagHolder>& p_tags)
{
	std::vector<TagHolder> l_tags = cutFilesDirs(p_tags);
	MaxLengthGetter l_lenghtGetter = std::for_each(
		l_tags.begin(),
		l_tags.end(),
		MaxLengthGetter());

	std::vector<std::string> l_tagsStrings;

	std::transform(
		l_tags.begin(),
		l_tags.end(),
		std::back_inserter(l_tagsStrings),
		ToStringConverter(l_lenghtGetter.nameMaxLength, l_lenghtGetter.pathMaxLength));

	return m_selector->select(l_tagsStrings);
}

std::vector<TagHolder> TagsSelector::cutFilesDirs(const std::vector<TagHolder>& p_tags)
{
	std::vector<TagHolder> l_ret;
	std::transform(
		p_tags.begin(),
		p_tags.end(),
		std::back_inserter(l_ret),
		&getFileName);

	return l_ret;
}

} // namespace CTagsPlugin
