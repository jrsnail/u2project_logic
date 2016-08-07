#ifndef __U2Scheduler_H__
#define __U2Scheduler_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Task.h"
#include "U2SimpleObjectManager.h"
#include "U2Timer.h"
#include "U2PredefinedPrerequisites.h"
#include "U2HeaderPrefix.h"



U2EG_NAMESPACE_BEGIN


class SchedulerTask : public Task
{
public:
    SchedulerTask(const String& type, const String& name);
    virtual ~SchedulerTask();

    void initialize(u2uint64 period
        , bool repeat = false, bool catchUp = true);

    u2uint64 getPeriod() { return m_ulPeriod; }
    bool isRepeated() { return m_bRepeat; }
    bool isCatchUp() { return m_bCatchup; }

    /** trigger callback.
    @param delta delta time between now and current trigger time
    @return next trigger time
    */
    u2uint64 trigger(u2uint64 delta)
    {
        u2int64 tempDelta = (u2int64)delta;
        while (tempDelta >= 0)
        {
            run();
            tempDelta -= m_ulPeriod;
            if (!m_bRepeat)
                return 0;
            if (m_ulPeriod == 0)
                return 0;
            if (!m_bCatchup)
                continue;
        }

        return (m_ulPeriod - (delta % m_ulPeriod));
    }

protected:
    u2uint64 m_ulPeriod;
    bool m_bRepeat;
    bool m_bCatchup;
};



/** This is a single thread scheduler, do not use it in multi thread at any time.
*/
class Scheduler : public SimpleObjectManager<SchedulerTask>, public Object
{
public:
    /**
    @param callOnFrameStarted true will cause timer callbacks to be sent
        during frame started, if false, callbacks will be sent during frame 
        ended.
    */
    Scheduler(const String& type, const String& name);
    virtual ~Scheduler();

    void initialize(bool callOnFrameStarted = true);

    template<class Function>
    SchedulerTask* createObjectWithFunction(Function&& f, const String& name
        , u2uint64 period, bool repeat = false, bool catchUp = true)
    {
        TaskLoop* pCurTaskLoop = TaskLoopManager::current();
        if (pCurTaskLoop == nullptr)
        {
            // should create logic task loop first
            assert(0);
        }
        else
        {
            // should run scheduler on logic(main) thread only
            assert(ON_Logic_TaskLoop == pCurTaskLoop->getName());
        }

        typedef typename std::remove_reference<Function>::type function_type;
        
        class WrappedFunction : public SchedulerTask
        {
        protected:
            Function f;

        public:
            WrappedFunction(const String& type, const String& name, function_type&& f)
                : SchedulerTask(type, name)
                , f(std::move(f))
            {
            }

            virtual void run() override
            {
                // as lamda created task, we must input lamda parameter in ctor, 
                // so we could not use object factory as creator, and the instance
                // should be deleted when function run() end.
                f();
                // delete in Scheduler::_next()
                //U2_DELETE this;
            }
        };

        //CREATE_FACTORY(WrappedFunction);
        //SchedulerTask* pTask = createObject(GET_OBJECT_TYPE(WrappedFunction), name);
        SchedulerTask* pTask = U2_NEW WrappedFunction(GET_OBJECT_TYPE(WrappedFunction), name, std::forward<Function>(f));
        pTask->initialize(period, repeat, catchUp);

        u2uint64 now = mTimer.getMilliseconds();
        m_SchedulerTaskMap.insert(std::make_pair(now + period, pTask));
        return pTask;
    }

    virtual void destoryObject(SchedulerTask* obj);

protected:
    virtual SchedulerTask* createObject(const String& type, const String& name = BLANK);

    void _onUpdate(float dt);

    bool _next();

protected:
    /// container for all our schedule task instances. 
    typedef std::multimap< u2uint64, SchedulerTask* >   SchedulerTaskMap;
    typedef std::pair<SchedulerTaskMap::iterator, SchedulerTaskMap::iterator> SchedulerTaskPair;
    SchedulerTaskMap m_SchedulerTaskMap;

    Timer mTimer;

    // determines if callbacks happen durring frame started, or frame ended. 
    bool m_bCallOnFrameStarted;
};



class SchedulerManager : public Singleton < SchedulerManager >, public SimpleObjectManager<Scheduler>
{
public:
    SchedulerManager();
    virtual ~SchedulerManager();

    virtual Scheduler* createObject(const String& type, const String& name, bool callOnFrameStarted);

protected:
    virtual Scheduler* createObject(const String& type, const String& name = BLANK);

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
    static SchedulerManager& getSingleton(void);

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
    static SchedulerManager* getSingletonPtr(void);

};



#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2Scheduler_H__
