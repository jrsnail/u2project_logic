#include "GameHttpRequests.h"

#include "GameHttpResponses.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TimeHReq::TimeHReq(const String& type, const String& name)
    : HttpRequest(type, name)
{
    setHttpType(HttpRequest::Type::HTTP_POST);
    setUrl("http://10.60.81.51:8080/watch.jsp");
}
//-----------------------------------------------------------------------
TimeHReq::~TimeHReq()
{
}
//-----------------------------------------------------------------------
const String& TimeHReq::getHttpResponse()
{
    static String szRspType = GET_OBJECT_TYPE(TimeHRsp);
    return szRspType;
}
//-----------------------------------------------------------------------
void TimeHReq::serialize()
{
}