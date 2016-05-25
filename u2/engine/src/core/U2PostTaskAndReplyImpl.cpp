#include "U2PostTaskAndReplyImpl.h"

#include "U2Task.h"
#include "U2TaskRunner.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
// This relay class remembers the MessageLoop that it was created on, and
// ensures that both the |task| and |reply| Closures are deleted on this same
// thread. Also, |task| is guaranteed to be deleted before |reply| is run or
// deleted.
//
// If this is not possible because the originating MessageLoop is no longer
// available, the the |task| and |reply| Closures are leaked.  Leaking is
// considered preferable to having a thread-safetey violations caused by
// invoking the Closure destructor on the wrong thread.
class PostTaskAndReplyRelay
{
public:
    PostTaskAndReplyRelay(Task* task, Task* reply)
        : m_spOriginLoop(ThreadTaskRunnerHandle::Get())
    {
        task_ = task;
        reply_ = reply;
    }

    ~PostTaskAndReplyRelay()
    {
        assert(m_spOriginLoop->BelongsToCurrentThread());
        task_.Reset();
        reply_.Reset();
    }

    void Run()
    {
        task_->run();
        m_spOriginLoop->PostTask(
            TaskManager::getSingleton().createObject([=] { this->RunReplyAndSelfDestruct(); })
            );
    }

private:
    void RunReplyAndSelfDestruct()
    {
        assert(m_spOriginLoop->BelongsToCurrentThread());

        // Force |task_| to be released before |reply_| is to ensure that no one
        // accidentally depends on |task_| keeping one of its arguments alive while
        // |reply_| is executing.
        task_.Reset();

        reply_->run();

        // Cue mission impossible theme.
        U2_DELETE this;
    }

private:
    std::shared_ptr<SingleThreadTaskRunner> m_spOriginLoop;
    Task* reply_;
    Task* task_;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
bool PostTaskAndReplyImpl::PostTaskAndReply(Task* task, Task* reply)
{
    PostTaskAndReplyRelay* pRelay = U2_NEW PostTaskAndReplyRelay(task, reply);
    if (!PostTask(
        TaskManager::getSingleton().createObject([=] { pRelay->Run(); })
        ))
    {
        U2_DELETE pRelay;
        return false;
    }

    return true;
}