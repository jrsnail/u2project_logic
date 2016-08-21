#ifndef __GameSendSocketTasks_H__
#define __GameSendSocketTasks_H__


#include "U2Core.h"
#include <json/json.h>


U2EG_NAMESPACE_USING


class GameWsHeartBeatSST : public WsHeartBeatSST
{
public:
    GameWsHeartBeatSST(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~GameWsHeartBeatSST();

    virtual void run() override;
};


class MoveSST : public SendSocketTask
{
public:
    MoveSST(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~MoveSST();

    virtual void serialize() override;

    void setTimestamp(u2uint64 timestamp);
    void setSelfPosition(const cocos2d::Vec2& pos);
    void setSelfVelocity(const cocos2d::Vec2& v);
    void setAttackedGameObjGuid(const u2::String& id);

private:
    cocos2d::Vec2   m_v2Velocity;
    cocos2d::Vec2   m_v2Position;
    u2uint64        m_ulTimestamp;
    u2::String      m_szAttackedGameObjGuid;
};


#endif /* defined(__GameSendSocketTasks_H__) */
