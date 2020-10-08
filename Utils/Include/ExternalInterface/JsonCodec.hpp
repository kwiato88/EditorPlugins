#pragma once

#include <vector>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Messaging
{
struct JsonCodec
{

using BufferType = std::vector<char>;

template <typename T>
static BufferType encode(const T& msg)
{
	std::stringstream buff;
	boost::property_tree::json_parser::write_json(buff, msg.exportMsg());
	BufferType output;
	for (char c : buff.str())
		output.push_back(c);
    return output;
}

template <typename T>
static T decode(const BufferType& p_buffer)
{
	std::stringstream buff;
	for (char c : p_buffer)
		buff << c;
	return decode<T>(buff);
}

template <typename T>
static T decode(const char* p_buffer, std::size_t p_bufferSize)
{
	std::stringstream buff;
	for (unsigned int i = 0; i < p_bufferSize; ++i)
		buff << p_buffer[i];
	return decode<T>(buff);
}

template <typename T>
static T decode(std::stringstream& buff)
{
	boost::property_tree::ptree data;
	boost::property_tree::json_parser::read_json(buff, data);
	T msg = {};
	msg.importMsg(data);
	return msg;
}

};
}
