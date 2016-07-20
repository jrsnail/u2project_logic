#ifndef __U2GameObject__
#define __U2GameObject__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2Prototype.h"
#include "U2ResourceManager.h"
#include "U2TypedObjectManager.h"
#include "U2Singleton.h"


U2EG_NAMESPACE_BEGIN

class Component;


class GameObject : public Resource, public Prototype<GameObject>
{
public:
    // <type, u2::Component*>
    typedef std::multimap<String, u2::Component*>               TypedComponentMap;
    typedef std::pair<TypedComponentMap::iterator, TypedComponentMap::iterator>   ComponentPair;

    // <type, GameObject*>
    typedef std::multimap<String, GameObject*>              TypedGameObjectMap;
    typedef std::pair<TypedGameObjectMap::iterator, TypedGameObjectMap::iterator> GameObjectPair;

public:
    GameObject(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~GameObject();

    void copy(const GameObject& src);

    virtual GameObject* cloneFromPrototype(const String& name = BLANK) override;
    virtual GameObject* cloneFromInstance(const String& name = BLANK) override;
    virtual void resetFromPrototype() override;
    virtual void applyToPrototype() override;

    u2::Component* createComponent(const String& type, const String& name);

    void destroyComponent(u2::Component* comp);

    void addComponent(u2::Component* comp);

    void removeComponent(u2::Component* comp);

    typedef MapIterator<TypedComponentMap>          ComponentMapIterator;
    typedef ConstMapIterator<TypedComponentMap>     ConstComponentMapIterator;
    ComponentMapIterator retrieveComponentsByType(const String& type)
    {
        ComponentPair p = m_ComponentMap.equal_range(type);
        return ComponentMapIterator(p.first, p.second);
    }

    u2::Component* retrieveComponentByGuid(const String& guid);

    GameObject* createGameObject(const String& type, const String& name);

    void destroyGameObject(GameObject* gameObj);

    void addGameObject(GameObject* gameObj);

    void removeGameObject(GameObject* gameObj);

    typedef MapIterator<TypedGameObjectMap>          GameObjectMapIterator;
    typedef ConstMapIterator<TypedGameObjectMap>     ConstGameObjectMapIterator;
    GameObjectMapIterator retrieveGameObjects()
    {
        return GameObjectMapIterator(m_GameObjMap.begin(), m_GameObjMap.end());
    }

    GameObject* retrieveGameObjectByGuid(const String& guid);

protected:
    /// @copydoc Resource::loadImpl
    virtual void loadImpl(void) override;

    /// @copydoc Resource::unloadImpl
    virtual void unloadImpl(void) override;

protected:
    TypedComponentMap           m_ComponentMap;
    TypedGameObjectMap          m_GameObjMap;
};


class GameObjectManager : public ResourceManager, public Singleton < GameObjectManager >
{
public:
    GameObjectManager();
    virtual ~GameObjectManager();

    /// @copydoc ScriptLoader::parseScript
    virtual void parseScript(InStreamPtr& stream, const String& groupName) override;

    virtual Resource* createImpl(const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader,
        const NameValuePairList* createParams) override;

    GameObject* createObject(const String& type, const String& name = BLANK);

    void destoryObject(GameObject* obj);

    GameObject* retrieveObjectByTN(const String& type, const String& name);

    TypedObjectManager<GameObject>::ObjectMapIterator retrieveAllObjectsByType(const String& type);

    GameObject* retrieveObjectByGuid(const String& guid);

    GameObject* retrieveObjectByType(const String& type);


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
    static GameObjectManager& getSingleton(void);

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
    static GameObjectManager* getSingletonPtr(void);

protected:
    TypedObjectManager<GameObject> m_InstanceCollection;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2Entity__) */
