#ifndef __GameRecvSocketTasks_H__
#define __GameRecvSocketTasks_H__


#include "U2Core.h"
#include <json/json.h>
#include "GameDataPool.h"


U2EG_NAMESPACE_USING


class GameMovableSnapshot;


class GameWsCloseRST : public WsCloseRST
{
public:
    GameWsCloseRST(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~GameWsCloseRST();

    virtual void run() override;
};


class GameWsErrorRST : public WsErrorRST
{
public:
    GameWsErrorRST(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~GameWsErrorRST();

    virtual void run() override;
};


class GameWsOpenRST : public WsOpenRST
{
public:
    GameWsOpenRST(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~GameWsOpenRST();

    virtual void run() override;
};

class SnapshotRST : public RecvSocketTask
{
public:
    SnapshotRST(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~SnapshotRST();

    virtual void deserialize() override;
    virtual void run() override;

private:
    bool _deserializeHero(Json::Value& jsonValue, GameMovableSnapshot* gameMovableSnapshot);

private:
    u2int32     m_nCode;
    u2::String  m_szMsg;
    u2::String  m_szVersion;
    SnapshotDataPool::FrameSnapshot m_FrameSnapshot;
};


#endif /* defined(__GameRecvSocketTasks_H__) */
