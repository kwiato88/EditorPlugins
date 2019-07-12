#pragma once

#include <memory>
#include <ctime>
#include <functional>
#include <boost/filesystem/path.hpp>

#include "ITagsReader.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{

template<typename T>
class Cached
{
public:
	Cached(std::function<std::time_t()> p_getDataLastModifyTime, std::function<T()> p_load,
		std::function<void()> p_cachReloadedObserver)
		: m_getDataLastModifyTime(p_getDataLastModifyTime), m_load(p_load),
		  m_cachReloadedObserver(p_cachReloadedObserver)
	{}

	void updateData()
	{
		auto lastModifyTime = m_getDataLastModifyTime();
		if (m_getDataLastModifyTime() > m_loadtime)
		{
			LOG_INFO << "Load cached data. Last load time: " << m_loadtime;
			m_loadtime = std::time(nullptr);
			m_data = m_load();
			m_cachReloadedObserver();
		}
	}
	T* operator->()
	{
		updateData();
		return &m_data;
	}
	T& get()
	{
		updateData();
		return m_data;
	}

private:
	std::function<std::time_t()> m_getDataLastModifyTime;
	std::function<T()> m_load;
	std::function<void()> m_cachReloadedObserver;
	std::time_t m_loadtime = {};
	T m_data = {};
};

class CachedTagsReader : public ITagsReader
{
public:
	CachedTagsReader(std::unique_ptr<ITagsReader> p_tagFileReader,
		const std::string& p_tagFilePath,
		std::function<void()> p_tagsLoadedObserver);

	std::vector<TagHolder> findTag(const std::string& p_tagName) const override;
	std::vector<TagHolder> findTag(TagMatcher p_matcher) const override;

private:
	const boost::filesystem::path m_tagFile;
	mutable Cached<std::vector<TagHolder>> m_tags;
	std::unique_ptr<ITagsReader> m_tagFileReader;
};

}