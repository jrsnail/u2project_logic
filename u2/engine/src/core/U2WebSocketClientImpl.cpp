#include "U2WebSocketClientImpl.h"

#include <json/json.h>
#include "U2LogManager.h"
#include "U2PredefinedPrerequisites.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
JsonWsTaskLoop::JsonWsTaskLoop(const String& type, const String& name)
    : WsTaskLoop(type, name)
{
    CREATE_FACTORY(WsCloseRST);
    CREATE_FACTORY(WsErrorRST);
    CREATE_FACTORY(WsOpenRST);
    CREATE_FACTORY(WsHeartBeatSST);
}
//-----------------------------------------------------------------------
JsonWsTaskLoop::~JsonWsTaskLoop()
{
}
//-----------------------------------------------------------------------
inline const String& JsonWsTaskLoop::_getWsCloseRecvTask()
{
    static String szType = GET_OBJECT_TYPE(WsCloseRST);
    return szType;
}
//-----------------------------------------------------------------------
inline const String& JsonWsTaskLoop::_getWsErrorRecvTask()
{
    static String szType = GET_OBJECT_TYPE(WsErrorRST);
    return szType;
}
//-----------------------------------------------------------------------
inline const String& JsonWsTaskLoop::_getWsOpenRecvTask()
{
    static String szType = GET_OBJECT_TYPE(WsOpenRST);
    return szType;
}
//-----------------------------------------------------------------------
inline const String& JsonWsTaskLoop::_getWsHeartBeatSendTask()
{
    static String szType = GET_OBJECT_TYPE(WsHeartBeatSST);
    return szType;
}
//-----------------------------------------------------------------------
inline const String& JsonWsTaskLoop::_getRecvTaskLoop()
{
    static u2::String szTaskLoop = ON_Logic_TaskLoop;
    return szTaskLoop;
}
//-----------------------------------------------------------------------
RecvSocketTask* JsonWsTaskLoop::_splitRecvTask(vector<u2char>::type& buffer, bool binary)
{
    std::string szJson(buffer.begin(), buffer.end());

    Json::Reader reader;
    Json::Value rootJsonVal;
    if (reader.parse(szJson, rootJsonVal))
    {
        std::string szMsgId = rootJsonVal["TaskId"].asString();

        RecvSocketTask* pTask = dynamic_cast<RecvSocketTask*>(
            TaskManager::getSingleton().createObject(szMsgId));
        pTask->setBinary(binary);
        pTask->setData(buffer);
        return pTask;
    }
    else
    {
        LogManager::getSingleton().stream(LML_CRITICAL) 
            << "Damaged recv task: " 
            << szJson;
        return nullptr;
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
WsCloseRST::WsCloseRST(const String& type, const String& name)
    : RecvSocketTask(type, name)
{

}
//-----------------------------------------------------------------------
WsCloseRST::~WsCloseRST()
{

}
//-----------------------------------------------------------------------
void WsCloseRST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
WsErrorRST::WsErrorRST(const String& type, const String& name)
    : RecvSocketTask(type, name)
{

}
//-----------------------------------------------------------------------
WsErrorRST::~WsErrorRST()
{

}
//-----------------------------------------------------------------------
void WsErrorRST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
WsOpenRST::WsOpenRST(const String& type, const String& name)
    : RecvSocketTask(type, name)
{

}
//-----------------------------------------------------------------------
WsOpenRST::~WsOpenRST()
{

}
//-----------------------------------------------------------------------
void WsOpenRST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
WsHeartBeatSST::WsHeartBeatSST(const String& type, const String& name)
    : SendSocketTask(type, name)
{

}
//-----------------------------------------------------------------------
WsHeartBeatSST::~WsHeartBeatSST()
{

}
//-----------------------------------------------------------------------
void WsHeartBeatSST::run()
{

}