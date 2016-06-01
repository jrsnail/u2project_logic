#include "U2MsgPumpDefault.h"

#include "U2Exception.h"


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
void MsgPumpDefault::run(Delegate* delegate)
{
    U2Assert(m_bKeepRunning, "Quit must have been called outside of Run!");

    for (;;)
    {
        bool bDidWork = delegate->doWork();
        if (!m_bKeepRunning)
            break;

        if (bDidWork)
            continue;

    }

    m_bKeepRunning = true;
}
//---------------------------------------------------------------------
void MsgPumpDefault::quit()
{
    m_bKeepRunning = false;
}