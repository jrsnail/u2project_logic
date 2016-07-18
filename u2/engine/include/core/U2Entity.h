#ifndef __U2Entity__
#define __U2Entity__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"
#include "U2Singleton.h"


U2EG_NAMESPACE_BEGIN


class Component;


class Entity : public Object
{
public:
    // <type, Component*>
    typedef std::multimap<String, Component*>                TypedComponentMap;
    typedef std::pair<TypedComponentMap::iterator, TypedComponentMap::iterator>   Pair;

public:
    Entity(const String& type, const String& name);
    virtual ~Entity();

    void addComponent(Component* comp);

    void removeComponent(Component* comp);

    typedef MapIterator<TypedComponentMap>          ComponentMapIterator;
    typedef ConstMapIterator<TypedComponentMap>     ConstComponentMapIterator;
    ComponentMapIterator retrieveComponentsByType(const String& type)
    {
        Pair p = m_TypedMap.equal_range(type);
        return ComponentMapIterator(p.first, p.second);
    }

    Component* retrieveComponentByGuid(const String& guid);

protected:
    TypedComponentMap          m_TypedMap;
};


class EntityManager : public SimpleObjectManager<Entity>, public Singleton < EntityManager >
{
public:
    /** Default constructor - should never get called by a client app.
    */
    EntityManager();

    /** Default destructor.
    */
    virtual ~EntityManager();

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static EntityManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static EntityManager* getSingletonPtr(void);
};



U2EG_NAMESPACE_END

#endif /* defined(__U2Entity__) */
