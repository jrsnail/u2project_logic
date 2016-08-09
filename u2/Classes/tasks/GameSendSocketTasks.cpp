#include "GameSendSocketTasks.h"



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
inline void MoveSST::setAttackedPlayerId(const String& id)
{
    m_szAttackedPlayerId = id;
}
//-----------------------------------------------------------------------
void MoveSST::serialize()
{
    Json::Value rootJsonValue;
    String szSelfRoomId;
    bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfRoomId", szSelfRoomId);
    String szSelfPlayerGuid;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfPlayerGuid", szSelfPlayerGuid);
    rootJsonValue["roomId"] = szSelfRoomId;
    rootJsonValue["userId"] = szSelfPlayerGuid;
    rootJsonValue["attUserId"] = StringUtil::parseUnsignedInt64(m_szAttackedPlayerId);
    rootJsonValue["x"] = m_v2Position.x;
    rootJsonValue["y"] = m_v2Position.y;
    rootJsonValue["vx"] = m_v2Velocity.x;
    rootJsonValue["vy"] = m_v2Velocity.y;
    rootJsonValue["timestamp"] = m_ulTimestamp;
    String szJsonStr = rootJsonValue.toStyledString();

    setData(vector<u2char>::type(szJsonStr.begin(), szJsonStr.end()));
}