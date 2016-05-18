#ifndef __U2Task_H__
#define __U2Task_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class _U2Export Task : public Object, public GeneralAllocatedObject
{
public:
    /** Constructor - don't call direct, used by ArchiveFactory.
    */
	Task(const String& type, const String& name )
        : Object(type, name)
	{}

    /** Default destructor.
    */
    virtual ~Task() {}

	// Tasks are automatically deleted after Run is called.  
	virtual void run() = 0;
};


class _U2Export PendingTask : public ThreadPoolTask
{
public:
    PendingTask();

    PendingTask(TimeTicks delayed_run_time, bool nestable);

    // Used to support sorting.
    bool operator<(const PendingTask& other) const;

public:
    // The time when the task should be run.
    TimeTicks delayed_run_time;
    // Secondary sort key for run time.
    int sequence_num;

    // OK to dispatch from a nested loop.
    bool nestable;
};


template<class Function>
class WrappedFunction : public PendingTask
{
protected:
    typedef typename std::remove_reference<Function>::type function_type;
    Function f;

public:
    WrappedFunction(function_type&& f) 
        : PendingTask()
        , f(std::move(f)) 
    { }

    virtual void operator()() override
    {
        f();
        delete this;
    }
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Task_H__
