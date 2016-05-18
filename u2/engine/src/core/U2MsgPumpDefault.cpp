#include "U2MsgPumpDefault.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
MsgPumpDefault::MsgPumpDefault() 
    : m_bKeepRunning(true)
{
}
//---------------------------------------------------------------------
MsgPumpDefault::~MsgPumpDefault()
{

}
//---------------------------------------------------------------------
void MsgPumpDefault::Run(Delegate* delegate)
{
    DCHECK(m_bKeepRunning) << "Quit must have been called outside of Run!";

    for (;;)
    {
#if defined(OS_MACOSX)
        mac::ScopedNSAutoreleasePool autorelease_pool;
#endif

        bool bDidWork = delegate->DoWork();
        if (!m_bKeepRunning)
            break;

        bDidWork |= delegate->DoDelayedWork(&m_DelayedWorkTime);
        if (!m_bKeepRunning)
            break;

        if (bDidWork)
            continue;

        bDidWork = delegate->DoIdleWork();
        if (!m_bKeepRunning)
            break;

        if (bDidWork)
            continue;

        if (m_DelayedWorkTime.is_null())
        {
            event_.Wait();
        }
        else 
        {
            TimeDelta delay = m_DelayedWorkTime - TimeTicks::Now();
            if (delay > TimeDelta())
            {
                event_.TimedWait(delay);
            }
            else
            {
                // It looks like delayed_work_time_ indicates a time in the past, so we
                // need to call DoDelayedWork now.
                m_DelayedWorkTime = TimeTicks();
            }
        }
        // Since event_ is auto-reset, we don't need to do anything special here
        // other than service each delegate method.
    }

    m_bKeepRunning = true;
}
//---------------------------------------------------------------------
void MsgPumpDefault::Quit()
{
    m_bKeepRunning = false;
}
//---------------------------------------------------------------------
void MsgPumpDefault::ScheduleWork()
{
    // Since this can be called on any thread, we need to ensure that our Run
    // loop wakes up.
    event_.Signal();
}
//---------------------------------------------------------------------
void MsgPumpDefault::ScheduleDelayedWork(const TimeTicks& delayed_work_time)
{
    // We know that we can't be blocked on Wait right now since this method can
    // only be called on the same thread as Run, so we only need to update our
    // record of how long to sleep when we do sleep.
    m_DelayedWorkTime = delayed_work_time;
}