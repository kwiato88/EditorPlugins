#pragma once

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
protected:
    ~ObjectsCounter() // objects should never be removed through pointers of this type
    {
        --objectsAlive;
    }
};

template <typename T> long int ObjectsCounter<T>::objectsCreated( 0 );
template <typename T> long int ObjectsCounter<T>::objectsAlive( 0 );

}
