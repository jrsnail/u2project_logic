#ifndef __U2SimpleObjectManager_H__
#define __U2SimpleObjectManager_H__

#include "U2Prerequisites.h"
#include "U2ObjectCollection.h"


U2EG_NAMESPACE_BEGIN



template <class T> 
class SimpleObjectManager : public ObjectCollection<T>
{
public:
    /** Default constructor - should never get called by a client app.
    */
    SimpleObjectManager();

    /** Default destructor.
    */
    virtual ~SimpleObjectManager();

    T* createObject(const String& type, const String& name = BLANK);
	

protected:
	ObjectMapIterator retrieveAllObjectsByName(const String& name)
	{
		return ObjectCollection<T>::retrieveAllObjectsByName(name);
	}

	ObjectMapIterator retrieveAllObjectsByType(const String& type)
	{
		return ObjectCollection<T>::retrieveAllObjectsByType(type);
	}

	T* retrieveObjectByType(const String& type)
	{
		return ObjectCollection<T>::retrieveObjectByType(type);
	}

	T* retrieveObjectByGuid(const String& guid)
	{
		return ObjectCollection<T>::retrieveObjectByGuid(guid);
	}

	ObjectMapIterator retrieveAllObjectsByTN(const String& type, const String& name)
	{
		return ObjectCollection<T>::retrieveAllObjectsByTN(type, name);
	}

	T* retrieveObjectByTN(const String& type, const String& name)
	{
		return ObjectCollection<T>::retrieveObjectByTN(type, name);
	}

	void addObject(T* obj)
	{
		ObjectCollection<T>::addObject(obj);
	}

	void removeObject(T* obj)
	{
		ObjectCollection<T>::removeObject(obj);
	}
};




//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
SimpleObjectManager<T>::SimpleObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
SimpleObjectManager<T>::~SimpleObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
T* SimpleObjectManager<T>::createObject(const String& type, const String& name)
{
	assert(retrieveObjectByName(name) == nullptr);
    return ObjectCollection<T>::createObject(type, name);
}


U2EG_NAMESPACE_END

#endif // __U2SimpleObjectManager_H__
