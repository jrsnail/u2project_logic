#include "U2TaskLoop.h"

#include "U2Exception.h"
#include "U2Task.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
TaskLoop::TaskLoop(const String& type, const String& name)
    : Object(type, name)
{
    run();
}
//---------------------------------------------------------------------
TaskLoop::~TaskLoop()
{
    DestructionListenerList::iterator it = m_DestructionListeners.begin();
    while (it != m_DestructionListeners.end())
    {
        (*it)->WillDestroyCurrentMessageLoop();
    }
    m_DestructionListeners.clear();

    m_TaskListeners.clear();

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
void TaskLoop::addDestructionListener(DestructionListener* destruction_observer) 
{
    DestructionListenerList::iterator it 
        = std::find(m_DestructionListeners.begin(), m_DestructionListeners.end(), destruction_observer);
    if (it != m_DestructionListeners.end())
    {
        m_DestructionListeners.push_back(destruction_observer);
    }
}
//---------------------------------------------------------------------
void TaskLoop::removeDestructionListener(DestructionListener* destruction_observer) 
{
    DestructionListenerList::iterator it 
        = std::find(m_DestructionListeners.begin(), m_DestructionListeners.end(), destruction_observer);
    if (it != m_DestructionListeners.end())
    {
        m_DestructionListeners.erase(it);
    }
}
//---------------------------------------------------------------------
void TaskLoop::addTaskListener(TaskListener* listener)
{
    assert(this == MsgLoopManager::current());
    TaskListenerList::iterator it
        = std::find(m_TaskListeners.begin(), m_TaskListeners.end(), listener);
    if (it != m_TaskListeners.end())
    {
        m_TaskListeners.push_back(listener);
    }
}
//---------------------------------------------------------------------
void TaskLoop::removeTaskListener(TaskListener* listener)
{
    assert(this == MsgLoopManager::current());
    TaskListenerList::iterator it
        = std::find(m_TaskListeners.begin(), m_TaskListeners.end(), listener);
    if (it != m_TaskListeners.end())
    {
        m_TaskListeners.erase(it);
    }
}
//---------------------------------------------------------------------
void TaskLoop::_runTask(Task* task)
{
    for (TaskListenerList::iterator it = m_TaskListeners.begin(); 
        it != m_TaskListeners.end(); it++)
    {
        (*it)->WillProcessTask();
    }

    task->run();

    for (TaskListenerList::iterator it = m_TaskListeners.begin();
        it != m_TaskListeners.end(); it++)
    {
        (*it)->DidProcessTask();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
map<String, std::shared_ptr<TaskLoop> >::type MsgLoopManager::ms_MsgLoops;
//-----------------------------------------------------------------------
template<> MsgLoopManager* Singleton<MsgLoopManager>::msSingleton = 0;
MsgLoopManager* MsgLoopManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new MsgLoopManager;
    }
    return msSingleton;
}
MsgLoopManager& MsgLoopManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
MsgLoopManager::MsgLoopManager()
{
}
//-----------------------------------------------------------------------
MsgLoopManager::~MsgLoopManager()
{
}
//-----------------------------------------------------------------------
void MsgLoopManager::postTask(const String& loopName, Task* task)
{
    TaskLoop* pMsgLoop = retrieveObjectByName(loopName);
    if (pMsgLoop)
    {
        pMsgLoop->postTask(task);
    }
}
//-----------------------------------------------------------------------
void MsgLoopManager::postTaskAndReply(const String& loopName, Task* task, Task* reply)
{
    TaskLoop* pMsgLoop = retrieveObjectByName(loopName);
    if (pMsgLoop)
    {
        pMsgLoop->postTaskAndReply(task, reply);
    }
}
//---------------------------------------------------------------------
TaskLoop* MsgLoopManager::current()
{
    std::thread::id tid = std::this_thread::get_id();
    StringStream stream;
    stream << tid;
    String&& szTid = stream.str();
    map<String, std::shared_ptr<TaskLoop> >::iterator it = ms_MsgLoops.find(szTid);
    assert(it != ms_MsgLoops.end());
    return it->second.get();
}