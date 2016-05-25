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
        bool bDidWork = delegate->DoWork();
        if (!m_bKeepRunning)
            break;

        if (bDidWork)
            continue;

    }

    m_bKeepRunning = true;
}
//---------------------------------------------------------------------
void MsgPumpDefault::Quit()
{
    m_bKeepRunning = false;
}