#include "U2MsgLoop.h"

#include "U2Exception.h"
#include "U2Task.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
MsgLoop::AutoRunState::AutoRunState(MsgLoop* loop) 
    : m_pLoop(loop)
{
    // Make the loop reference us.
    m_pPreviousState = m_pLoop->m_pState;
    if (m_pPreviousState) 
    {
        nRunDepth = m_pPreviousState->nRunDepth + 1;
    }
    else 
    {
        nRunDepth = 1;
    }
    m_pLoop->m_pState = this;

    // Initialize the other fields:
    bQuitReceived = false;
}
//---------------------------------------------------------------------
MsgLoop::AutoRunState::~AutoRunState()
{
    m_pLoop->m_pState = m_pPreviousState;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
MsgLoop::MsgLoop(Type type)
    : m_eType(type)
    , m_nNextSequenceNum(0)
    , m_bNestableTasksAllowed(true)
    , m_pState(nullptr)
{
    /*
#if defined(OS_WIN)
#define MESSAGE_PUMP_UI new base::MessagePumpForUI()
#define MESSAGE_PUMP_IO new base::MessagePumpForIO()
#elif defined(OS_MACOSX)
#define MESSAGE_PUMP_UI base::MessagePumpMac::Create()
#define MESSAGE_PUMP_IO new base::MessagePumpLibevent()
#elif defined(OS_NACL)
    // Currently NaCl doesn't have a UI or an IO MessageLoop.
    // TODO(abarth): Figure out if we need these.
#define MESSAGE_PUMP_UI NULL
#define MESSAGE_PUMP_IO NULL
#elif defined(OS_POSIX)  // POSIX but not MACOSX.
#define MESSAGE_PUMP_UI new base::MessagePumpForUI()
#define MESSAGE_PUMP_IO new base::MessagePumpLibevent()
#else
#error Not implemented
#endif

    if (m_eType == TYPE_UI) 
    {
        if (message_pump_for_ui_factory_)
            pump_ = message_pump_for_ui_factory_();
        else
            pump_ = MESSAGE_PUMP_UI;
    }
    else if (m_eType == TYPE_IO) 
    {
        pump_ = MESSAGE_PUMP_IO;
    }
    else 
    {
        DCHECK_EQ(TYPE_DEFAULT, type_);
        pump_ = new base::MessagePumpDefault();
    }
    */
}
//---------------------------------------------------------------------
MsgLoop::~MsgLoop()
{
    U2Assert(m_pState == nullptr, "state should be null when destroyed.");

    DestructionListenerList::iterator it = m_DestructionListeners.begin();
    while (it != m_DestructionListeners.end())
    {
        (*it)->WillDestroyCurrentMessageLoop();
    }
    m_DestructionListeners.clear();

    /*
    // Clean up any unprocessed tasks, but take care: deleting a task could
    // result in the addition of more tasks (e.g., via DeleteSoon).  We set a
    // limit on the number of times we will allow a deleted task to generate more
    // tasks.  Normally, we should only pass through this loop once or twice.  If
    // we end up hitting the loop limit, then it is probably due to one task that
    // is being stubborn.  Inspect the queues to see who is left.
    bool did_work;
    for (int i = 0; i < 100; ++i) {
        DeletePendingTasks();
        ReloadWorkQueue();
        // If we end up with empty queues, then break out of the loop.
        did_work = DeletePendingTasks();
        if (!did_work)
            break;
    }
    */

}
//---------------------------------------------------------------------
MsgLoop* MsgLoop::current()
{
    // TODO(darin): sadly, we cannot enable this yet since people call us even
    // when they have no intention of using us.
    // DCHECK(loop) << "Ouch, did you forget to initialize me?";
    return lazy_tls_ptr.Pointer()->Get();
}
//---------------------------------------------------------------------
void MsgLoop::AddDestructionObserver(DestructionObserver* destruction_observer) 
{
    DestructionListenerList::iterator it 
        = std::find(m_DestructionListeners.begin(), m_DestructionListeners.end(), destruction_observer);
    if (it != m_DestructionListeners.end())
    {
        m_DestructionListeners.push_back(destruction_observer);
    }
}
//---------------------------------------------------------------------
void MsgLoop::RemoveDestructionObserver(DestructionObserver* destruction_observer) 
{
    DestructionListenerList::iterator it 
        = std::find(m_DestructionListeners.begin(), m_DestructionListeners.end(), destruction_observer);
    if (it != m_DestructionListeners.end())
    {
        m_DestructionListeners.erase(it);
    }
}
//---------------------------------------------------------------------
void MsgLoop::AddTaskObserver(TaskObserver* task_observer)
{
    DCHECK_EQ(this, current());
    TaskListenerList::iterator it
        = std::find(m_TaskListeners.begin(), m_TaskListeners.end(), task_observer);
    if (it != m_TaskListeners.end())
    {
        m_TaskListeners.push_back(task_observer);
    }
}
//---------------------------------------------------------------------
void MsgLoop::RemoveTaskObserver(TaskObserver* task_observer)
{
    DCHECK_EQ(this, current());
    TaskListenerList::iterator it
        = std::find(m_TaskListeners.begin(), m_TaskListeners.end(), task_observer);
    if (it != m_TaskListeners.end())
    {
        m_TaskListeners.erase(it);
    }
}
//---------------------------------------------------------------------
void MsgLoop::_addToIncomingQueue(PendingTask* pending_task)
{
    {
        U2_LOCK_MUTEX(m_mtxIncomingQueue);
        bool bWasEmpty = m_IncomingQueue.empty();
        m_IncomingQueue.push(pending_task);
        if (!bWasEmpty)
        {
            return;  // Someone else should have started the sub-pump.
        }
    }

    m_spPump->ScheduleWork();
}
//---------------------------------------------------------------------
void MsgLoop::_addToDelayedWorkQueue(PendingTask* pending_task) 
{
    // Move to the delayed work queue.  Initialize the sequence number
    // before inserting into the delayed_work_queue_.  The sequence number
    // is used to faciliate FIFO sorting when two tasks have the same
    // delayed_run_time value.
    PendingTask* new_pending_task = new PendingTask(*pending_task);
    new_pending_task->sequence_num = m_nNextSequenceNum++;
    delayed_work_queue_.push(new_pending_task);
}
//---------------------------------------------------------------------
TimeTicks MsgLoop::_calculateDelayedRuntime(u2int64 delay_ms) 
{
    TimeTicks delayed_run_time;
    if (delay_ms > 0) 
    {
        delayed_run_time = TimeTicks::Now() + TimeDelta::FromMilliseconds(delay_ms);
    }
    else 
    {
        U2Assert(delay_ms >= 0, "delay should not be negative.");
    }
    return delayed_run_time;
}
//---------------------------------------------------------------------
void MsgLoop::Run()
{
    AutoRunState save_state(this);
    _runInternal();
}
//---------------------------------------------------------------------
void MsgLoop::RunAllPending()
{
    AutoRunState save_state(this);
    m_pState->bQuitReceived = true;  // Means run until we would otherwise block.
    _runInternal();
}
//---------------------------------------------------------------------
void MsgLoop::_runInternal()
{
    DCHECK_EQ(this, current());

    m_spPump->Run(this);
}
//---------------------------------------------------------------------
bool MsgLoop::DoWork() 
{
    if (!m_bNestableTasksAllowed) 
    {
        // Task can't be executed right now.
        return false;
    }

    for (;;) 
    {
        _reloadWorkQueue();
        if (m_WorkQueue.empty())
            break;

        // Execute oldest task.
        do 
        {
            PendingTask* pending_task = m_WorkQueue.front();
            m_WorkQueue.pop();
            if (!pending_task->delayed_run_time.is_null()) 
            {
                _addToDelayedWorkQueue(pending_task);
                // If we changed the topmost task, then it is time to reschedule.
                if (delayed_work_queue_.top().task.Equals(pending_task.task))
                    m_spPump->ScheduleDelayedWork(pending_task->delayed_run_time);
            }
            else 
            {
                if (_deferOrRunPendingTask(pending_task))
                    return true;
            }
        } while (!m_WorkQueue.empty());
    }

    // Nothing happened.
    return false;
}
//---------------------------------------------------------------------
bool MsgLoop::DoDelayedWork(TimeTicks* next_delayed_work_time) 
{
    if (!m_bNestableTasksAllowed || delayed_work_queue_.empty())
    {
        m_RecentTime = *next_delayed_work_time = TimeTicks();
        return false;
    }

    // When we "fall behind," there will be a lot of tasks in the delayed work
    // queue that are ready to run.  To increase efficiency when we fall behind,
    // we will only call Time::Now() intermittently, and then process all tasks
    // that are ready to run before calling it again.  As a result, the more we
    // fall behind (and have a lot of ready-to-run delayed tasks), the more
    // efficient we'll be at handling the tasks.

    TimeTicks next_run_time = delayed_work_queue_.top()->delayed_run_time;
    if (next_run_time > m_RecentTime)
    {
        m_RecentTime = TimeTicks::Now();  // Get a better view of Now();
        if (next_run_time > m_RecentTime)
        {
            *next_delayed_work_time = next_run_time;
            return false;
        }
    }

    PendingTask* pending_task = delayed_work_queue_.top();
    delayed_work_queue_.pop();

    if (!delayed_work_queue_.empty())
        *next_delayed_work_time = delayed_work_queue_.top()->delayed_run_time;

    return _deferOrRunPendingTask(pending_task);
}
//---------------------------------------------------------------------
bool MsgLoop::DoIdleWork()
{
    if (_processNextDelayedNonNestableTask())
        return true;

    if (m_pState->bQuitReceived)
        m_spPump->Quit();

    return false;
}
//---------------------------------------------------------------------
bool MsgLoop::_deferOrRunPendingTask(PendingTask* pending_task)
{
    if (pending_task->nestable || m_pState->nRunDepth == 1)
    {
        _runTask(pending_task);
        // Show that we ran a task (Note: a new one might arrive as a
        // consequence!).
        return true;
    }

    // We couldn't run the task now because we're in a nested message loop
    // and the task isn't nestable.
    m_DeferredNonNestableWorkQueue.push(pending_task);
    return false;
}
//---------------------------------------------------------------------
void MsgLoop::_runTask(PendingTask* pending_task)
{
    U2Assert(m_bNestableTasksAllowed, "");

    // Execute the task and assume the worst: It is probably not reentrant.
    m_bNestableTasksAllowed = false;

    for (TaskListenerList::iterator it = m_TaskListeners.begin(); 
        it != m_TaskListeners.end(); it++)
    {
        (*it)->WillProcessTask(pending_task->time_posted);
    }

    (*pending_task)();

    for (TaskListenerList::iterator it = m_TaskListeners.begin();
        it != m_TaskListeners.end(); it++)
    {
        (*it)->DidProcessTask(pending_task->time_posted);
    }

    m_bNestableTasksAllowed = true;
}
//---------------------------------------------------------------------
void MsgLoop::SetNestableTasksAllowed(bool allowed) 
{
    if (m_bNestableTasksAllowed != allowed) 
    {
        m_bNestableTasksAllowed = allowed;
        if (!m_bNestableTasksAllowed)
            return;
        // Start the native pump if we are not already pumping.
        m_spPump->ScheduleWork();
    }
}
//---------------------------------------------------------------------
bool MsgLoop::isNestableTasksAllowed() const 
{
    return m_bNestableTasksAllowed;
}
//---------------------------------------------------------------------
void MsgLoop::_reloadWorkQueue()
{
    // We can improve performance of our loading tasks from incoming_queue_ to
    // work_queue_ by waiting until the last minute (work_queue_ is empty) to
    // load.  That reduces the number of locks-per-task significantly when our
    // queues get large.
    if (!m_WorkQueue.empty())
        return;  // Wait till we *really* need to lock and load.

    // Acquire all we can from the inter-thread queue with one lock acquisition.
    {
        U2_LOCK_MUTEX(m_mtxIncomingQueue);
        if (m_IncomingQueue.empty())
            return;
        m_IncomingQueue.swap(m_WorkQueue);  // Constant time
        //DCHECK(incoming_queue_.empty());
    }
}
//---------------------------------------------------------------------
bool MsgLoop::_processNextDelayedNonNestableTask()
{
    if (m_pState->nRunDepth != 1)
        return false;

    if (m_DeferredNonNestableWorkQueue.empty())
        return false;

    PendingTask* pending_task = m_DeferredNonNestableWorkQueue.front();
    m_DeferredNonNestableWorkQueue.pop();

    _runTask(pending_task);
    return true;
}
//---------------------------------------------------------------------
void MsgLoop::Quit() 
{
    DCHECK_EQ(this, current());
    if (m_pState) 
    {
        m_pState->bQuitReceived = true;
    }
}
//---------------------------------------------------------------------
void MsgLoop::QuitNow() 
{
    DCHECK_EQ(this, current());
    if (m_pState)
    {
        m_spPump->Quit();
    }
}