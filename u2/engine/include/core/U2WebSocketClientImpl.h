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
    JsonWsTaskLoop(const String& type, const String& name);
    virtual ~JsonWsTaskLoop();

protected:
    virtual RecvSocketTask* _dispatchRecvTask(vector<u2char>::type& buffer, bool binary) override;

};


class WsCloseRST : public RecvSocketTask
{
public:
    WsCloseRST(const String& type, const String& name);
    virtual ~WsCloseRST();

    virtual void run() override;
};


class WsErrorRST : public RecvSocketTask
{
public:
    WsErrorRST(const String& type, const String& name);
    virtual ~WsErrorRST();

    virtual void run() override;
};


class WsOpenRST : public RecvSocketTask
{
public:
    WsOpenRST(const String& type, const String& name);
    virtual ~WsOpenRST();

    virtual void run() override;
};


class WsHeartBeatSST : public SendSocketTask
{
public:
    WsHeartBeatSST(const String& type, const String& name);
    virtual ~WsHeartBeatSST();

    virtual void run() override;
};


#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2WebSocketClientImpl_H__
