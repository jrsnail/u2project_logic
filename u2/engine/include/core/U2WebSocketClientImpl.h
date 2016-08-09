#ifndef __U2WebSocketClientImpl_H__
#define __U2WebSocketClientImpl_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2WebSocketClient.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class _U2Export JsonWsTaskLoop : public WsTaskLoop
{
public:
    JsonWsTaskLoop(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~JsonWsTaskLoop();

protected:
    virtual const String& _getWsCloseRecvTask() override;
    virtual const String& _getWsErrorRecvTask() override;
    virtual const String& _getWsOpenRecvTask() override;
    virtual const String& _getWsHeartBeatSendTask() override;

    virtual RecvSocketTask* _splitRecvTask(vector<u2char>::type& buffer, bool binary) override;

    virtual const String& _getRecvTaskLoop() override;

};


class WsCloseRST : public RecvSocketTask
{
public:
    WsCloseRST(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~WsCloseRST();

    virtual void run() override;
};


class WsErrorRST : public RecvSocketTask
{
public:
    WsErrorRST(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~WsErrorRST();

    virtual void run() override;
};


class WsOpenRST : public RecvSocketTask
{
public:
    WsOpenRST(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~WsOpenRST();

    virtual void run() override;
};


class WsHeartBeatSST : public SendSocketTask
{
public:
    WsHeartBeatSST(const String& type, const String& name, const u2::String& guid = BLANK);
    virtual ~WsHeartBeatSST();

    virtual void run() override;
};


#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2WebSocketClientImpl_H__
