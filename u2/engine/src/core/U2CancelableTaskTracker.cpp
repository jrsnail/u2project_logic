#include "U2CancelableTaskTracker.h"

#include "U2Task.h"
#include "U2TaskRunner.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void RunIfNotCanceled(AtomicScalar<bool>* flag, Task* task)
{
    if (!flag->get())
        task->run();
}
//---------------------------------------------------------------------
void RunIfNotCanceledThenUntrack(AtomicScalar<bool>* flag,
    Task* task,
    Task* untrack)
{
    RunIfNotCanceled(flag, task);
    untrack->run();
}
//---------------------------------------------------------------------
void RunOrPostToTaskRunner(TaskRunner* task_runner, Task* task)
{
    if (task_runner->RunTasksOnCurrentThread())
        task->run();
    else
        task_runner->PostTask(task);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
const String CancelableTaskTracker::kBadTaskId = "BadTaskId";
//---------------------------------------------------------------------
CancelableTaskTracker::CancelableTaskTracker()
    : Object(GET_OBJECT_TYPE(CancelableTaskTracker), BLANK)
{
}
//---------------------------------------------------------------------
CancelableTaskTracker::~CancelableTaskTracker()
{
    TryCancelAll();
}
//---------------------------------------------------------------------
const String& CancelableTaskTracker::PostTask(TaskRunner* task_runner, Task* task)
{
    //DCHECK(thread_checker_.CalledOnValidThread());

    return PostTaskAndReply(task_runner, task, 
        // do nothing
        TaskManager::getSingleton().createObject([] {})
        );
}
//---------------------------------------------------------------------
const String& CancelableTaskTracker::PostTaskAndReply(
    TaskRunner* task_runner, Task* task, Task* reply)
{
    //DCHECK(thread_checker_.CalledOnValidThread());

    // We need a MessageLoop to run reply.
    //DCHECK(base::MessageLoopProxy::current().get());

    AtomicScalar<bool>* pFlag = new AtomicScalar<bool>(false);

    const String& szGuid = task->getGuid();
    Task* pUntrack = TaskManager::getSingleton().createObject([=] { 
        this->Untrack(szGuid);
    });
    bool bSuccess =
        task_runner->PostTaskAndReply(
            TaskManager::getSingleton().createObject([=] { RunIfNotCanceled(pFlag, task); }),
            TaskManager::getSingleton().createObject([=] { RunIfNotCanceledThenUntrack(pFlag, reply, pUntrack); })
            );

    if (!bSuccess)
        return kBadTaskId;

    Track(szGuid, pFlag);
    return szGuid;
}
//---------------------------------------------------------------------
void CancelableTaskTracker::TryCancel(const String& taskId)
{
    //DCHECK(thread_checker_.CalledOnValidThread());

    CancelFlagMap::const_iterator it = task_flags_.find(taskId);
    if (it == task_flags_.end())
    {
        // Two possibilities:
        //
        //   1. The task has already been untracked.
        //   2. The TaskId is bad or unknown.
        //
        // Since this function is best-effort, it's OK to ignore these.
        return;
    }
    it->second->set(true);
}
//---------------------------------------------------------------------
void CancelableTaskTracker::TryCancelAll()
{
    //DCHECK(thread_checker_.CalledOnValidThread());

    for (CancelFlagMap::const_iterator it = task_flags_.begin();
        it != task_flags_.end();
        ++it)
    {
        it->second->set(true);
    }
}
//---------------------------------------------------------------------
bool CancelableTaskTracker::HasTrackedTasks() const
{
    //DCHECK(thread_checker_.CalledOnValidThread());
    return !task_flags_.empty();
}
//---------------------------------------------------------------------
void CancelableTaskTracker::Track(const String& taskId, AtomicScalar<bool>* flag)
{
    //DCHECK(thread_checker_.CalledOnValidThread());
    CancelFlagMap::iterator it = task_flags_.find(taskId);
    if (it == task_flags_.end())
    {
    }
    else
    {
        bool success = task_flags_.insert(std::make_pair(taskId, flag)).second;
        assert(success);
    }
}
//---------------------------------------------------------------------
void CancelableTaskTracker::Untrack(const String& taskId)
{
    //DCHECK(thread_checker_.CalledOnValidThread());
    task_flags_.erase(taskId);
}