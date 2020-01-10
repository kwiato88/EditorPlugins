#pragma once

#include <string>
#include <sstream>

namespace Meas
{

template <typename Tag>
class Counter
{
public:
	static void increment();
	static unsigned int get();
	static std::string print();

private:
	static unsigned int value;
};

template <typename Tag> unsigned int Counter<Tag>::value = 0;

template <typename Tag>
void Counter<Tag>::increment()
{
	++value;
}

template <typename Tag>
unsigned int Counter<Tag>::get()
{
	return value;
}

template <typename Tag>
std::string Counter<Tag>::print()
{
	std::ostringstream buff;
	buff << "[" << value << ", " << Tag::name() << "]";
	return buff.str();
}

template <typename NumeratorTag, typename DenominatorTag>
struct Percent
{
	static float value()
	{
		return static_cast<float>(Counter<NumeratorTag>::get())
			/ static_cast<float>(Counter<DenominatorTag>::get());
	}
	static std::string print()
	{
		std::ostringstream buff;
		buff << Counter<NumeratorTag>::print() << " " << Counter<DenominatorTag>::print() << " [";
		appendValue(buff);
		buff << "]";
		return buff.str();
	}

private:
	static void appendValue(std::ostream& p_buff)
	{
		if (Counter<DenominatorTag>::get() == 0)
			p_buff << "NAN - division by 0";
		else
			p_buff << value();
	}
};

}
