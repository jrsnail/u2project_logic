#ifndef __U2PoolingObjectManager_H__
#define __U2PoolingObjectManager_H__

#include "U2Prerequisites.h"
#include "U2IteratorWrapper.h"
#include "U2FactoryManager.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN


class ObjectFactory;


template <class T> 
class PoolingObjectManager
{
protected:
    /// <guid, T*>
    typedef std::map<String, T*>                ObjectQueue;
    /// <type, ObjectQueue>
    typedef std::map<String, ObjectQueue>       ObjectQueueMap;

public:
    /** Default constructor - should never get called by a client app.
    */
    PoolingObjectManager();

    /** Default destructor.
    */
    virtual ~PoolingObjectManager();

    T* createOrReuseObject(const String& type);

    const T* retrieveObject(const String& guid) const;

    T* retrieveObject(const String& guid);

    bool hasObject(const String& guid) const;

    void recycleObject(T* obj);

    void destoryUnusedObject(const String& type);

//     typedef MapIterator<ObjectQueue> ObjectQueueIterator;
// 
//     /** Get an iterator over the Archives in this Manager. */
// 	ObjectQueueIterator getObjectIterator(const String& type)
//     {
// 		ObjectQueue& queue = mUsingObjects[type];
//         return ObjectQueueIterator(queue.begin(), queue.end());
//     }

protected:
    T* _createObject(const String& type);
    T* _reuseObject(const String& type);

protected:
	/// Currently loaded archives
    ObjectQueueMap                              mUsingObjects;
    ObjectQueueMap                              mUnusedObjects;
    ObjectQueue                                 mAllObjects;
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
    for (typename ObjectQueue::iterator itr = mAllObjects.begin();
        itr != mAllObjects.end();
        ++itr)
    {
		FactoryManager::getSingleton().destroyObject(itr->second);
    }
    mAllObjects.clear();
    // Empty the list
    mUsingObjects.clear();
    // Empty the list
    mUnusedObjects.clear();
}
//-----------------------------------------------------------------------
template <class T>
T* PoolingObjectManager<T>::createOrReuseObject(const String& type)
{
    do
    {
        // has unused object?
		typename ObjectQueueMap::iterator itr = mUnusedObjects.find(type);
        if (itr != mUnusedObjects.end())
        {
            ObjectQueue& unusedQueue = itr->second;
            if (unusedQueue.size() > 0)
            {
                break;
            }
        }

        // create
        _createObject(type);
    } while (false);

    return _reuseObject(type);
}
//-----------------------------------------------------------------------
template <class T>
const T* PoolingObjectManager<T>::retrieveObject(const String& guid) const
{
	typename ObjectQueue::const_iterator itr = mAllObjects.find(guid);
    if (itr != mAllObjects.end())
    {
        Object* pObj = itr->second;
		typename ObjectQueueMap::const_iterator itr2 = mUsingObjects.find(pObj->getType());
        if (itr2 != mUsingObjects.end())
        {
            const ObjectQueue& queue = itr2->second;
			typename ObjectQueue::const_iterator itr3 = queue.find(guid);
            if (itr3 != queue.end())
            {
                return itr3->second;
            }
        }
    }

    return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
T* PoolingObjectManager<T>::retrieveObject(const String& guid)
{
    return const_cast<T*>(this->retrieveObject(guid));
}
//-----------------------------------------------------------------------
template <class T>
bool PoolingObjectManager<T>::hasObject(const String& guid) const
{
    return nullptr != retrieveObject(guid);
}
//-----------------------------------------------------------------------
template<class T>
void PoolingObjectManager<T>::recycleObject(T* obj)
{
	typename ObjectQueueMap::iterator itr = mUsingObjects.find(obj->getType());
    if (itr != mUsingObjects.end())
    {
        ObjectQueue& usingQueue = mUsingObjects[obj->getType()];
		typename ObjectQueue::iterator itr2 = usingQueue.find(obj->getGuid());
        if (itr2 != usingQueue.end())
        {
            obj->preRecycleByPool();

            // remove from using queue
            usingQueue.erase(itr2);

            // add to unused queue
            ObjectQueue& unusedQueue = mUnusedObjects[obj->getType()];
            unusedQueue[obj->getGuid()] = obj;

            return;
        }
    }
}
//-----------------------------------------------------------------------
template<class T>
T* PoolingObjectManager<T>::_createObject(const String& type)
{
	T* pObj = dynamic_cast<T*>(FactoryManager::getSingleton().createObject(type, BLANK));

    // add to unused queue
    ObjectQueue& queue = mUnusedObjects[pObj->getType()];
    queue[pObj->getGuid()] = pObj;

    // add to all queue
    mAllObjects[pObj->getGuid()] = pObj;

    return pObj;
}
//-----------------------------------------------------------------------
template<class T>
T* PoolingObjectManager<T>::_reuseObject(const String& type)
{
    T* pObj = nullptr;

    // Reuse
	typename ObjectQueueMap::iterator itr = mUnusedObjects.find(type);
    if (itr != mUnusedObjects.end())
    {
        ObjectQueue& unusedQueue = itr->second;
        if (unusedQueue.size() > 0)
        {
            // remove from unused pool
			typename ObjectQueue::iterator itr2 = unusedQueue.begin();
            pObj = itr2->second;
            unusedQueue.erase(itr2);

            // add to using pool
            ObjectQueue& usingQueue = mUsingObjects[pObj->getType()];
            usingQueue[pObj->getGuid()] = pObj;

            pObj->postReuseFromPool();
        }
    }

    return pObj;
}
//-----------------------------------------------------------------------
template<class T>
void PoolingObjectManager<T>::destoryUnusedObject(const String& type)
{
	typename ObjectQueueMap::iterator itr = mUnusedObjects.find(type);
    if (itr != mUnusedObjects.end())
    {
        ObjectQueue& queue = itr->second;
        for (typename ObjectQueue::iterator itr2 = queue.begin();
            itr2 != queue.end();
            ++itr2)
        {
            T* pObj = itr2->second;

            // erase from unused queue
            queue.erase(itr2);

            // erase from all queue
			typename ObjectQueue::iterator itr3 = mAllObjects.find(pObj->getGuid());
            if (itr3 != mAllObjects.end())
            {
                mAllObjects.erase(itr3);
            }

            // delete object
			FactoryManager::getSingleton().destroyObject(pObj);
        }
    }
}


U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
