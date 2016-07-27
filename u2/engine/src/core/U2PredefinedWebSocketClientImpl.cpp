#include "U2PredefinedWebSocketClientImpl.h"

#include <json/json.h>
#include "U2LogManager.h"



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