#ifndef __U2PoolingObjectManager_H__
#define __U2PoolingObjectManager_H__

#include "U2Prerequisites.h"
#include "U2ObjectCollection.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN


class ObjectFactory;


template <class T> 
class PoolingObjectManager : public ObjectCollection<T>
{
public:
    /** Default constructor - should never get called by a client app.
    */
    PoolingObjectManager();

    /** Default destructor.
    */
    virtual ~PoolingObjectManager();

    T* reuseObjectAsName(const String& type, const String& name = BLANK);

    void recycleObject(T* obj);

    void destoryUnusedObject(const String& type);

protected:
	T* createObject(const String& type, const String& name = BLANK)
	{
		return ObjectCollection<T>::createObject(type, name);
	}

	void destoryObject(T* obj)
	{
		ObjectCollection<T>::destoryObject(obj);
	}

	void addObject(T* obj)
	{
		ObjectCollection<T>::addObject(obj);
	}

	void removeObject(T* obj)
	{
		ObjectCollection<T>::removeObject(obj);
	}

protected:
	ObjectCollection<T>                         mUnusedObjects;
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
T* PoolingObjectManager<T>::reuseObjectAsName(const String& type, const String& name)
{
	T* pObj = mUnusedObjects.retrieveObjectByType(type);
	if (pObj == nullptr)
	{
		pObj = mUnusedObjects.createObject(type, name);
	}
	
	mUnusedObjects.removeObject(pObj);
	addObject(pObj);
	pObj->postReuseFromPool();

    return pObj;
}
//-----------------------------------------------------------------------
template<class T>
void PoolingObjectManager<T>::recycleObject(T* obj)
{
	assert(obj != nullptr);
	assert(retrieveObjectByGuid(obj->getGuid()) != nullptr);
	assert(mUnusedObjects.retrieveObjectByGuid(obj->getGuid()) == nullptr);

	obj->preRecycleByPool();
	removeObject(obj);
	mUnusedObjects.addObject(obj);
}
//-----------------------------------------------------------------------
template<class T>
void PoolingObjectManager<T>::destoryUnusedObject(const String& type)
{
	typename ObjectCollection<T>::ObjectMapIterator it = mUnusedObjects.retrieveAllObjectsByType(type);
	while (it.hasMoreElements())
	{
		T* pObj = dynamic_cast<T*>(it.getNext());
		mUnusedObjects.destoryObject(pObj);
	}
}


U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
