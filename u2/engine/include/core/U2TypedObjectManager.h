#ifndef __U2TypedObjectManager_H__
#define __U2TypedObjectManager_H__


#include "U2Prerequisites.h"
#include "U2IteratorWrapper.h"
#include "U2FactoryManager.h"


U2EG_NAMESPACE_BEGIN


class ObjectFactory;


template <class T> 
class TypedObjectManager
{
protected:
    /// <name, T*>
    typedef std::map<String, T*>                ObjectQueue;
    /// <type, ObjectQueue>
    typedef std::map<String, ObjectQueue>       ObjectQueueMap;

public:
    /** Default constructor - should never get called by a client app.
    */
    TypedObjectManager();

    /** Default destructor.
    */
    virtual ~TypedObjectManager();

    T* createObject(const String& type, const String& name);

	const T* retrieveObject(const String& type, const String& name) const;

    T* retrieveObject(const String& type, const String& name);

	bool hasObject(const String& type, const String& name) const;

	void destoryObject(T* obj);

    typedef MapIterator<ObjectQueue> ObjectQueueIterator;

    /** Get an iterator over the Archives in this Manager. */
    ObjectQueueIterator getObjectIterator(const String& type)
    {
        ObjectQueue& queue = mObjects[type];
        return ObjectQueueIterator(queue.begin(), queue.end());
    }

protected:
	/// Currently loaded archives
	ObjectQueueMap                              mObjects;
};



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
TypedObjectManager<T>::TypedObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
TypedObjectManager<T>::~TypedObjectManager()
{
    for (typename ObjectQueueMap::iterator itr = mObjects.begin();
        itr != mObjects.end();
        ++itr)
    {
        ObjectQueue& queue = itr->second;
        for (typename ObjectQueue::iterator itr2 = queue.begin();
            itr2 != queue.end();
            ++itr2)
        {
			FactoryManager::getSingleton().destroyObject(itr2->second);
        }
    }
    mObjects.clear();
}
//-----------------------------------------------------------------------
template <class T>
T* TypedObjectManager<T>::createObject(const String& type, const String& name)
{
    T* pObj = retrieveObject(type, name);
    if (pObj != nullptr)
    {
        return pObj;
    }

	pObj = dynamic_cast<T*>(FactoryManager::getSingleton().createObject(type, name));
    ObjectQueue& queue = mObjects[type];
    queue[name] = pObj;
    return pObj;
}
//-----------------------------------------------------------------------
template <class T>
const T* TypedObjectManager<T>::retrieveObject(const String& type, const String& name) const
{
    return const_cast<const T*>(this->retrieveObject(type, name));
}
//-----------------------------------------------------------------------
template <class T>
T* TypedObjectManager<T>::retrieveObject(const String& type, const String& name)
{
	typename ObjectQueueMap::const_iterator itr = mObjects.find(type);
    if (itr != mObjects.end())
    {
        const ObjectQueue& queue = itr->second;
		typename ObjectQueue::const_iterator itr2 = queue.find(name);
        if (itr2 != queue.end())
        {
            return itr2->second;
        }
    }

    return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
bool TypedObjectManager<T>::hasObject(const String& type, const String& name) const
{
    return nullptr != retrieveObject(type, name);
}
//-----------------------------------------------------------------------
template <class T>
void TypedObjectManager<T>::destoryObject(T* obj)
{
	typename ObjectQueueMap::iterator itr = mObjects.find(obj->getType());
    if (itr != mObjects.end())
    {
        ObjectQueue& queue = itr->second;
		typename ObjectQueue::iterator itr2 = queue.find(obj->getName());
        if (itr2 != queue.end())
        {
			FactoryManager::getSingleton().destroyObject(obj);
            queue.erase(itr2);
        }
    }
}


U2EG_NAMESPACE_END

#endif // __U2TypedObjectManager_H__
