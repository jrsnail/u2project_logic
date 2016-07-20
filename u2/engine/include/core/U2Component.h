#ifndef __U2Component__
#define __U2Component__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"
#include "U2Singleton.h"


U2EG_NAMESPACE_BEGIN


class GameObject;


class Component : public Object
{
public:
    Component(const String& type, const String& name);
    virtual ~Component();

    inline void bornOn(GameObject* gameObj);
    inline GameObject* getBornGameObject() const;

    inline void attachedOn(GameObject* gameObj);
    inline GameObject* getAttachedGameObject() const;

    inline void setState(size_t state);
    inline size_t getState() const;

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


class ComponentManager : public SimpleObjectManager<u2::Component>, public Singleton < ComponentManager >
{
public:
    /** Default constructor - should never get called by a client app.
    */
    ComponentManager();

    /** Default destructor.
    */
    virtual ~ComponentManager();

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
};



U2EG_NAMESPACE_END

#endif /* defined(__U2Component__) */
