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


#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2WebSocketClientImpl_H__
