#include "Counters.hpp"
#include <exception>
#include <sstream>
#include <algorithm>
#include <numeric>

namespace Meas
{

void Counters::Counter::add(double p_value)
{
	measurements.push_back(p_value);
}

std::string Counters::Counter::print() const
{
	std::ostringstream buff;
	buff << "min: " << min() << ", max: " << max() << ", avg: " << avg() << ", mid: " << mid();
	return buff.str();
}

double Counters::Counter::min() const
{
	if (measurements.empty())
		return 0;
	return *std::min_element(measurements.begin(), measurements.end());
}
double Counters::Counter::max() const
{
	if (measurements.empty())
		return 0;
	return *std::max_element(measurements.begin(), measurements.end());
}
double Counters::Counter::avg() const
{
	if (measurements.empty())
		return 0;
	double sum = std::accumulate(measurements.begin(), measurements.end(), double(0));
	return (sum / measurements.size());
}
double Counters::Counter::mid() const
{
	if (measurements.empty())
		return 0;
	auto iter = measurements.begin();
	std::advance(iter, measurements.size() / 2);
	return *iter;
}

void Counters::addCounter(const std::string& p_name)
{
	counters.emplace(p_name, Counter());
}

Counters::Counter& Counters::get(const std::string& p_name)
{
	auto counter = counters.find(p_name);
	if (counter == counters.end())
		throw std::invalid_argument(std::string("No counter named ") + p_name);
	return counter->second;
}

}
