﻿#ifndef __U2PoolingObjectManager_H__
#define __U2PoolingObjectManager_H__

#include "U2Prerequisites.h"
#include "U2TypedObjectManager.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN


// todo: check T is ReusableObject type in compile time
template <class T> 
class PoolingObjectManager : public TypedObjectManager<T>
{
public:
    /** Default constructor - should never get called by a client app.
    */
    PoolingObjectManager();

    /** Default destructor.
    */
    virtual ~PoolingObjectManager();

    T* reuseObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

    void recycleObject(T* obj);

    void destoryUnusedObject(const String& type);

protected:
	T* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK)
	{
		return TypedObjectManager<T>::createObject(type, name, guid);
	}

	void destoryObject(T* obj)
	{
		TypedObjectManager<T>::destoryObject(obj);
	}
    
    void addObject(T* obj)
    {
        TypedObjectManager<T>::addObject(obj);
    }
    
    void removeObject(T* obj)
    {
        TypedObjectManager<T>::removeObject(obj);
    }


protected:
	TypedObjectManager<T>                         mUnusedObjects;
};



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
PoolingObjectManager<T>::PoolingObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
PoolingObjectManager<T>::~PoolingObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
T* PoolingObjectManager<T>::reuseObject(const String& type, const String& name, const String& guid)
{
	T* pObj = mUnusedObjects.retrieveObjectByType(type);
	if (pObj == nullptr)
	{
		pObj = mUnusedObjects.createObject(type, name, guid);
		mUnusedObjects.removeObject(pObj);
	}
	else
	{
		mUnusedObjects.removeObject(pObj);
		pObj->renameAsName(name);
        pObj->renameAsGuid(guid);
	}
	
	addObject(pObj);
	pObj->postReuseFromPool();

    return pObj;
}
//-----------------------------------------------------------------------
template<class T>
void PoolingObjectManager<T>::recycleObject(T* obj)
{
	assert(obj != nullptr);
	assert(this->retrieveObjectByGuid(obj->getGuid()) != nullptr);
	assert(mUnusedObjects.retrieveObjectByGuid(obj->getGuid()) == nullptr);

	obj->preRecycleByPool();
	removeObject(obj);
	mUnusedObjects.addObject(obj);
}
//-----------------------------------------------------------------------
template<class T>
void PoolingObjectManager<T>::destoryUnusedObject(const String& type)
{
	typename PoolingObjectManager<T>::ObjectMapIterator it = mUnusedObjects.retrieveAllObjectsByType(type);
	while (it.hasMoreElements())
	{
		mUnusedObjects.destoryObject(it.getNext());
	}
}


U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
