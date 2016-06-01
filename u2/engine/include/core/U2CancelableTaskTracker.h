#ifndef __U2CancelableTaskTracker_H__
#define __U2CancelableTaskTracker_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2AtomicScalar.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Task;
class TaskRunner;


class _U2Export CancelableTaskTracker : public Object, public GeneralAllocatedObject
{
public:
    CancelableTaskTracker();
    // Cancels all tracked tasks.
    virtual ~CancelableTaskTracker();

    const String& PostTask(TaskRunner* task_runner, Task* task);

    const String& PostTaskAndReply(TaskRunner* task_runner, Task* task, Task* reply);

    // After calling this function, |task| and |reply| will not run. If the
    // cancelation happens when |task| is running or has finished running, |reply|
    // will not run. If |reply| is running or has finished running, cancellation
    // is a noop.
    //
    // Note. It's OK to cancel a |task| for more than once. The later calls are
    // noops.
    void TryCancel(const String& taskId);

    // It's OK to call this function for more than once. The later calls are
    // noops.
    void TryCancelAll();

    // Returns true iff there are in-flight tasks that are still being
    // tracked.
    bool HasTrackedTasks() const;

protected:
    void Track(const String& taskId, AtomicScalar<bool>* flag);
    void Untrack(const String& taskId);

private:
    U2_DISALLOW_COPY_AND_ASSIGN(CancelableTaskTracker);

public:
    static const String kBadTaskId;

protected:
    typedef map<String, AtomicScalar<bool>* >    CancelFlagMap;
    CancelFlagMap::type task_flags_;

};



U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2CancelableTaskTracker_H__
