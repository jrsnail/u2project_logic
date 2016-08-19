#include "U2HttpClientImpl.h"

//#include "U2PredefinedPrerequisites.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ActiveHttpTaskLoop::ActiveHttpTaskLoop(const String& type, const String& name, const u2::String& guid)
    : HttpTaskLoop(type, name, guid)
{
}
//-----------------------------------------------------------------------
ActiveHttpTaskLoop::~ActiveHttpTaskLoop()
{
}
//-----------------------------------------------------------------------
void ActiveHttpTaskLoop::_runInternal()
{
    m_threadId = std::this_thread::get_id();
    
    _postRunCurrentTaskLoop();

    for (;;)
    {
        {
            U2_LOCK_MUTEX(m_KeepRunningMutex);
            if (!m_bKeepRunning)
            {
                break;
            }
        }

        if (m_WorkingQueue.empty())
        {
            bool bEmpty = true;
            {
                U2_LOCK_MUTEX(m_mtxIncomingQueue);
                bEmpty = m_IncomingQueue.empty();
            }
            if (bEmpty)
            {
                U2_THREAD_SLEEP(1);
            }
            else
            {
                U2_LOCK_MUTEX(m_mtxIncomingQueue);
                while (!m_IncomingQueue.empty())
                {
                    m_WorkingQueue.push_back(m_IncomingQueue.front());
                    m_IncomingQueue.pop_front();
                }
            }
        }

        while (!m_WorkingQueue.empty())
        {
            {
                U2_LOCK_MUTEX(m_KeepRunningMutex);
                if (!m_bKeepRunning)
                    break;
            }

            Task* pTask = m_WorkingQueue.front();
            if (pTask == nullptr)
            {
                assert(0);
            }
            else
            {
                processTask(dynamic_cast<HttpRequest*>(pTask), m_ResponseMessage);
            }
            m_WorkingQueue.pop_front();
            TaskManager::getSingleton().destoryObject(pTask);
        }
    }

    _postQuitCurrentTaskLoop();
}
//---------------------------------------------------------------------
void ActiveHttpTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push_back(task);
}
//---------------------------------------------------------------------
inline const String& ActiveHttpTaskLoop::_getRecvTaskLoop()
{
    static u2::String szTaskLoop = "ON_Logic_TaskLoop";
    return szTaskLoop;
}