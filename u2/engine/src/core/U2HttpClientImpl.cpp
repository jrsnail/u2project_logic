#include "U2HttpClientImpl.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ActiveHttpTaskLoop::ActiveHttpTaskLoop(const String& type, const String& name)
    : HttpTaskLoop(type, name)
{
}
//-----------------------------------------------------------------------
ActiveHttpTaskLoop::~ActiveHttpTaskLoop()
{
}
//-----------------------------------------------------------------------
void ActiveHttpTaskLoop::_runInternal()
{
    for (;;)
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

        if (m_WorkingQueue.empty())
        {
            bool bEmpty = true;
            {
                U2_LOCK_MUTEX(m_mtxIncomingQueue);
                bEmpty = m_IncomingQueue.empty();
            }
            if (bEmpty)
            {
                U2_THREAD_SLEEP(1000);
            }
            else
            {
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
                _runTask(pTask);
            }
            m_WorkingQueue.pop_front();
        }
    }
}
//---------------------------------------------------------------------
void ActiveHttpTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push_back(task);
}