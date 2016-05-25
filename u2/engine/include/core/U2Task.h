#ifndef __U2Task_H__
#define __U2Task_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class _U2Export Task : public Object, public GeneralAllocatedObject
{
public:
    /** Constructor - don't call direct, used by ArchiveFactory.
    */
	Task(const String& type, const String& name)
        : Object(type, name)
	{}

    /** Default destructor.
    */
    virtual ~Task() {}

	// Tasks are automatically deleted after Run is called.  
	virtual void run() = 0;
};


typedef Task PendingTask;


template<class Function>
class WrappedFunction : public Task
{
protected:
    typedef typename std::remove_reference<Function>::type function_type;
    Function f;

public:
    WrappedFunction(const String& name)
        : Task("WrappedFunction", name)
    { }

    void initialize(function_type&& f)
    {
        f = std::move(f);
    }

    virtual void operator()() override
    {
        f();
        //delete this;
    }
};


class _U2Export TaskManager : public SimpleObjectManager<Task>, public Singleton < TaskManager >
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    TaskManager();

public:
    /** Default destructor.
    */
    virtual ~TaskManager();

    Task* createObject(const String& type, const String& name);

    template<class Function>
    Task* createObject(Function&& f)
    {
        CREATE_FACTORY(WrappedFunction<Function>);
        Task* pObj = createObject(GET_OBJECT_TYPE(WrappedFunction<Function>), BLANK);
        if (pObj != nullptr)
        {
            WrappedFunction<Function>* pWrappedFunc = dynamic_cast<WrappedFunction<Function>*>(pObj);
            if (pWrappedFunc != nullptr)
            {
                pWrappedFunc->initialize(f);
            }
        }
        return pObj;
    }

    virtual void destoryObject(Task* obj);

protected:
    virtual Task* createObject(const String& type, const String& name = BLANK);

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
    static TaskManager& getSingleton(void);

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
    static TaskManager* getSingletonPtr(void);
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Task_H__
