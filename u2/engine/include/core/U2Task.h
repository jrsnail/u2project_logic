#ifndef __U2Task_H__
#define __U2Task_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"
#include "U2TaskLoop.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class _U2Export Task : public Object, public GeneralAllocatedObject
{
public:
    /** Constructor - don't call direct, used by ArchiveFactory.
    */
	Task(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : Object(type, name, guid)
	{}

    /** Default destructor.
    */
    virtual ~Task() {}

	// Tasks are automatically deleted after Run is called.  
	virtual void run() = 0;

};


// This relay class remembers the MessageLoop that it was created on, and
// ensures that both the |task| and |reply| Closures are deleted on this same
// thread. Also, |task| is guaranteed to be deleted before |reply| is run or
// deleted.
//
// If this is not possible because the originating MessageLoop is no longer
// available, the the |task| and |reply| Closures are leaked.  Leaking is
// considered preferable to having a thread-safetey violations caused by
// invoking the Closure destructor on the wrong thread.
class PostTaskAndReplyRelay : public Task
{
public:
    PostTaskAndReplyRelay(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~PostTaskAndReplyRelay();

    void initialize(Task* task, Task* reply);

    virtual void run();

protected:
    void runReplyAndSelfDestruct();

protected:
    std::shared_ptr<TaskLoop> m_spOriginLoop;
    Task* m_pReply;
    Task* m_pTask;
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

    virtual Task* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

    template<class Function>
    Task* createObjectWithFunction(Function&& f)
    {
        typedef typename std::remove_reference<Function>::type function_type;

        class WrappedFunction : public Task
        {
        protected:
            Function f;

        public:
            WrappedFunction(const String& type, const String& name, function_type&& f)
                : Task(type, name)
                , f(std::move(f))
            {
            }

            virtual void run() override
            {
                // as lamda created task, we must input lamda parameter in ctor, 
                // so we could not use object factory as creator, and the instance
                // should be deleted when function run() end.
                f();
                U2_DELETE this;
            }
        };

//         CREATE_FACTORY(WrappedFunction);
//         return createObject(GET_OBJECT_TYPE(WrappedFunction), BLANK);
        return U2_NEW WrappedFunction(GET_OBJECT_TYPE(WrappedFunction), BLANK, std::forward<Function>(f));
    }

    PostTaskAndReplyRelay* createObject(const String& type, const String& name, const String& guid
        , Task* task, Task* reply);

    virtual void destoryObject(Task* obj);

    void destoryObjectByName(const String& name);

    Task* retrieveObjectByName(const String& name);

    Task* retrieveObjectByGuid(const String& guid);

    bool hasObjectByName(const String& name);

    void addObject(Task* obj);

    void removeObject(Task* obj);

protected:
    ObjectMapIterator retrieveAllObjects()
    {
        return SimpleObjectManager<Task>::retrieveAllObjects();
    }

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

protected:
    U2_AUTO_MUTEX;
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Task_H__
