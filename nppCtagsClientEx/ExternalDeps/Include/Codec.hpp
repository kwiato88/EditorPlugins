#pragma once

#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

namespace Messaging
{

typedef std::vector<char> BufferType;

template <typename T>
BufferType encode(const T& p_data)
{
    BufferType buffer;
    boost::iostreams::stream<boost::iostreams::back_insert_device<BufferType> > outputStream(buffer);
    boost::archive::binary_oarchive archive(outputStream);
    archive << p_data;
    outputStream.flush();
    return buffer;
}

template <typename T>
T decode(const BufferType& p_buffer)
{
    boost::iostreams::basic_array_source<char> source(p_buffer.data(),p_buffer.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source <char> > inputStream(source);
    boost::archive::binary_iarchive archive(inputStream);
    T data;
    archive >> data;
    return data;
}

template <typename T>
T decode(const char* p_buffer, std::size_t p_bufferSize)
{
    boost::iostreams::basic_array_source<char> source(p_buffer,p_bufferSize);
    boost::iostreams::stream<boost::iostreams::basic_array_source <char> > inputStream(source);
    boost::archive::binary_iarchive archive(inputStream);
    T data;
    archive >> data;
    return data;
}

}
