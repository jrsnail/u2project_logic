#include "U2TaskLoop.h"

#include "U2Exception.h"
#include "U2Task.h"
#include "U2WebSocketClientImpl.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
TaskLoop::TaskLoop(const String& type, const String& name)
    : Object(type, name)
{
    
}
//---------------------------------------------------------------------
TaskLoop::~TaskLoop()
{
    TaskLoopListenerList::iterator it = m_TaskLoopListeners.begin();
    while (it != m_TaskLoopListeners.end())
    {
        (*it)->preDestroyCurrentTaskLoop(this);
        it = m_TaskLoopListeners.begin();
    }
    m_TaskLoopListeners.clear();

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
void TaskLoop::addTaskLoopListener(TaskLoopListener* listener)
{
    TaskLoopListenerList::iterator it 
        = std::find(m_TaskLoopListeners.begin(), m_TaskLoopListeners.end(), listener);
    if (it != m_TaskLoopListeners.end())
    {
        m_TaskLoopListeners.push_back(listener);
    }
}
//---------------------------------------------------------------------
void TaskLoop::removeTaskLoopListener(TaskLoopListener* listener)
{
    TaskLoopListenerList::iterator it 
        = std::find(m_TaskLoopListeners.begin(), m_TaskLoopListeners.end(), listener);
    if (it != m_TaskLoopListeners.end())
    {
        m_TaskLoopListeners.erase(it);
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
//-----------------------------------------------------------------------
void TaskLoop::run()
{
    TaskLoopListenerList::iterator it = m_TaskLoopListeners.begin();
    while (it != m_TaskLoopListeners.end())
    {
        (*it)->postRunCurrentTaskLoop(this);
        it = m_TaskLoopListeners.begin();
    }
}
//-----------------------------------------------------------------------
void TaskLoop::quit()
{
    TaskLoopListenerList::iterator it = m_TaskLoopListeners.begin();
    while (it != m_TaskLoopListeners.end())
    {
        (*it)->preQuitCurrentTaskLoop(this);
        it = m_TaskLoopListeners.begin();
    }
}
//---------------------------------------------------------------------
void TaskLoop::pause()
{
    TaskLoopListenerList::iterator it = m_TaskLoopListeners.begin();
    while (it != m_TaskLoopListeners.end())
    {
        (*it)->prePauseCurrentTaskLoop(this);
        it = m_TaskLoopListeners.begin();
    }
}
//---------------------------------------------------------------------
void TaskLoop::resume()
{
    TaskLoopListenerList::iterator it = m_TaskLoopListeners.begin();
    while (it != m_TaskLoopListeners.end())
    {
        (*it)->postResumeCurrentTaskLoop(this);
        it = m_TaskLoopListeners.begin();
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
map<String, std::shared_ptr<TaskLoop> >::type MsgLoopManager::ms_TaskLoops;
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
    CREATE_FACTORY(JsonWsTaskLoop);
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
    TaskLoopMap::iterator it = ms_TaskLoops.find(szTid);
    assert(it != ms_TaskLoops.end());
    return it->second.get();
}
//---------------------------------------------------------------------
void MsgLoopManager::postRunCurrentTaskLoop(TaskLoop* loop)
{
    String szId = loop->getThreadId();
    TaskLoopMap::iterator it = ms_TaskLoops.find(szId);
    if (it == ms_TaskLoops.end())
    {
        ms_TaskLoops[szId] = std::shared_ptr<TaskLoop>(loop);
    }
    else
    {
        assert(0);
    }
}
//---------------------------------------------------------------------
void MsgLoopManager::preQuitCurrentTaskLoop(TaskLoop* loop)
{
    String szId = loop->getThreadId();
    TaskLoopMap::iterator it = ms_TaskLoops.find(szId);
    if (it == ms_TaskLoops.end())
    {
        assert(0);
    }
    else
    {
        ms_TaskLoops.erase(it);
    }
}
//---------------------------------------------------------------------
void MsgLoopManager::prePauseCurrentTaskLoop(TaskLoop* loop)
{

}
//---------------------------------------------------------------------
void MsgLoopManager::postResumeCurrentTaskLoop(TaskLoop* loop)
{

}
//---------------------------------------------------------------------
void MsgLoopManager::preDestroyCurrentTaskLoop(TaskLoop* loop)
{

}