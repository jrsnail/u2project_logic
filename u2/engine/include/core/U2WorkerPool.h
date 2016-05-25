#ifndef __U2WorkerPool_H__
#define __U2WorkerPool_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2TaskRunner.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Task;


class _U2Export WorkerPool
{
public:
    // This function posts |task| to run on a worker thread.  |task_is_slow|
    // should be used for tasks that will take a long time to execute.  Returns
    // false if |task| could not be posted to a worker thread.  Regardless of
    // return value, ownership of |task| is transferred to the worker pool.
    static bool PostTask(Task* task, bool task_is_slow);

    // Just like MessageLoopProxy::PostTaskAndReply, except the destination
    // for |task| is a worker thread and you can specify |task_is_slow| just
    // like you can for PostTask above.
    static bool PostTaskAndReply(Task* task, Task* reply, bool task_is_slow);

    // Return true if the current thread is one that this WorkerPool runs tasks
    // on.  (Note that if the Windows worker pool is used without going through
    // this WorkerPool interface, RunsTasksOnCurrentThread would return false on
    // those threads.)
    static bool RunTasksOnCurrentThread();

    // Get a TaskRunner wrapper which posts to the WorkerPool using the given
    // |task_is_slow| behavior.
    static const std::shared_ptr<TaskRunner>& GetTaskRunner(bool task_is_slow);
};



U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2WorkerPool_H__
