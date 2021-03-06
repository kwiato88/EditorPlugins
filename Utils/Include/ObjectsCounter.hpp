#pragma once

#include <string>
#include <sstream>
#include <boost/type_index.hpp>

namespace Meas
{

template <typename T>
struct ObjectsCounter
{
    static long int objectsCreated;
    static long int objectsAlive;

    ObjectsCounter()
    {
        ++objectsCreated;
        ++objectsAlive;
    }
    
    ObjectsCounter(const ObjectsCounter&)
    {
        ++objectsCreated;
        ++objectsAlive;
    }

	static std::string print()
	{
		std::ostringstream buff;
		buff << "[alive " << objectsAlive << ", created " << objectsCreated
			<< ", " << boost::typeindex::type_id<T>().pretty_name() << "]";
		return buff.str();
	}
protected:
    ~ObjectsCounter() // objects should never be removed through pointers of this type
    {
        --objectsAlive;
    }
};

template <typename T> long int ObjectsCounter<T>::objectsCreated( 0 );
template <typename T> long int ObjectsCounter<T>::objectsAlive( 0 );

}
