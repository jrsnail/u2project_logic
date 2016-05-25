#include "U2WorkerPool.h"

#include "U2Exception.h"
#include "U2Task.h"
#include "U2PostTaskAndReplyImpl.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
class PostTaskAndReplyWorkerPool : public PostTaskAndReplyImpl
{
public:
    explicit PostTaskAndReplyWorkerPool(bool task_is_slow)
        : m_bTaskIsSlow(task_is_slow)
    {
    }

private:
    virtual bool PostTask(Task* task) override
    {
        return WorkerPool::PostTask(task, m_bTaskIsSlow);
    }

private:
    bool m_bTaskIsSlow;
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// A TaskRunner which posts tasks to a WorkerPool with a
// fixed ShutdownBehavior.
//
// Note that this class is RefCountedThreadSafe (inherited from TaskRunner).
class WorkerPoolTaskRunner : public TaskRunner
{
public:
    explicit WorkerPoolTaskRunner(bool tasks_are_slow);

    // TaskRunner implementation
    virtual bool PostDelayedTask(Task* task, TimeDelta delay) override;
    virtual bool RunTasksOnCurrentThread() const override;

private:
    virtual ~WorkerPoolTaskRunner();

    // Helper function for posting a delayed task. Asserts that the delay is
    // zero because non-zero delays are not supported.
    bool PostDelayedTaskAssertZeroDelay(Task* task, TimeDelta delay);

    DISALLOW_COPY_AND_ASSIGN(WorkerPoolTaskRunner);

private:
    const bool tasks_are_slow_;
};
//---------------------------------------------------------------------
WorkerPoolTaskRunner::WorkerPoolTaskRunner(bool tasks_are_slow)
    : tasks_are_slow_(tasks_are_slow)
{
}
//---------------------------------------------------------------------
WorkerPoolTaskRunner::~WorkerPoolTaskRunner()
{
}
//---------------------------------------------------------------------
bool WorkerPoolTaskRunner::PostDelayedTask(Task* task, TimeDelta delay)
{
    return PostDelayedTaskAssertZeroDelay(task, delay);
}
//---------------------------------------------------------------------
bool WorkerPoolTaskRunner::RunTasksOnCurrentThread() const
{
    return WorkerPool::RunTasksOnCurrentThread();
}
//---------------------------------------------------------------------
bool WorkerPoolTaskRunner::PostDelayedTaskAssertZeroDelay(Task* task, TimeDelta delay)
{
    U2Assert(delay.InMillisecondsRoundedUp() == 0
        , "WorkerPoolTaskRunner does not support non-zero delays");
    return WorkerPool::PostTask(task, tasks_are_slow_);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
struct TaskRunnerHolder
{
    TaskRunnerHolder()
    {
        taskrunners_[0] = std::make_shared<WorkerPoolTaskRunner>(false);
        taskrunners_[1] = std::make_shared<WorkerPoolTaskRunner>(true);
    }
    std::shared_ptr<TaskRunner> taskrunners_[2];
};

TaskRunnerHolder g_taskrunners;
//---------------------------------------------------------------------
bool WorkerPool::PostTaskAndReply(Task* task, Task* reply, bool task_is_slow)
{
    return PostTaskAndReplyWorkerPool(task_is_slow).PostTaskAndReply(task, reply);
}
//---------------------------------------------------------------------
const std::shared_ptr<TaskRunner>& WorkerPool::GetTaskRunner(bool tasks_are_slow)
{
    return g_taskrunners.taskrunners_[tasks_are_slow];
}