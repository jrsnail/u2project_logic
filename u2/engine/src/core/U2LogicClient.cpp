#include "U2LogicClient.h"

#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2FrameListenerCollection.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LogicTaskLoop::LogicTaskLoop(const String& type, const String& name)
    : TaskLoop(type, name)
    , m_bKeepRunning(true)
    , m_bPausing(false)
{
}
//-----------------------------------------------------------------------
LogicTaskLoop::~LogicTaskLoop()
{
}
//-----------------------------------------------------------------------
void LogicTaskLoop::postTask(Task* task)
{
    _addToIncomingQueue(task);
}
//-----------------------------------------------------------------------
void LogicTaskLoop::postTaskAndReply(Task* task, Task* reply)
{
    assert(0);
}
//-----------------------------------------------------------------------
void LogicTaskLoop::run()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = true;

    FrameListenerCollection::getSingleton().addFrameListener(this
        , std::bind(&LogicTaskLoop::_onUpdate, this, std::placeholders::_1));

    TaskLoop::run();
}
//-----------------------------------------------------------------------
void LogicTaskLoop::quit()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = false;

    TaskLoop::quit();
}
//-----------------------------------------------------------------------
void LogicTaskLoop::pause()
{
    U2_LOCK_MUTEX(m_PausingMutex);
    m_bPausing = true;
    TaskLoop::pause();
}
//-----------------------------------------------------------------------
void LogicTaskLoop::resume()
{
    U2_LOCK_MUTEX(m_PausingMutex);
    m_bPausing = false;
    TaskLoop::resume();
}
//-----------------------------------------------------------------------
String LogicTaskLoop::getThreadId()
{
    StringStream stream;
    stream << std::this_thread::get_id();
    return stream.str();
}
//-----------------------------------------------------------------------
void LogicTaskLoop::_onUpdate(float dt)
{
    {
        U2_LOCK_MUTEX(m_KeepRunningMutex);
        if (!m_bKeepRunning)
        {
            FrameListenerCollection::getSingleton().removeFrameListener(this);
            return;
        }
    }

    {
        U2_LOCK_MUTEX(m_PausingMutex);
        if (m_bPausing)
        {
            U2_THREAD_SLEEP(1000);
            return;
        }
    }

    if (m_WorkingQueue.empty())
    {
        bool bEmpty = true;
        {
            U2_LOCK_MUTEX(m_mtxIncomingQueue);
            bEmpty = m_IncomingQueue.empty();
            if (!bEmpty)
            {
                while (!m_IncomingQueue.empty())
                {
                    m_WorkingQueue.push_back(m_IncomingQueue.front());
                    m_IncomingQueue.pop_front();
                }
            }
        }
        if (bEmpty)
        {
            U2_THREAD_SLEEP(1000);
        }
    }

    while (!m_WorkingQueue.empty())
    {
        {
            U2_LOCK_MUTEX(m_KeepRunningMutex);
            if (!m_bKeepRunning)
                break;
        }

        {
            U2_LOCK_MUTEX(m_PausingMutex);
            if (m_bPausing)
            {
                U2_THREAD_SLEEP(1000);
                continue;
            }
        }

        Task* pTask = m_WorkingQueue.front();
        if (pTask == nullptr)
        {
            assert(0);
        }
        else
        {
            LogManager::getSingleton().stream(LML_TRIVIAL)
                << "LogicClient run task: "
                << pTask->getType()
                << ", "
                << pTask->getName();
            _runTask(pTask);
        }
        m_WorkingQueue.pop_front();
    }
}
//---------------------------------------------------------------------
void LogicTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push_back(task);
}