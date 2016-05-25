#include "U2TaskRunner.h"

#include "U2Task.h"
#include "U2PostTaskAndReplyImpl.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
// TODO(akalin): There's only one other implementation of
// PostTaskAndReplyImpl in WorkerPool.  Investigate whether it'll be
// possible to merge the two.
class PostTaskAndReplyTaskRunner : public PostTaskAndReplyImpl
{
public:
    explicit PostTaskAndReplyTaskRunner(TaskRunner* destination);

private:
    virtual bool PostTask(Task* task) override;

private:
    // Non-owning.
    TaskRunner* m_pDestination;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
PostTaskAndReplyTaskRunner::PostTaskAndReplyTaskRunner(TaskRunner* destination) 
    : m_pDestination(destination)
{
    assert(m_pDestination != nullptr);
}
//---------------------------------------------------------------------
bool PostTaskAndReplyTaskRunner::PostTask(Task* task)
{
    return m_pDestination->PostTask(task);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
TaskRunner::TaskRunner()
{}
//---------------------------------------------------------------------
TaskRunner::~TaskRunner()
{}
//---------------------------------------------------------------------
void TaskRunner::OnDestruct() const
{
    //delete this;
}
//---------------------------------------------------------------------
bool TaskRunner::PostTask(Task* task)
{
    return PostDelayedTask(task, TimeDelta());
}
//---------------------------------------------------------------------
bool TaskRunner::PostTaskAndReply(Task* task, Task* reply)
{
    return PostTaskAndReplyTaskRunner(this).PostTaskAndReply(task, reply);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
bool SequencedTaskRunner::PostNonNestableTask(Task* task)
{
    return PostNonNestableDelayedTask(task, TimeDelta());
}
//---------------------------------------------------------------------
bool SequencedTaskRunner::DeleteSoon(void(*deleter)(const void*), const void* object)
{
    return PostNonNestableTask(
        TaskManager::getSingleton().createObject([=] { deleter(object); })
        );
}