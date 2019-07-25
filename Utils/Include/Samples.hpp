#pragma once

#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <boost/timer.hpp>

namespace Meas
{

template <typename Tag>
class Samples
{
public:
	static void add(double p_value);
	static std::string print();
private:
	static double minElem();
	static double maxElem();
	static double avgElem();
	static double midElem();

	static std::list<double> measurements;
};

template <typename Tag>
class ExecutionTimeSample
{
public:
	~ExecutionTimeSample()
	{
		auto time = tm.elapsed();
		Meas::Samples<Tag>::add(time);
	}
private:
	boost::timer tm;
};

template <typename Tag>
void Samples<Tag>::add(double p_value)
{
	measurements.push_back(p_value);
	measurements.sort();
}

template <typename Tag>
std::string Samples<Tag>::print()
{
	std::ostringstream buff;
	buff << "[min: " << minElem() << ", max: " << maxElem()
		<< ", avg: " << avgElem() << ", mid: " << midElem()
		<< ", count: " << measurements.size() << ", " << Tag::name() << "]";
	return buff.str();
}

template <typename Tag>
double Samples<Tag>::minElem()
{
	if (measurements.empty())
		return 0;
	return *std::min_element(measurements.begin(), measurements.end());
}

template <typename Tag>
double Samples<Tag>::maxElem()
{
	if (measurements.empty())
		return 0;
	return *std::max_element(measurements.begin(), measurements.end());
}

template <typename Tag>
double Samples<Tag>::avgElem()
{
	if (measurements.empty())
		return 0;
	double sum = std::accumulate(measurements.begin(), measurements.end(), double(0));
	return (sum / measurements.size());
}

template <typename Tag>
double Samples<Tag>::midElem()
{
	if (measurements.empty())
		return 0;
	auto iter = measurements.begin();
	std::advance(iter, measurements.size() / 2);
	return *iter;
}

template <typename Tag> std::list<double> Samples<Tag>::measurements;

}
