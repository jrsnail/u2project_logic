#include "U2SocketTask.h"

#include "U2LogManager.h"
#include "U2Exception.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SendSocketTask::SendSocketTask(const String& type, const String& name)
    : Task(type, name)
    , m_bIsBinary(false)
    , m_uIssued(0)
{

}
//-----------------------------------------------------------------------
SendSocketTask::~SendSocketTask()
{

}
//-----------------------------------------------------------------------
void SendSocketTask::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
RecvSocketTask::RecvSocketTask(const String& type, const String& name)
    : Task(type, name)
    , m_bIsBinary(false)
{

}
//-----------------------------------------------------------------------
RecvSocketTask::~RecvSocketTask()
{

}
//-----------------------------------------------------------------------
void RecvSocketTask::run()
{

}
//-----------------------------------------------------------------------
void RecvSocketTask::setBinary(bool b)
{
    m_bIsBinary = b;
}
//-----------------------------------------------------------------------
bool RecvSocketTask::isBinary() const
{
    return m_bIsBinary;
}
//-----------------------------------------------------------------------
void RecvSocketTask::setData(const vector<u2char>::type& data)
{
    m_Data = data;
}