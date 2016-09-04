#include "U2TaskLoop.h"

#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2Task.h"
#include "U2Scheduler.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
TaskLoop::TaskLoop(const String& type, const String& name, const String& guid)
    : Object(type, name, guid)
    , m_pScheduler(nullptr)
{
    
}
//---------------------------------------------------------------------
TaskLoop::~TaskLoop()
{
    // destroy scheduler
    if (m_pScheduler != nullptr)
    {
        SchedulerManager::getSingleton().destoryObject(m_pScheduler);
        m_pScheduler = nullptr;
    }

    _preDestroyCurrentTaskLoop();

    m_TaskListeners.clear();
}
//---------------------------------------------------------------------
void TaskLoop::addTaskLoopListener(TaskLoopListener* listener)
{
    TaskLoopListenerList::iterator it 
        = std::find(m_TaskLoopListeners.begin(), m_TaskLoopListeners.end(), listener);
    if (it == m_TaskLoopListeners.end())
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
    assert(this == TaskLoopManager::current());
    TaskListenerList::iterator it
        = std::find(m_TaskListeners.begin(), m_TaskListeners.end(), listener);
    if (it == m_TaskListeners.end())
    {
        m_TaskListeners.push_back(listener);
    }
}
//---------------------------------------------------------------------
void TaskLoop::removeTaskListener(TaskListener* listener)
{
    assert(this == TaskLoopManager::current());
    TaskListenerList::iterator it
        = std::find(m_TaskListeners.begin(), m_TaskListeners.end(), listener);
    if (it != m_TaskListeners.end())
    {
        m_TaskListeners.erase(it);
    }
}
//---------------------------------------------------------------------
void TaskLoop::postSchedulerTask(const String& schedulerTaskName
    , const String& taskType, const String& taskName
    , u2uint64 period, bool repeat, bool catchUp)
{
    if (m_pScheduler == nullptr)
    {
        m_pScheduler = SchedulerManager::getSingleton().createObject(GET_OBJECT_TYPE(Scheduler), BLANK, BLANK, true);
    }
    m_pScheduler->createObjectWithFunction([=]() {
        //LogManager::getSingleton().stream(LML_TRIVIAL) << "scheduler callback";
        Task* pTask = TaskManager::getSingleton().createObject(taskType, taskName);
        TaskLoopManager::getSingleton().postTask(getName(), pTask);
    }, schedulerTaskName, period, repeat, catchUp);
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

    TaskManager::getSingleton().destoryObject(task);
}
//-----------------------------------------------------------------------
void TaskLoop::_postRunCurrentTaskLoop()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->postRunCurrentTaskLoop(this);
    }
}
//-----------------------------------------------------------------------
void TaskLoop::_postQuitCurrentTaskLoop()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->postQuitCurrentTaskLoop(this);
    }
}
//-----------------------------------------------------------------------
void TaskLoop::_prePauseCurrentTaskLoop()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->prePauseCurrentTaskLoop(this);
    }
}
//-----------------------------------------------------------------------
void TaskLoop::_postResumeCurrentTaskLoop()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->postResumeCurrentTaskLoop(this);
    }
}
//-----------------------------------------------------------------------
void TaskLoop::_preDestroyCurrentTaskLoop()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->preDestroyCurrentTaskLoop(this);
    }
    m_TaskLoopListeners.clear();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
map<String, TaskLoop* >::type TaskLoopManager::ms_TaskLoops;
//-----------------------------------------------------------------------
template<> TaskLoopManager* Singleton<TaskLoopManager>::msSingleton = 0;
TaskLoopManager* TaskLoopManager::getSingletonPtr(void)
{
    return msSingleton;
}
TaskLoopManager& TaskLoopManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
TaskLoopManager::TaskLoopManager()
{
    
}
//-----------------------------------------------------------------------
TaskLoopManager::~TaskLoopManager()
{
    quitAll();
}
//-----------------------------------------------------------------------
TaskLoop* TaskLoopManager::createObject(const String& type, const String& name, const String& guid)
{
    TaskLoop* pTaskLoop = SimpleObjectManager<TaskLoop>::createObject(type, name, guid);
    if (pTaskLoop != nullptr)
    {
        pTaskLoop->addTaskLoopListener(this);
    }
    return pTaskLoop;
}
//-----------------------------------------------------------------------
void TaskLoopManager::postTask(const String& loopName, Task* task)
{
    TaskLoop* pMsgLoop = retrieveObjectByName(loopName);
    if (pMsgLoop)
    {
        pMsgLoop->postTask(task);
    }
}
//-----------------------------------------------------------------------
void TaskLoopManager::postTaskAndReply(const String& loopName, Task* task, Task* reply)
{
    TaskLoop* pMsgLoop = retrieveObjectByName(loopName);
    if (pMsgLoop)
    {
        pMsgLoop->postTaskAndReply(task, reply);
    }
}
//---------------------------------------------------------------------
TaskLoop* TaskLoopManager::current()
{
    std::thread::id tid = std::this_thread::get_id();
    StringStream stream;
    stream << tid;
    String&& szTid = stream.str();
    TaskLoopMap::iterator it = ms_TaskLoops.find(szTid);
    assert(it != ms_TaskLoops.end());
    return it->second;
}
//---------------------------------------------------------------------
void TaskLoopManager::quitAll()
{
    ObjectMapIterator it = TaskLoopManager::getSingleton().retrieveAllObjects();
    while (it.hasMoreElements())
    {
        TaskLoop* pTaskLoop = it.getNext();
        if (pTaskLoop != nullptr)
        {
            if (pTaskLoop->isRunning())
            {
                pTaskLoop->quit();
                pTaskLoop->join();
            }
        }
    }
    ms_TaskLoops.clear();
}
//---------------------------------------------------------------------
void TaskLoopManager::runAll()
{
    ObjectMapIterator it = TaskLoopManager::getSingleton().retrieveAllObjects();
    while (it.hasMoreElements())
    {
        TaskLoop* pTaskLoop = it.getNext();
        if (pTaskLoop != nullptr)
        {
            if (!pTaskLoop->isRunning())
            {
                pTaskLoop->run();
            }
        }
    }
}
//---------------------------------------------------------------------
void TaskLoopManager::pauseAll()
{
    ObjectMapIterator it = TaskLoopManager::getSingleton().retrieveAllObjects();
    while (it.hasMoreElements())
    {
        TaskLoop* pTaskLoop = it.getNext();
        if (pTaskLoop != nullptr)
        {
            if (pTaskLoop->isRunning() && !pTaskLoop->isPausing())
            {
                pTaskLoop->pause();
            }
        }
    }
}
//---------------------------------------------------------------------
void TaskLoopManager::resumeAll()
{
    ObjectMapIterator it = TaskLoopManager::getSingleton().retrieveAllObjects();
    while (it.hasMoreElements())
    {
        TaskLoop* pTaskLoop = it.getNext();
        if (pTaskLoop != nullptr)
        {
            if (pTaskLoop->isRunning() && pTaskLoop->isPausing())
            {
                pTaskLoop->resume();
            }
        }
    }
}
//---------------------------------------------------------------------
void TaskLoopManager::postRunCurrentTaskLoop(TaskLoop* loop)
{
    String szId = loop->getThreadId();
    TaskLoopMap::iterator it = ms_TaskLoops.find(szId);
    if (it == ms_TaskLoops.end())
    {
        ms_TaskLoops[szId] = loop;
    }
    else
    {
        assert(0);
    }
}
//---------------------------------------------------------------------
void TaskLoopManager::postQuitCurrentTaskLoop(TaskLoop* loop)
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
void TaskLoopManager::prePauseCurrentTaskLoop(TaskLoop* loop)
{

}
//---------------------------------------------------------------------
void TaskLoopManager::postResumeCurrentTaskLoop(TaskLoop* loop)
{

}
//---------------------------------------------------------------------
void TaskLoopManager::preDestroyCurrentTaskLoop(TaskLoop* loop)
{

}