#include "U2HttpClientImpl.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PassiveHttpTaskLoop::PassiveHttpTaskLoop(const String& type, const String& name)
    : HttpTaskLoop(type, name)
{
}
//-----------------------------------------------------------------------
PassiveHttpTaskLoop::~PassiveHttpTaskLoop()
{
}
//-----------------------------------------------------------------------
void PassiveHttpTaskLoop::_runInternal()
{
    m_bKeepRunning = true;

    for (;;)
    {
        Task* pTask = nullptr;
        {
            U2_LOCK_MUTEX(m_mtxIncomingQueue);
            pTask = m_IncomingQueue.front();
            if (pTask == nullptr)
            {
                U2_THREAD_WAIT(m_IncomingQueueSync, lck);
                continue;
            }
            else
            {
                m_IncomingQueue.pop();
            }
        }

        if (pTask != nullptr)
        {
            _runTask(pTask);
        }

        if (!m_bKeepRunning)
            break;
    }
}
//---------------------------------------------------------------------
void PassiveHttpTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push(task);
    m_IncomingQueueSync.notify_all();
}
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
    m_bKeepRunning = true;

    
    for (;;)
    {
        if (m_WorkingQueue.empty())
        {
            U2_LOCK_MUTEX(m_mtxIncomingQueue);
            bool bEmpty = m_IncomingQueue.empty();
            if (bEmpty)
            {
                U2_THREAD_SLEEP(1000);
            }
            else
            {
                while (!m_IncomingQueue.empty())
                {
                    m_WorkingQueue.push(m_IncomingQueue.front());
                    m_IncomingQueue.pop();
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
            m_WorkingQueue.pop();

            if (!m_bKeepRunning)
                break;
        }
        
        if (!m_bKeepRunning)
            break;
    }
}
//---------------------------------------------------------------------
void ActiveHttpTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push(task);
}