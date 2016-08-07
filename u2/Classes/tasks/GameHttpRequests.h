#ifndef __GameHttpRequests_H__
#define __GameHttpRequests_H__


#include "U2Core.h"
#include <json/json.h>


U2EG_NAMESPACE_USING


class TimeHReq : public HttpRequest
{
public:
    TimeHReq(const String& type, const String& name);
    virtual ~TimeHReq();

    virtual const String& getHttpResponse() override;

    virtual void serialize() override;

private:
};


#endif /* defined(__GameHttpRequests_H__) */
