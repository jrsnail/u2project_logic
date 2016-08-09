#ifndef __U2GameUrlMaker_H__
#define __U2GameUrlMaker_H__

#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameUrlMaker : public UrlMaker
{
public:
    GameUrlMaker();
    virtual ~GameUrlMaker();

    virtual void setProtocol(const String& protocol) override;
    virtual void setAddress(const String& address) override;
    virtual void setPort(u2uint32 port) override;

protected:
};


#endif // __U2GameUrlMaker_H__
