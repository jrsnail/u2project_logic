#ifndef __U2SimpleObjectManager_H__
#define __U2SimpleObjectManager_H__

#include "U2Prerequisites.h"
#include "U2IteratorWrapper.h"
#include "U2FactoryManager.h"


U2EG_NAMESPACE_BEGIN



template <class T> 
class SimpleObjectManager
{
protected:
    /// <name, T*>
    typedef std::map<String, T*>                ObjectMap;

public:
    /** Default constructor - should never get called by a client app.
    */
    SimpleObjectManager();

    /** Default destructor.
    */
    virtual ~SimpleObjectManager();

    T* createObject(const String& type, const String& name);

	const T* retrieveObject(const String& name) const;

    T* retrieveObject(const String& name);

	bool hasObject(const String& name) const;

	void destoryObject(T* obj);

    typedef MapIterator<ObjectMap>          ObjectMapIterator;
    typedef ConstMapIterator<ObjectMap>     ConstObjectMapIterator;

    /** Get an iterator over the Archives in this Manager. */
    ObjectMapIterator getObjectIterator()
    {
        return ObjectMapIterator(mObjects.begin(), mObjects.end());
    }

    ConstObjectMapIterator getConstObjectIterator()
    {
        return ConstObjectMapIterator(mObjects.begin(), mObjects.end());
    }

protected:
	/// Currently loaded archives
    ObjectMap                                   mObjects;
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
    // Unload & delete resources in turn
    for (typename ObjectMap::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
    {
		FactoryManager::getSingleton().destroyObject(it->second);
    }
    // Empty the list
    mObjects.clear();
}
//-----------------------------------------------------------------------
template <class T>
T* SimpleObjectManager<T>::createObject(const String& type, const String& name)
{
	typename ObjectMap::iterator i = mObjects.find(name);
    T* pObj = nullptr;

    if (i == mObjects.end())
    {
		pObj = dynamic_cast<T*>(FactoryManager::getSingleton().createObject(type, name));
        mObjects[name] = pObj;
    }
    else
    {
        pObj = i->second;
    }
    return pObj;
}
//-----------------------------------------------------------------------
template <class T>
const T* SimpleObjectManager<T>::retrieveObject(const String& name) const
{
    return const_cast<const T*>(this->retrieveObject(name));
}
//-----------------------------------------------------------------------
template <class T>
T* SimpleObjectManager<T>::retrieveObject(const String& name)
{
	typename ObjectMap::iterator itr = mObjects.find(name);
    if (itr != mObjects.end())
    {
        return itr->second;
    }

    return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
bool SimpleObjectManager<T>::hasObject(const String& name) const
{
    return nullptr != retrieveObject(name);
}
//-----------------------------------------------------------------------
template <class T>
void SimpleObjectManager<T>::destoryObject(T* obj)
{
	typename ObjectMap::iterator it = mObjects.find(obj->getName());
    if (it != mObjects.end())
    {
		FactoryManager::getSingleton().destroyObject(it->second);
        mObjects.erase(it);
    }
}


U2EG_NAMESPACE_END

#endif // __U2SimpleObjectManager_H__
