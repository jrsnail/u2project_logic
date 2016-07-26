#include "U2WebSocketClientImpl.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
JsonWsTaskLoop::JsonWsTaskLoop(const String& type, const String& name)
    : WsTaskLoop(type, name)
{
}
//-----------------------------------------------------------------------
JsonWsTaskLoop::~JsonWsTaskLoop()
{
}
//-----------------------------------------------------------------------
RecvSocketTask* JsonWsTaskLoop::_dispatchRecvTask(vector<u2char>::type& buffer, bool binary)
{
    return nullptr;
}