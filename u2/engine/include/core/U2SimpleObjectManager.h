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

    T* createObject(const String& type, const String& name);

protected:
	ConstObjectMapIterator retrieveAllObjectsByName(const String& name) const
	{
		return ObjectCollection<T>::retrieveAllObjectsByName(name);
	}

	ObjectMapIterator retrieveAllObjectsByName(const String& name)
	{
		return ObjectCollection<T>::retrieveAllObjectsByName(name);
	}

	ConstObjectMapIterator retrieveAllObjectsByType(const String& type) const
	{
		return ObjectCollection<T>::retrieveAllObjectsByType(type);
	}

	ObjectMapIterator retrieveAllObjectsByType(const String& type)
	{
		return ObjectCollection<T>::retrieveAllObjectsByType(type);
	}

	const T* retrieveObjectByType(const String& type) const
	{
		return ObjectCollection<T>::retrieveObjectByType(type);
	}

	T* retrieveObjectByType(const String& type)
	{
		return ObjectCollection<T>::retrieveObjectByType(type);
	}

	const T* retrieveObjectByGuid(const String& guid) const
	{
		return ObjectCollection<T>::retrieveObjectByGuid(guid);
	}

	T* retrieveObjectByGuid(const String& guid)
	{
		return ObjectCollection<T>::retrieveObjectByGuid(guid);
	}

	ConstObjectMapIterator retrieveAllObjectsByTN(const String& type, const String& name) const
	{
		return ObjectCollection<T>::retrieveAllObjectsByTN(type, name);
	}

	ObjectMapIterator retrieveAllObjectsByTN(const String& type, const String& name)
	{
		return ObjectCollection<T>::retrieveAllObjectsByTN(type, name);
	}

	const T* retrieveObjectByTN(const String& type, const String& name) const
	{
		return ObjectCollection<T>::retrieveObjectByTN(type, name);
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
