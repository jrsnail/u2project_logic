#ifndef __GameHttpResponses_H__
#define __GameHttpResponses_H__


#include "U2Core.h"
#include <json/json.h>


U2EG_NAMESPACE_USING


class TimeHRsp : public HttpResponse
{
public:
    TimeHRsp(const String& type, const String& name);
    virtual ~TimeHRsp();

    virtual void deserialize() override;

    virtual void run() override;

private:
    u2uint64 m_ulServerBaseTime;
};


#endif /* defined(__GameHttpResponses_H__) */
