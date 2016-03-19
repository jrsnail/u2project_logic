#ifndef __U2ObjectCollection_H__
#define __U2ObjectCollection_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2IteratorWrapper.h"
#include "U2FactoryManager.h"
#include "U2ValueList.h"


U2EG_NAMESPACE_BEGIN


class ObjectFactory;


template <class T> 
class ObjectCollection
{
protected:
	// <name, type, guid>
	// guid: sole
	// type: multi
	// name: multi
	typedef std::map<TYPE_KVLIST_3(String, String, String), T*>		MultiKeyMap;

public:
    /** Default constructor - should never get called by a client app.
    */
    ObjectCollection();

    /** Default destructor.
    */
    virtual ~ObjectCollection();

    typedef MapIterator<MultiKeyMap>		ObjectQueueIterator;
	typedef ConstMapIterator<MultiKeyMap>	ConstObjectQueueIterator;

	T* createObject(const String& type, const String& name);

	void destoryObject(T* obj);

	ConstObjectQueueIterator retrieveAllObjectsByName(const String& name) const
	{
		MultiKeyMap::const_iterator bit = mObjects.lower_bound(VLIST_1(name));
		MultiKeyMap::const_iterator eit = mObjects.upper_bound(VLIST_1(name));
		return ConstObjectQueueIterator(bit, eit);
	}

	ObjectQueueIterator retrieveAllObjectsByName(const String& name)
	{
		MultiKeyMap::iterator bit = mObjects.lower_bound(VLIST_1(name));
		MultiKeyMap::iterator eit = mObjects.upper_bound(VLIST_1(name));
		return ObjectQueueIterator(bit, eit);
	}

	const T* retrieveObjectByName(const String& name) const;

	T* retrieveObjectByName(const String& name);

	ConstObjectQueueIterator retrieveAllObjectsByType(const String& type) const
	{
		MultiKeyMap::const_iterator bit = mObjects.lower_bound(VLIST_2(KeyHolder<String>(), type));
		MultiKeyMap::const_iterator eit = mObjects.upper_bound(VLIST_2(KeyHolder<String>(), type));
		return ConstObjectQueueIterator(bit, eit);
	}

	ObjectQueueIterator retrieveAllObjectsByType(const String& type)
	{
		MultiKeyMap::iterator bit = mObjects.lower_bound(VLIST_2(KeyHolder<String>(), type));
		MultiKeyMap::iterator eit = mObjects.upper_bound(VLIST_2(KeyHolder<String>(), type));
		return ObjectQueueIterator(bit, eit);
	}

	const T* retrieveObjectByType(const String& type) const;

	T* retrieveObjectByType(const String& type);

	const T* retrieveObjectByGuid(const String& guid) const;

	T* retrieveObjectByGuid(const String& guid);

	ConstObjectQueueIterator retrieveAllObjectsByTN(const String& type, const String& name) const
	{
		MultiKeyMap::const_iterator bit = mObjects.lower_bound(VLIST_2(name, type));
		MultiKeyMap::const_iterator eit = mObjects.upper_bound(VLIST_2(name, type));
		return ConstObjectQueueIterator(bit, eit);
	}

	ObjectQueueIterator retrieveAllObjectsByTN(const String& type, const String& name)
	{
		MultiKeyMap::iterator bit = mObjects.lower_bound(VLIST_2(name, type));
		MultiKeyMap::iterator eit = mObjects.upper_bound(VLIST_2(name, type));
		return ObjectQueueIterator(bit, eit);
	}

	const T* retrieveObjectByTN(const String& type, const String& name) const;

	T* retrieveObjectByTN(const String& type, const String& name);


protected:
	MultiKeyMap					mObjects;
};



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
ObjectCollection<T>::ObjectCollection()
{
}
//-----------------------------------------------------------------------
template <class T>
ObjectCollection<T>::~ObjectCollection()
{
    for (typename MultiKeyMap::iterator it = mObjects.begin();
		it != mObjects.end();
        ++it)
    {
		FactoryManager::getSingleton().destroyObject(it->second);
    }
    mObjects.clear();
}
//-----------------------------------------------------------------------
template <class T>
T* ObjectCollection<T>::createObject(const String& type, const String& name)
{
	T* pObj = dynamic_cast<T*>(FactoryManager::getSingleton().createObject(type, name));
	mObjects[VLIST_3(pObj->getName(), pObj->getType(), pObj->getGuid())] = pObj;
	return pObj;
}
//-----------------------------------------------------------------------
template <class T>
void ObjectCollection<T>::destoryObject(T* obj)
{
	assert(obj);
	typename ObjectQueueMap::iterator it = mObjects.find(VLIST_3(obj->getName(), obj->getType(), obj->getGuid()));
	if (it != mObjects.end())
	{
		FactoryManager::getSingleton().destroyObject(obj);
		mObjects.erase(it);
	}
}
//-----------------------------------------------------------------------
template <class T>
const T* ObjectCollection<T>::retrieveObjectByName(const String& name) const
{
    return const_cast<const T*>(this->retrieveObjectByName(name));
}
//-----------------------------------------------------------------------
template <class T>
T* ObjectCollection<T>::retrieveObjectByName(const String& name)
{
	typename MultiKeyMap::iterator it = mObjects.find(VLIST_1(name));
    if (it != mObjects.end())
    {
		return it->second;
    }

    return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
const T* ObjectCollection<T>::retrieveObjectByTN(const String& type, const String& name) const
{
	return const_cast<const T*>(this->retrieveObjectByTN(type, name));
}
//-----------------------------------------------------------------------
template <class T>
T* ObjectCollection<T>::retrieveObjectByTN(const String& type, const String& name)
{
	typename MultiKeyMap::iterator it = mObjects.find(VLIST_2(name, type));
	if (it != mObjects.end())
	{
		return it->second;
	}

	return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
const T* ObjectCollection<T>::retrieveObjectByType(const String& type) const
{
	return const_cast<const T*>(this->retrieveObjectByType(type));
}
//-----------------------------------------------------------------------
template <class T>
T* ObjectCollection<T>::retrieveObjectByType(const String& type)
{
	typename MultiKeyMap::iterator it = mObjects.find(VLIST_2(KeyHolder<String>(), type));
	if (it != mObjects.end())
	{
		return it->second;
	}

	return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
const T* ObjectCollection<T>::retrieveObjectByGuid(const String& guid) const
{
	return const_cast<const T*>(this->retrieveObjectByGuid(guid));
}
//-----------------------------------------------------------------------
template <class T>
T* ObjectCollection<T>::retrieveObjectByGuid(const String& guid)
{
	typename MultiKeyMap::iterator it = mObjects.find(VLIST_3(KeyHolder<String>(), KeyHolder<String>(), guid));
	if (it != mObjects.end())
	{
		return it->second;
	}

	return nullptr;
}


U2EG_NAMESPACE_END

#endif // __U2ObjectCollection_H__
