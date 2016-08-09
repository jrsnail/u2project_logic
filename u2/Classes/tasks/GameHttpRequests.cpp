#include "GameHttpRequests.h"

#include "GameHttpResponses.h"
#include "GameUrlMaker.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TimeHReq::TimeHReq(const u2::String& type, const u2::String& name, const u2::String& guid)
    : HttpRequest(type, name, guid)
{
    setHttpType(HttpRequest::Type::HTTP_POST);
}
//-----------------------------------------------------------------------
TimeHReq::~TimeHReq()
{
}
//-----------------------------------------------------------------------
const u2::String& TimeHReq::getHttpResponse()
{
    static u2::String szRspType = GET_OBJECT_TYPE(TimeHRsp);
    return szRspType;
}
//-----------------------------------------------------------------------
void TimeHReq::serialize()
{
    GameUrlMaker urlMaker;
    urlMaker.setPath("watch.jsp");
    setUrl(urlMaker.generate());
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
RegisterHReq::RegisterHReq(const u2::String& type, const u2::String& name, const u2::String& guid)
    : HttpRequest(type, name, guid)
{
    setHttpType(HttpRequest::Type::HTTP_POST);
}
//-----------------------------------------------------------------------
RegisterHReq::~RegisterHReq()
{
}
//-----------------------------------------------------------------------
const u2::String& RegisterHReq::getHttpResponse()
{
    static u2::String szRspType = GET_OBJECT_TYPE(RegisterHRsp);
    return szRspType;
}
//-----------------------------------------------------------------------
void RegisterHReq::serialize()
{
    Json::Value rootJsonValue;
    rootJsonValue["hardid"] = "test_abc";
    rootJsonValue["nickName"] = "test_nickname";
    rootJsonValue["icon"] = 1;
    String szContentJsonStr = rootJsonValue.toStyledString();

    GameUrlMaker urlMaker;
    urlMaker.setPath("plane/operation");
    urlMaker.addParam("content", szContentJsonStr);
    urlMaker.addParam("key", "10");
    urlMaker.addParam("protocolVer", "1");
    urlMaker.addParam("checkout", "");
    setUrl(urlMaker.generate());
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PlayHReq::PlayHReq(const u2::String& type, const u2::String& name, const u2::String& guid)
    : HttpRequest(type, name, guid)
{
    setHttpType(HttpRequest::Type::HTTP_POST);
}
//-----------------------------------------------------------------------
PlayHReq::~PlayHReq()
{
}
//-----------------------------------------------------------------------
const u2::String& PlayHReq::getHttpResponse()
{
    static u2::String szRspType = GET_OBJECT_TYPE(PlayHRsp);
    return szRspType;
}
//-----------------------------------------------------------------------
void PlayHReq::serialize()
{
    Json::Value rootJsonValue;
    String szSelfPlayerGuid;
    bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfPlayerGuid", szSelfPlayerGuid);
    rootJsonValue["userId"] = szSelfPlayerGuid;
    String szContentJsonStr = rootJsonValue.toStyledString();

    GameUrlMaker urlMaker;
    urlMaker.setPath("plane/operation");
    urlMaker.addParam("content", szContentJsonStr);
    urlMaker.addParam("key", "12");
    urlMaker.addParam("protocolVer", "1");
    urlMaker.addParam("user", szSelfPlayerGuid);
    urlMaker.addParam("checkout", "");
    setUrl(urlMaker.generate());
}