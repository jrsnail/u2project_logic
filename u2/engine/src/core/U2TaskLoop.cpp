#include "U2TaskLoop.h"

#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2Task.h"
#include "U2Scheduler.h"
#include "U2LogicClient.h"
#include "U2WebSocketClientImpl.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
TaskLoop::TaskLoop(const String& type, const String& name)
    : Object(type, name)
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

    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->preDestroyCurrentTaskLoop(this);
    }
    m_TaskLoopListeners.clear();

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
//-----------------------------------------------------------------------
void TaskLoop::run()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->postRunCurrentTaskLoop(this);
    }
}
//-----------------------------------------------------------------------
void TaskLoop::quit()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->preQuitCurrentTaskLoop(this);
    }
}
//---------------------------------------------------------------------
void TaskLoop::pause()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->prePauseCurrentTaskLoop(this);
    }
}
//---------------------------------------------------------------------
void TaskLoop::resume()
{
    // copy, avaid to interrupt iterator
    TaskLoopListenerList v = m_TaskLoopListeners;
    for (TaskLoopListenerList::iterator it = v.begin(); it != v.end(); it++)
    {
        (*it)->postResumeCurrentTaskLoop(this);
    }
}
//---------------------------------------------------------------------
void TaskLoop::postSchedulerTask(const String& schedulerTaskName
    , const String& taskType, const String& taskName
    , u2uint64 period, bool repeat, bool catchUp)
{
    if (m_pScheduler == nullptr)
    {
        m_pScheduler = SchedulerManager::getSingleton().createObject(GET_OBJECT_TYPE(Scheduler), BLANK, true);
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
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
map<String, std::shared_ptr<TaskLoop> >::type TaskLoopManager::ms_TaskLoops;
//-----------------------------------------------------------------------
template<> TaskLoopManager* Singleton<TaskLoopManager>::msSingleton = 0;
TaskLoopManager* TaskLoopManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new TaskLoopManager;
    }
    return msSingleton;
}
TaskLoopManager& TaskLoopManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
TaskLoopManager::TaskLoopManager()
{
    CREATE_FACTORY(LogicTaskLoop);
    CREATE_FACTORY(JsonWsTaskLoop);
}
//-----------------------------------------------------------------------
TaskLoopManager::~TaskLoopManager()
{
}
//-----------------------------------------------------------------------
TaskLoop* TaskLoopManager::createObject(const String& type, const String& name)
{
    TaskLoop* pTaskLoop = SimpleObjectManager<TaskLoop>::createObject(type, name);
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
    return it->second.get();
}
//---------------------------------------------------------------------
void TaskLoopManager::postRunCurrentTaskLoop(TaskLoop* loop)
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
void TaskLoopManager::preQuitCurrentTaskLoop(TaskLoop* loop)
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