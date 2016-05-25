#ifndef __U2MsgPumpDefault_H__
#define __U2MsgPumpDefault_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MsgPump.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class _U2Export MsgPumpDefault : public MsgPump
{
public:
    MsgPumpDefault();
    virtual ~MsgPumpDefault();

    virtual void Run(Delegate* delegate) override;
    virtual void Quit() override;

protected:
    // This flag is set to false when Run should return.
    bool m_bKeepRunning;

    DISALLOW_COPY_AND_ASSIGN(MsgPumpDefault);
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2MsgPumpDefault_H__
