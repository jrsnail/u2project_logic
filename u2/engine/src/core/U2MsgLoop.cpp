#include "U2MsgLoop.h"

#include "U2Exception.h"
#include "U2Task.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
MsgLoop::MsgLoop(Type type)
    : m_eType(type)
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
    std::thread::id tid = std::this_thread::get_id();
    StringStream stream;
    stream << tid;
    String&& szTid = stream.str();
    map<String, std::shared_ptr<MsgLoop> >::iterator it = ms_MsgLoops.find(szTid);
    assert(it != ms_MsgLoops.end());
    return it->second.get();
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
    assert(this == current());
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
    assert(this == current());
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
void MsgLoop::Run()
{
    _runInternal();
}
//---------------------------------------------------------------------
void MsgLoop::RunAllPending()
{
    _runInternal();
}
//---------------------------------------------------------------------
void MsgLoop::_runInternal()
{
    assert(this == current());

    m_spPump->Run(this);
}
//---------------------------------------------------------------------
bool MsgLoop::DoWork() 
{
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
            _runTask(pending_task);
            return true;
        } while (!m_WorkQueue.empty());
    }

    // Nothing happened.
    return false;
}
//---------------------------------------------------------------------
void MsgLoop::_runTask(PendingTask* pending_task)
{
    for (TaskListenerList::iterator it = m_TaskListeners.begin(); 
        it != m_TaskListeners.end(); it++)
    {
        (*it)->WillProcessTask();
    }

    (*pending_task)();

    for (TaskListenerList::iterator it = m_TaskListeners.begin();
        it != m_TaskListeners.end(); it++)
    {
        (*it)->DidProcessTask();
    }
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
void MsgLoop::Quit() 
{
    m_spPump->Quit();
}