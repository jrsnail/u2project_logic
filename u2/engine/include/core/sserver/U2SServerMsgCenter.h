#ifndef __U2SServerMsgCenter_H__
#define __U2SServerMsgCenter_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2NetMsgCenter.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class SServerMsgCenter : public MsgCenter
{
public:
	SServerMsgCenter(const String& type, const String& name);
	virtual ~SServerMsgCenter();

	virtual void pushOutMsg(OutMsg* msg) override;
	virtual void pushInMsg(Msg* msg) override;
	virtual void onUpdate(float dt) override;

protected:
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2SServerMsgCenter_H__
