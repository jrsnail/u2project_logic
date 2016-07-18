#ifndef __U2SimpleObjectManager_H__
#define __U2SimpleObjectManager_H__

#include "U2Prerequisites.h"
#include "U2ObjectCollection.h"


U2EG_NAMESPACE_BEGIN



template <class T> 
class SimpleObjectManager
{
protected:
    typedef std::map<String, T*>        NamedObjectMap;
    
public:
    /** Default constructor - should never get called by a client app.
    */
    SimpleObjectManager();

    /** Default destructor.
    */
    virtual ~SimpleObjectManager();

    T* createObject(const String& type, const String& name = BLANK);
    
    void destoryObject(T* obj);
    
    T* retrieveObjectByName(const String& name);
    
    T* retrieveObjectByGuid(const String& guid);
    
    bool hasObjectByName(const String& name);
    
    typedef MapIterator<NamedObjectMap>          ObjectMapIterator;
    typedef ConstMapIterator<NamedObjectMap>     ConstObjectMapIterator;
    
    ObjectMapIterator retrieveAllObjects()
    {
        typename NamedObjectMap::iterator bit = m_NamedMap.begin();
        typename NamedObjectMap::iterator eit = m_NamedMap.end();
        return ObjectMapIterator(bit, eit);
    }
	

protected:
    NamedObjectMap          m_NamedMap;
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
    // As simple object manager, we index object with name which should be an unique key
    assert(SimpleObjectManager<T>::retrieveObjectByName(name) == nullptr);
    
    T* pObj = dynamic_cast<T*>(ObjectCollection::getSingletonPtr()->createObject(type, name));
    if (pObj != nullptr)
    {
        m_NamedMap[pObj->getName()] = pObj;
    }
    return pObj;
}
//-----------------------------------------------------------------------
template <class T>
void SimpleObjectManager<T>::destoryObject(T* obj)
{
    assert(obj);
    // manager can only destroy objects which managed by itself
    assert(SimpleObjectManager<T>::retrieveObjectByName(obj->getName()) != nullptr);
    
    // destory in SimpleObjectManager
    typename NamedObjectMap::iterator it = m_NamedMap.find(obj->getName());
    if (it != m_NamedMap.end())
    {
        m_NamedMap.erase(it);
    }

    // destory in ObjectCollection
    ObjectCollection::getSingletonPtr()->destoryObject(obj);
}
//-----------------------------------------------------------------------
template <class T>
T* SimpleObjectManager<T>::retrieveObjectByName(const String& name)
{
    typename NamedObjectMap::iterator it = m_NamedMap.find(name);
    if (it != m_NamedMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
//-----------------------------------------------------------------------
template <class T>
bool SimpleObjectManager<T>::hasObjectByName(const String& name)
{
    return retrieveObjectByName(name) != nullptr;
}
//-----------------------------------------------------------------------
template <class T>
T* SimpleObjectManager<T>::retrieveObjectByGuid(const String& guid)
{
    for (typename NamedObjectMap::iterator it = m_NamedMap.begin(); it != m_NamedMap.end(); it++)
    {
        T* pObj = it->second;
        if (pObj->getGuid() == guid)
        {
            return pObj;
        }
    }
    return nullptr;
}


U2EG_NAMESPACE_END

#endif // __U2SimpleObjectManager_H__
