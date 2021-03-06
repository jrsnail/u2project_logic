﻿#include "U2WebSocketClientImpl.h"

//#include <rapidjson/document.h>
#include "U2LogManager.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
JsonWsTaskLoop::JsonWsTaskLoop(const String& type, const String& name, const u2::String& guid)
    : WsTaskLoop(type, name, guid)
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
    static u2::String szTaskLoop = "ON_Logic_TaskLoop";
    return szTaskLoop;
}
//-----------------------------------------------------------------------
RecvSocketTask* JsonWsTaskLoop::_splitRecvTask(vector<u2char>::type& buffer, bool binary)
{
    std::string szJson(buffer.begin(), buffer.end());

    /*
    rapidjson::Document document;
    document.Parse(szJson.c_str());

    do
    {
        if (!document.IsObject())
        {
            break;
        }

        if (!document.HasMember("TaskId"))
        {
            break;
        }
        std::string szMsgId = document["TaskId"].GetString();

        RecvSocketTask* pTask = dynamic_cast<RecvSocketTask*>(
            TaskManager::getSingleton().createObject(szMsgId));
        pTask->setBinary(binary);
        pTask->setData(buffer);
        return pTask;

    } while (0);
    */

    LogManager::getSingleton().stream(LML_CRITICAL)
        << "Damaged recv task: "
        << szJson;
    return nullptr;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
WsCloseRST::WsCloseRST(const String& type, const String& name, const u2::String& guid)
    : RecvSocketTask(type, name, guid)
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
WsErrorRST::WsErrorRST(const String& type, const String& name, const u2::String& guid)
    : RecvSocketTask(type, name, guid)
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
WsOpenRST::WsOpenRST(const String& type, const String& name, const u2::String& guid)
    : RecvSocketTask(type, name, guid)
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
WsHeartBeatSST::WsHeartBeatSST(const String& type, const String& name, const u2::String& guid)
    : SendSocketTask(type, name, guid)
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