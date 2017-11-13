#pragma once

#include <functional>
#include <vector>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace Plugin
{

template<typename T>
class Tree
{
public:
	typedef std::function<std::vector<T>(const T&)> Children;

	Tree() : value(), children(), parent(nullptr) {}
	Tree(const Tree& p_other) : value(p_other.value), children(p_other.children), parent(p_other.parent) {}

	static Tree build(const T& p_rootValue, const Children& p_children)
	{
		return Tree(p_rootValue, p_children);
	}
	static Tree buildWithoutCycles(const T& p_rootValue, const Children& p_children, const T& p_valueWhenCycleDetected)
	{
		return Tree(p_rootValue, p_children, ProtectFromCycles{ p_valueWhenCycleDetected });
	}

	std::vector<T> childrenValues() const
	{
		std::vector<T> values;
		boost::range::copy(
			children | boost::adaptors::transformed([&](const auto& p) { return p.value; }),
			std::back_inserter(values));
		return values;
	}

	T value;
	std::vector<Tree> children;

private:
	struct ProtectFromCycles
	{
		T defaultValue;
	};

	const Tree* parent;

	Tree(const T& p_value, const Tree* p_partent)
		: value(p_value), children(), parent(p_partent)
	{}
	Tree(const T& p_value, const Children& p_children, const Tree* p_parent = nullptr)
		: value(p_value), children(), parent(p_parent)
	{
		parse(p_children);
	}
	Tree(const T& p_value, const Children& p_children, const ProtectFromCycles& p_default, const Tree* p_parent = nullptr)
		: value(p_value), children(), parent(p_parent)
	{
		parseWithoutCycles(p_children, p_default);
	}
	void parse(const Children& p_children)
	{
		for (const auto& child : p_children(value))
			children.push_back(Tree(child, p_children, this));
	}
	void parseWithoutCycles(const Children& p_children, const ProtectFromCycles& p_default)
	{
		if (hasTheSameValueInBranchUp())
		{
			children = std::vector<Tree>{ Tree(p_default.defaultValue, this) };
			return;
		}
		for (const auto& child : p_children(value))
			children.push_back(Tree(child, p_children, p_default, this));
	}
	bool hasTheSameValueInBranchUp() const
	{
		return hasValueInBranchUp(value);
	}
	bool hasValueInBranchUp(const T p_value) const
	{
		if (parent == nullptr)
			return false;
		if (parent->value == p_value)
			return true;
		return parent->hasValueInBranchUp(p_value);
	}
};

}
