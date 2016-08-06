#ifndef __GameWsClientImpl_H__
#define __GameWsClientImpl_H__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameWsTaskLoop : public JsonWsTaskLoop
{
public:
    GameWsTaskLoop(const u2::String& type, const u2::String& name);
    virtual ~GameWsTaskLoop();

protected:
    virtual const u2::String& _getWsCloseRecvTask() override;
    virtual const u2::String& _getWsErrorRecvTask() override;
    virtual const u2::String& _getWsOpenRecvTask() override;
    virtual const u2::String& _getWsHeartBeatSendTask() override;

    virtual RecvSocketTask* _splitRecvTask(vector<u2char>::type& buffer, bool binary) override;

    virtual const u2::String& _getRecvTaskLoop() override;
};


#endif // __GameWsClientImpl_H__
