#include "GameSendSocketTasks.h"

#include "ecs/GameComponents.h"
#include "application/AppPrerequisites.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsHeartBeatSST::GameWsHeartBeatSST(const u2::String& type, const u2::String& name, const u2::String& guid)
    : WsHeartBeatSST(type, name, guid)
{

}
//-----------------------------------------------------------------------
GameWsHeartBeatSST::~GameWsHeartBeatSST()
{

}
//-----------------------------------------------------------------------
void GameWsHeartBeatSST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
MoveSST::MoveSST(const String& type, const String& name, const u2::String& guid)
    : SendSocketTask(type, name, guid)
    , m_v2Velocity(cocos2d::Vec2::ZERO)
    , m_v2Position(cocos2d::Vec2::ZERO)
    , m_ulTimestamp(0L)
{
}
//-----------------------------------------------------------------------
MoveSST::~MoveSST()
{
}
//-----------------------------------------------------------------------
inline void MoveSST::setTimestamp(u2uint64 timestamp)
{
    m_ulTimestamp = timestamp;
}
//-----------------------------------------------------------------------
inline void MoveSST::setSelfPosition(const cocos2d::Vec2& pos)
{
    m_v2Position = pos;
}
//-----------------------------------------------------------------------
inline void MoveSST::setSelfVelocity(const cocos2d::Vec2& v)
{
    m_v2Velocity = v;
}
//-----------------------------------------------------------------------
inline void MoveSST::setAttackedGameObjGuid(const String& id)
{
    m_szAttackedGameObjGuid = id;
}
//-----------------------------------------------------------------------
void MoveSST::serialize()
{
    // room id
    String szSelfRoomId;
    bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfRoomId", szSelfRoomId);
    
    // self game object guid
    String szSelfGameObjGuid;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfGameObjGuid", szSelfGameObjGuid);

    // self game object
    GameObject* pSelfGameObj = GameObjectManager::getSingleton().retrieveObjectByGuid(szSelfGameObjGuid);
    if (pSelfGameObj == nullptr)
    {
        assert(0);
    }

    // position
    PositionComponent* pPositionComp 
        = dynamic_cast<PositionComponent*>(pSelfGameObj->retrieveComponentByType("component_position"));
    if (pPositionComp == nullptr)
    {
        assert(0);
    }
    else
    {
        m_v2Position = pPositionComp->v2Pos;
    }

    // velocity
    VelocityComponent* pVelocityComp
        = dynamic_cast<VelocityComponent*>(pSelfGameObj->retrieveComponentByType("component_velocity"));
    if (pVelocityComp == nullptr)
    {
        assert(0);
    }
    else
    {
        m_v2Velocity = pVelocityComp->v2Velocity;
    }

    // timestamp
    u2uint64 ulServerStartRoomTime = 0;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryUint64Data(ON_ServerStartRoomTime, ulServerStartRoomTime);
    m_ulTimestamp = ulServerStartRoomTime + Root::getSingleton().getTimer()->getMilliseconds();

    // create json data
    Json::Value rootJsonValue;
    rootJsonValue["roomId"] = szSelfRoomId;
    rootJsonValue["heroId"] = szSelfGameObjGuid;
    rootJsonValue["attHeroId"] = StringUtil::parseUnsignedInt64(m_szAttackedGameObjGuid);
    rootJsonValue["x"] = m_v2Position.x;
    rootJsonValue["y"] = m_v2Position.y;
    rootJsonValue["vx"] = m_v2Velocity.x;
    rootJsonValue["vy"] = m_v2Velocity.y;
    rootJsonValue["timestamp"] = m_ulTimestamp;
    rootJsonValue["taskId"] = "plane";
    rootJsonValue["version"] = "1.0.0";
    String szJsonStr = rootJsonValue.toStyledString();

    setData(vector<u2char>::type(szJsonStr.begin(), szJsonStr.end()));
}