#ifndef __U2Component__
#define __U2Component__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Prototype.h"
#include "U2ResourceManager.h"
#include "U2TypedObjectManager.h"
#include "U2Singleton.h"


class TiXmlElement;


U2EG_NAMESPACE_BEGIN


class GameObject;


class Component : public Resource, public Prototype<Component>
{
public:
    Component(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, bool isManual = false
        , ManualResourceLoader* loader = 0);
    Component(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~Component();

    virtual void copy(const Component& src);

    virtual Component* cloneFromPrototype(const String& name = BLANK, const String& guid = BLANK) override;
    virtual Component* cloneFromInstance(const String& name = BLANK, const String& guid = BLANK) override;
    virtual void resetFromPrototype() override;
    virtual void applyToPrototype() override;

    void bornOn(GameObject* gameObj);
    GameObject* getBornGameObject() const;

    void attachedOn(GameObject* gameObj);
    GameObject* getAttachedGameObject() const;

    void setState(size_t state);
    size_t getState() const;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) { return false; };

protected:
    /// @copydoc Resource::loadImpl
    virtual void loadImpl(void) override;

    /// @copydoc Resource::unloadImpl
    virtual void unloadImpl(void) override;

public:
    static const size_t CS_None = 0;
    static const size_t CS_Attaching = 1;
    static const size_t CS_Active = 2;
    static const size_t CS_Deactive = 3;
    static const size_t CS_Detaching = 4;

protected:
    GameObject*     m_pBornGameObj;
    GameObject*     m_pAttachedGameObj;
    size_t          m_uState;
};


typedef std::shared_ptr<Component>	ComponentPtr;


class ComponentManager : public ResourceManager, public Singleton < ComponentManager >
{
    friend Component;

public:
    /** Default constructor - should never get called by a client app.
    */
    ComponentManager();

    /** Default destructor.
    */
    virtual ~ComponentManager();

    /// @copydoc ScriptLoader::parseScript
    virtual void parseScript(InStreamPtr& stream, const String& groupName) override;

    virtual Resource* createImpl(const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader,
        const NameValuePairList* createParams) override;

    /// @see ResourceManager::createResource
    ComponentPtr create(const String& name, const String& group,
        bool isManual = false, ManualResourceLoader* loader = 0,
        const NameValuePairList* createParams = 0);

    Component* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

    void destoryObject(Component* obj);

    Component* retrieveObjectByTN(const String& type, const String& name);

    TypedObjectManager<Component>::ObjectMapIterator retrieveAllObjectsByType(const String& type);

    Component* retrieveObjectByGuid(const String& guid);

    Component* retrieveObjectByType(const String& type);

protected:
    Component* _createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

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
    static ComponentManager& getSingleton(void);

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
    static ComponentManager* getSingletonPtr(void);

protected:
    TypedObjectManager<Component> m_InstanceCollection;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2Component__) */
