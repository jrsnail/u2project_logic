#include "U2LogicClient.h"

#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2FrameListenerCollection.h"
#include "U2Root.h"
#include "U2System.h"
#include "U2Timer.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LogicTaskLoop::LogicTaskLoop(const String& type, const String& name, const String& guid)
    : TaskLoop(type, name, guid)
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
    FrameListenerCollection::getSingleton().addFrameListener(this
        , std::bind(&LogicTaskLoop::_onUpdate, this, std::placeholders::_1));
    
    _postRunCurrentTaskLoop();
}
//-----------------------------------------------------------------------
void LogicTaskLoop::quit()
{
}
//-----------------------------------------------------------------------
void LogicTaskLoop::pause()
{
}
//-----------------------------------------------------------------------
void LogicTaskLoop::resume()
{
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
    }

    while (!m_WorkingQueue.empty())
    {
        Task* pTask = m_WorkingQueue.front();
        if (pTask == nullptr)
        {
            assert(0);
        }
        else
        {
            _runTask(pTask);
        }
        m_WorkingQueue.pop_front();
    }

    // execute systems
    u2uint64 ulStart = Root::getSingleton().getTimer()->getMilliseconds();
    SystemManager::getSingleton().execute(dt);
    u2uint64 ulDelta = Root::getSingleton().getTimer()->getMilliseconds() - ulStart;
    LogManager::getSingleton().stream(LML_TRIVIAL) << "SystemManager : " << ulDelta;
}
//---------------------------------------------------------------------
void LogicTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push_back(task);
}