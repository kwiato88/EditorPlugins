#pragma once

#include <string>
#include <map>
#include <list>
#include <memory>

namespace Meas
{

class Counters
{
public:
	class Counter
	{
	public:
		void add(double p_value);
		std::string print() const;
	private:
		double min() const;
		double max() const;
		double avg() const;
		double mid() const;

		std::list<double> measurements;
	};

public:
	void addCounter(const std::string& p_name);
	Counter& get(const std::string& p_name);

private:
	std::map<std::string, Counter> counters;
};

}
