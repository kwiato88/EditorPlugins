#pragma once

#include <ostream>

namespace Printable
{
    
template<typename T>
class Element
{
public:
    Element(const T& p_elem,
		const std::string& p_prefix = "{", const std::string& p_suffix = "}", const std::string& p_separator = ", ")
     : elem(p_elem), prefix(p_prefix), suffix(p_suffix), separator(p_separator)
     {}
    void print(std::ostream& p_out) const
    {
        printImpl(p_out, elem);
    }

private:
    template<typename Type>
    void printImpl(std::ostream& p_out, const Type& p_elem) const
    {
        p_out << p_elem;
    }
    template<typename Type1, typename Type2>
    void printImpl(std::ostream& p_out, const std::pair<Type1, Type2>& p_elem) const
    {
        p_out << prefix << p_elem.first << separator << p_elem.second << suffix;
    }
    const T& elem;
	std::string prefix;
	std::string suffix;
	std::string separator;
};

template <typename T>
inline Element<T> makeElem(const T& p_elem, const std::string& p_prefix = "{", const std::string& p_suffix = "}", const std::string& p_separator = ", ")
{
    return Element<T>(p_elem, p_prefix, p_suffix, p_separator);
}

template<typename Iterator>
class Range
{
public:
    Range(const Iterator& p_begin, const Iterator& p_end,
		const std::string& p_prefix = "{", const std::string& p_suffix = "}", const std::string& p_separator = ", ")
     : begin(p_begin), end(p_end), prefix(p_prefix), suffix(p_suffix), separator(p_separator)
    {}

    void print(std::ostream& p_out) const
    {
        p_out << prefix;
        if(begin != end)
            printElements(p_out);
        p_out << suffix;
    }

private:
    void printElements(std::ostream& p_out) const
    {
        auto elem = begin;
        p_out << makeElem(*elem);
        while(++elem != end)
            p_out << separator << makeElem(*elem);
    }
    
    Iterator begin;
    Iterator end;
	std::string prefix;
	std::string suffix;
	std::string separator;
};

template<typename Iterator>
Range<Iterator> makeRange(Iterator p_begin, Iterator p_end, const std::string& p_prefix = "{", const std::string& p_suffix = "}", const std::string& p_separator = ", ")
{
    return Range<Iterator>(p_begin, p_end, p_prefix, p_suffix, p_separator);
}

template<typename Container>
Range<typename Container::const_iterator> makeRange(const Container& p_elems, const std::string& p_prefix = "{", const std::string& p_suffix = "}", const std::string& p_separator = ", ")
{
    return makeRange(std::begin(p_elems), std::end(p_elems), p_prefix, p_suffix, p_separator);
}

template<typename T, std::size_t N>
Range<T*> makeRange(T (&p_array) [N], const std::string& p_prefix = "{", const std::string& p_suffix = "}", const std::string& p_separator = ", ")
{
    return makeRange(std::begin(p_array), std::end(p_array), p_prefix, p_suffix, p_separator);
}

template <typename T>
std::ostream& operator<<(std::ostream& p_out, const Printable::Element<T>& p_elem)
{
	p_elem.print(p_out);
	return p_out;
}

template<typename Iterator>
std::ostream& operator<<(std::ostream& p_out, const Printable::Range<Iterator>& p_range)
{
	p_range.print(p_out);
	return p_out;
}

}
