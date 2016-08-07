#ifndef __GameRecvSocketTasks_H__
#define __GameRecvSocketTasks_H__


#include "U2Core.h"
#include <json/json.h>


U2EG_NAMESPACE_USING


class GameWsCloseRST : public WsCloseRST
{
public:
    GameWsCloseRST(const u2::String& type, const u2::String& name);
    virtual ~GameWsCloseRST();

    virtual void run() override;
};


class GameWsErrorRST : public WsErrorRST
{
public:
    GameWsErrorRST(const u2::String& type, const u2::String& name);
    virtual ~GameWsErrorRST();

    virtual void run() override;
};


class GameWsOpenRST : public WsOpenRST
{
public:
    GameWsOpenRST(const u2::String& type, const u2::String& name);
    virtual ~GameWsOpenRST();

    virtual void run() override;
};

class SnapshotRST : public RecvSocketTask
{
public:
    struct PlayerSnapshot
    {
        PlayerSnapshot()
            : ulTimestamp(0)
            , nCurHp(0)
            , uCurSpeed(0)
            , uAtkDistance(0)
            , bAlive(true)
            , v2Velocity(cocos2d::Vec2::ZERO)
            , v2Position(cocos2d::Vec2::ZERO)
        {

        }

        u2::String      szPlayerId;
        u2::String      szGameObjGuid;
        u2::String      szPlayerName;
        u2uint64        ulTimestamp;
        u2int32         nCurHp;
        u2uint32        uCurSpeed;
        u2uint32        uAtkDistance;
        bool            bAlive;
        cocos2d::Vec2   v2Velocity;
        cocos2d::Vec2   v2Position;
    };

public:
    SnapshotRST(const String& type, const String& name);
    virtual ~SnapshotRST();

    virtual void deserialize() override;
    virtual void run() override;

private:
    bool _deserializeHero(Json::Value& jsonValue, PlayerSnapshot& playerSnapshot);

private:
    u2int32     m_nCode;
    u2::String  m_szMsg;
    u2uint64    m_ulTimestamp;
    vector<PlayerSnapshot*>::type   m_PlayerSnapshots;
};


#endif /* defined(__GameRecvSocketTasks_H__) */
