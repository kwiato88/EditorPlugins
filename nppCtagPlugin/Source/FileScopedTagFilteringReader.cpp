#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <iterator>

#include "FileScopedTagFilteringReader.hpp"
#include "Tag.hpp"

namespace CTagsPlugin
{
namespace
{
std::string fixPath(std::string p_path)
{
	boost::algorithm::replace_all(p_path, "\\/", "/");
	boost::algorithm::replace_all(p_path, "\\\\", "\\");
	return p_path;
}
}

FileScopedTagFilteringReader::FileScopedTagFilteringReader(
	Plugin::Editor& p_editor,
	std::unique_ptr<ITagsReader> p_reader)
	: m_editor(p_editor), m_reader(std::move(p_reader))
{}

std::vector<TagHolder> FileScopedTagFilteringReader::findTag(const std::string& p_tagName) const
{
	return filter(m_reader->findTag(p_tagName));
}

std::vector<TagHolder> FileScopedTagFilteringReader::findTag(TagMatcher p_matcher) const
{
	return m_reader->findTag(p_matcher);
}

std::vector<TagHolder> FileScopedTagFilteringReader::filter(const std::vector<TagHolder> p_tags) const
{
	std::string file = m_editor.getFile();
	std::vector<TagHolder> filtered;
	boost::range::copy(
		p_tags | boost::adaptors::filtered(
					[&](const Tag& p){ return ( p.isLocalIn(file) || (fixPath(p.getAddr().filePath()) == file)); }),
		std::back_inserter(filtered));
	return filtered;
}

}
