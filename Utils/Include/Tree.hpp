#pragma once

#include <functional>
#include <vector>

namespace Plugin
{

template<typename T>
class Tree
{
public:
	typedef std::function<std::vector<T>(const T&)> Children;

	Tree() : value(), children() {}
	Tree(const Tree& p_other) : value(p_other.value), children(p_other.children) {}
	Tree(const T& p_value, const Children& p_children) : value(p_value), children()
	{
		parse(p_children);
	}

	T value;
	std::vector<Tree> children;

private:
	void parse(const Children& p_children)
	{
		for (const auto& child : p_children(value))
			children.push_back(Tree(child, p_children));
	}
};

}