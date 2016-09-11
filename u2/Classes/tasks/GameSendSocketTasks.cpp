#include "GameSendSocketTasks.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>  
#include <rapidjson/writer.h>
#include "ecs/GameComponents.h"
#include "application/AppPrerequisites.h"
#include "pb/SnapshotReq.pb.h"



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
    if (m_bSerializeSucceed)
    {
        return;
    }

    setBinary(true);

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
    u2uint64 ulServerTimeElapseEnterRoom = 0;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryUint64Data(ON_ServerTimeElapse_EnterRoom, ulServerTimeElapseEnterRoom);
    u2uint64 ulLocalTimeEnterRoom = 0;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryUint64Data(ON_LocaleTime_EnterRoom, ulLocalTimeEnterRoom);
    u2uint64 ulTripLantency = 0;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryUint64Data(ON_TripLatency, ulTripLantency);
    m_ulTimestamp 
        = (Root::getSingleton().getTimer()->getMilliseconds() - ulLocalTimeEnterRoom) 
        + ulServerTimeElapseEnterRoom;


    
    // create json data
    rapidjson::Document document;
    document.SetObject();
    document.AddMember("roomId"
        , rapidjson::Value(szSelfRoomId.c_str(), document.GetAllocator()).Move()
        , document.GetAllocator());
    document.AddMember("entityId"
        , rapidjson::Value(szSelfGameObjGuid.c_str(), document.GetAllocator()).Move()
        , document.GetAllocator());
    document.AddMember("attEntityId", 0, document.GetAllocator());
    document.AddMember("accSpeed", 0, document.GetAllocator());
    document.AddMember("x", m_v2Position.x, document.GetAllocator());
    document.AddMember("y", m_v2Position.y, document.GetAllocator());
    document.AddMember("vx", m_v2Velocity.x, document.GetAllocator());
    document.AddMember("vy", m_v2Velocity.y, document.GetAllocator());
    document.AddMember("timestamp", m_ulTimestamp, document.GetAllocator());
    document.AddMember("taskId", "plane", document.GetAllocator());
    document.AddMember("version", "1.0.0", document.GetAllocator());
    document.AddMember("skillId", 0, document.GetAllocator());
    document.AddMember("skillUsed", 0, document.GetAllocator());
    document.AddMember("skillMoveX", 0, document.GetAllocator());
    document.AddMember("skillMoveY", 0, document.GetAllocator());
    document.AddMember("skillTime", 0, document.GetAllocator());
    document.AddMember("skillErp", 0, document.GetAllocator());
    document.AddMember("skillRefHp", 0, document.GetAllocator());
    document.AddMember("eatObjId", 0, document.GetAllocator());
    document.AddMember("eatObjType", 0, document.GetAllocator());
    document.AddMember("hpMax", 0, document.GetAllocator());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer< rapidjson::StringBuffer > writer(buffer);
    document.Accept(writer);
    String szJsonStr = buffer.GetString();
    LogManager::getSingleton().stream(LML_TRIVIAL) << "MoveSST: " << szJsonStr;

    setData(vector<u2char>::type(szJsonStr.begin(), szJsonStr.end()));

    m_bSerializeSucceed = true;
    
    
    /*
    SnapshotReq snapshotReq;
    snapshotReq.set_roomid(StringUtil::parseUnsignedInt(szSelfRoomId));
    snapshotReq.set_heroid(StringUtil::parseUnsignedInt(szSelfGameObjGuid));
    snapshotReq.set_attheroid(0);
    snapshotReq.set_accspeed(0.0f);
    snapshotReq.set_x(m_v2Position.x);
    snapshotReq.set_y(m_v2Position.y);
    snapshotReq.set_vx(m_v2Velocity.x);
    snapshotReq.set_vy(m_v2Velocity.y);
    snapshotReq.set_timestamp(m_ulTimestamp);
    snapshotReq.set_taskid("plane");
    snapshotReq.set_version("1.0.0");
    String szProtoBufStr = snapshotReq.SerializeAsString();
    LogManager::getSingleton().stream(LML_TRIVIAL) << "MoveSST: " << szProtoBufStr;

    vector<u2char>::type v;
    OutStreamQueue<DataFilterOutStream> out;
    out.push<VariableMemOutStream>("aaa", &v);
    out.push<DataFilterOutStream>("bbb");
    out->writeInt32(1);     // taskId
    out->close();

    setData(vector<u2char>::type(szProtoBufStr.begin(), szProtoBufStr.end()));

    m_bSerializeSucceed = true;
    */
}