#pragma once

#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace PtreeUtils
{

template<typename T>
boost::property_tree::ptree toPtree(const T& p_obj)
{
	boost::property_tree::ptree tree;
	tree.put<T>("", p_obj);
	return tree;
}
template<typename T>
boost::property_tree::ptree toPtree(const std::vector<T>& p_vec)
{
	boost::property_tree::ptree tree;
	for (const auto& item : p_vec)
		tree.push_back(std::make_pair("", toPtree(item)));
	return tree;
}

template <typename T>
struct Type {};

template<typename T>
T fromPtree(Type<T>, const boost::property_tree::ptree& p_obj)
{
	return p_obj.get<T>("");
}
template<typename T>
std::vector<T> fromPtree(Type<std::vector<T>>, const boost::property_tree::ptree& p_vec)
{
	std::vector<T> items;
	for (const auto& item : p_vec)
		items.push_back(fromPtree(Type<T>{}, item.second));
	return items;
}
template<typename T>
T fromPtree(const boost::property_tree::ptree& p_obj)
{
	return fromPtree(Type<T>{}, p_obj);
}

class ToPtree
{
public:
	template <typename T>
	ToPtree& add(const std::string& name, const T& object)
	{
		tree.put<T>(name, object);
	}
	template <typename T>
	ToPtree& add(const std::string& name, const std::vector<T>& container)
	{
		tree.add_child(name, PtreeUtils::toPtree(container));
	}
	inline boost::property_tree::ptree get() const { return tree; }
private:
	boost::property_tree::ptree tree;
};

class FromPtree
{
public:
	FromPtree(const boost::property_tree::ptree& t) : tree(t) {}
	template <typename T>
	T get(const std::string& name) const
	{
		return get(Type<T>{}, name);
	}
	
private:
	template <typename T>
	T get(Type<T>, const std::string& name) const
	{
		return tree.get<T>(name);
	}
	template <typename T>
	std::vector<T> get(Type<std::vector<T>>, const std::string& name) const
	{
		return PtreeUtils::fromPtree<std::vector<T>>(tree.get_child(name));
	}

	const boost::property_tree::ptree& tree;
};

}
