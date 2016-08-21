#ifndef __GameHttpResponses_H__
#define __GameHttpResponses_H__


#include "U2Core.h"
#include <json/json.h>


U2EG_NAMESPACE_USING


class GameMovableSnapshot;


class TimeHRsp : public HttpResponse
{
public:
    TimeHRsp(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~TimeHRsp();

    virtual void deserialize() override;

    virtual void run() override;

private:
    u2uint64 m_ulServerBaseTime;
};


class RegisterHRsp : public HttpResponse
{
public:
    RegisterHRsp(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~RegisterHRsp();

    virtual void deserialize() override;

    virtual void run() override;

private:
    bool m_bDeserializeSucceed;
    u2int32     m_nCode;
    u2::String  m_szMsg;
    u2uint64    m_ulSelfPlayerGuid;
};


class PlayHRsp : public HttpResponse
{
public:
    PlayHRsp(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~PlayHRsp();

    virtual void deserialize() override;

    virtual void run() override;

private:
    bool _deserializeHero(Json::Value& jsonValue, GameMovableSnapshot* gameMovableSnapshot);

private:
    bool m_bDeserializeSucceed;
    u2int32     m_nCode;
    u2::String  m_szMsg;
    u2uint64    m_ulTimestamp;
    SnapshotDataPool::FrameSnapshot m_FrameSnapshot;
    u2::String  m_szRoomGuid;
    u2int32     m_nRoomState;
    u2uint64    m_ulCreateRoomTimestamp;
    u2uint64    m_ulStartRoomTimestamp;
};


#endif /* defined(__GameHttpResponses_H__) */
