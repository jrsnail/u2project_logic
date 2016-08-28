#ifndef __U2TypedObjectManager_H__
#define __U2TypedObjectManager_H__


#include "U2Prerequisites.h"
#include "U2ObjectCollection.h"


U2EG_NAMESPACE_BEGIN


template <class T> 
class TypedObjectManager
{
protected:
    // <type, T*>
    typedef std::multimap<String, T*>     TypedObjectMap;
    typedef std::pair<typename TypedObjectMap::iterator, typename TypedObjectMap::iterator>   Pair;
    
public:
    /** Default constructor - should never get called by a client app.
     */
    TypedObjectManager();
    
    /** Default destructor.
     */
    virtual ~TypedObjectManager();
    
    T* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);
    
    void destoryObject(T* obj);
    
    T* retrieveObjectByTN(const String& type, const String& name);
    
    typedef MapIterator<TypedObjectMap>          ObjectMapIterator;
    typedef ConstMapIterator<TypedObjectMap>     ConstObjectMapIterator;
    ObjectMapIterator retrieveAllObjectsByType(const String& type)
    {
        Pair p = m_TypedMap.equal_range(type);
        return ObjectMapIterator(p.first, p.second);
    }
    
    T* retrieveObjectByGuid(const String& guid);
    
    T* retrieveObjectByType(const String& type);
    
    void addObject(T* obj);
    
    void removeObject(T* obj);
    
protected:
    TypedObjectMap          m_TypedMap;
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
}
//-----------------------------------------------------------------------
template <class T>
T* TypedObjectManager<T>::createObject(const String& type, const String& name, const String& guid)
{
    T* pObj = dynamic_cast<T*>(ObjectCollection::getSingletonPtr()->createObject(type, name, guid));
    if (pObj != nullptr)
    {
        m_TypedMap.insert(std::make_pair(type, pObj));
    }
    
    return pObj;
}
//-----------------------------------------------------------------------
template <class T>
void TypedObjectManager<T>::destoryObject(T* obj)
{
    assert(obj);
    // manager can only destroy objects which managed by itself
    ObjectMapIterator it = retrieveAllObjectsByType(obj->getType());
    while (it.hasMoreElements())
    {
        typename TypedObjectMap::iterator curIt = it.current();
        T* pObj = it.getNext();
        if (pObj == obj)
        {
            ObjectCollection::getSingletonPtr()->destoryObject(obj);
            m_TypedMap.erase(curIt);
            break;
        }
    }
}
//-----------------------------------------------------------------------
template <class T>
T* TypedObjectManager<T>::retrieveObjectByTN(const String& type, const String& name)
{
    ObjectMapIterator it = retrieveAllObjectsByType(type);
    while (it.hasMoreElements())
    {
        return it.getNext();
    }
    return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
T* TypedObjectManager<T>::retrieveObjectByGuid(const String& guid)
{
    for (typename TypedObjectMap::iterator it = m_TypedMap.begin(); it != m_TypedMap.end(); it++)
    {
        T* pObj = it->second;
        if (pObj->getGuid() == guid)
        {
            return pObj;
        }
    }
    return nullptr;
}
//-----------------------------------------------------------------------
template <class T>
T* TypedObjectManager<T>::retrieveObjectByType(const String& type)
{
    typename TypedObjectMap::iterator it = m_TypedMap.find(type);
    if (it == m_TypedMap.end())
    {
        return nullptr;
    }
    else
    {
        return it->second;
    }
}
//-----------------------------------------------------------------------
template <class T>
void TypedObjectManager<T>::addObject(T* obj)
{
    assert(obj);
    m_TypedMap.insert(std::make_pair(obj->getType(), obj));
}
//-----------------------------------------------------------------------
template <class T>
void TypedObjectManager<T>::removeObject(T* obj)
{
    assert(obj);
    Pair p = m_TypedMap.equal_range(obj->getType());
    for (typename TypedObjectMap::iterator it = p.first; it != p.second; it++)
    {
        if (obj == it->second)
        {
            m_TypedMap.erase(it);
            return;
        }
    }
}



U2EG_NAMESPACE_END

#endif // __U2TypedObjectManager_H__
