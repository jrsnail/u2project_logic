#include "GameWsClientImpl.h"

#include <json/json.h>
#include "GameRecvSocketTasks.h"
#include "GameSendSocketTasks.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsTaskLoop::GameWsTaskLoop(const u2::String& type, const u2::String& name, const u2::String& guid)
    : JsonWsTaskLoop(type, name, guid)
{
    CREATE_FACTORY(GameWsCloseRST);
    CREATE_FACTORY(GameWsErrorRST);
    CREATE_FACTORY(GameWsOpenRST);
    CREATE_FACTORY(GameWsHeartBeatSST);
}
//-----------------------------------------------------------------------
GameWsTaskLoop::~GameWsTaskLoop()
{
}
//-----------------------------------------------------------------------
inline const u2::String& GameWsTaskLoop::_getWsCloseRecvTask()
{
    static u2::String szType = GET_OBJECT_TYPE(GameWsCloseRST);
    return szType;
}
//-----------------------------------------------------------------------
inline const u2::String& GameWsTaskLoop::_getWsErrorRecvTask()
{
    static u2::String szType = GET_OBJECT_TYPE(GameWsErrorRST);
    return szType;
}
//-----------------------------------------------------------------------
inline const u2::String& GameWsTaskLoop::_getWsOpenRecvTask()
{
    static u2::String szType = GET_OBJECT_TYPE(GameWsOpenRST);
    return szType;
}
//-----------------------------------------------------------------------
inline const u2::String& GameWsTaskLoop::_getWsHeartBeatSendTask()
{
    static u2::String szType = GET_OBJECT_TYPE(GameWsHeartBeatSST);
    return szType;
}
//-----------------------------------------------------------------------
inline const u2::String& GameWsTaskLoop::_getRecvTaskLoop()
{
    static u2::String szTaskLoop = ON_Logic_TaskLoop;
    return szTaskLoop;
}
//-----------------------------------------------------------------------
RecvSocketTask* GameWsTaskLoop::_splitRecvTask(vector<u2char>::type& buffer, bool binary)
{
    std::string szJson(buffer.begin(), buffer.end());

    Json::Reader reader;
    Json::Value rootJsonVal;
    if (reader.parse(szJson, rootJsonVal))
    {
        std::string szMsgId = rootJsonVal["taskId"].asString();

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