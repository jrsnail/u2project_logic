#ifndef __U2System__
#define __U2System__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"
#include "U2Singleton.h"


U2EG_NAMESPACE_BEGIN



class System : public Object
{
public:
    System(const String& type, const String& name);
    virtual ~System();

    void initialize(size_t priority);

    virtual void enter() = 0;
    virtual void execute(u2real dt) = 0;
    virtual void exit() = 0;

protected:
    size_t m_uPriority;
};


class SystemManager : public SimpleObjectManager<System>, public Singleton < SystemManager >
{
public:
    /** Default constructor - should never get called by a client app.
    */
    SystemManager();

    /** Default destructor.
    */
    virtual ~SystemManager();

    void enter();
    void execute(u2real dt);
    void exit();

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
    static SystemManager& getSingleton(void);

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
    static SystemManager* getSingletonPtr(void);

protected:
    typedef map<size_t, System*>::type      PriorityMap;
    PriorityMap     m_ActiveMap;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2System__) */
