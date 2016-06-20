#ifndef __U2TNedObjectManager_H__
#define __U2TNedObjectManager_H__


#include "U2Prerequisites.h"
#include "U2SimpleObjectManager.h"
#include "U2ObjectCollection.h"


U2EG_NAMESPACE_BEGIN


template <class T> 
class TNedObjectManager
{
protected:
    // <name, T*>
    typedef std::map<String, T*>                NamedObjectMap;
    // <type, NamedObjectMap>
    typedef std::map<String, NamedObjectMap>    TypedObjectMap;
    
public:
    /** Default constructor - should never get called by a client app.
     */
    TNedObjectManager();
    
    /** Default destructor.
     */
    virtual ~TNedObjectManager();
    
    T* createObject(const String& type, const String& name = BLANK);
    
    void destoryObject(T* obj);
    
    T* retrieveObjectByTN(const String& type, const String& name);
    
    typedef MapIterator<NamedObjectMap>          ObjectMapIterator;
    typedef ConstMapIterator<NamedObjectMap>     ConstObjectMapIterator;
    ObjectMapIterator retrieveAllObjectsByType(const String& type)
    {
        typename TypedObjectMap::iterator tIt = m_TypedMap.find(type);
        if (tIt == m_TypedMap.end())
        {
            assert(0);
        }
        else
        {
            NamedObjectMap& namedMap = m_TypedMap[type];
            typename NamedObjectMap::iterator bit = namedMap.begin();
            typename NamedObjectMap::iterator eit = namedMap.end();
            return ObjectMapIterator(bit, eit);
        }
    }
    
protected:
    TypedObjectMap          m_TypedMap;
};


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
TNedObjectManager<T>::TNedObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
TNedObjectManager<T>::~TNedObjectManager()
{
}
//-----------------------------------------------------------------------
template <class T>
T* TNedObjectManager<T>::createObject(const String& type, const String& name)
{
    T* pObj = nullptr;
    
    NamedObjectMap& namedMap = m_TypedMap[type];
    typename NamedObjectMap::iterator it = namedMap.find(name);
    // inner named object map index object by unique name key
    if (it == namedMap.end())
    {
        pObj = dynamic_cast<T*>(ObjectCollection::getSingleton().createObject(type, name));
        if (pObj != nullptr)
        {
            namedMap[name] = pObj;
        }
    }
    else
    {
        assert(0);
    }
    
    return pObj;
}
//-----------------------------------------------------------------------
template <class T>
void TNedObjectManager<T>::destoryObject(T* obj)
{
    assert(obj);
    // manager can only destroy objects which managed by itself
    assert(TNedObjectManager<T>::retrieveObjectByTN(obj->getType(), obj->getName()) == nullptr);
    
    ObjectCollection::getSingleton().destoryObject(obj);
}
//-----------------------------------------------------------------------
template <class T>
T* TNedObjectManager<T>::retrieveObjectByTN(const String& type, const String& name)
{
    typename TypedObjectMap::iterator tIt = m_TypedMap.find(type);
    if (tIt == m_TypedMap.end())
    {
        return nullptr;
    }
    
    NamedObjectMap& namedMap = m_TypedMap[type];
    typename NamedObjectMap::iterator nIt = namedMap.find(name);
    if (nIt == namedMap.end())
    {
        return nullptr;
    }
    else
    {
        return nIt->second;
    }
}



U2EG_NAMESPACE_END

#endif // __U2TypedObjectManager_H__
