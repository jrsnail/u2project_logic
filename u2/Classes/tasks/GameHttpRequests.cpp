#include "GameHttpRequests.h"

#include "GameHttpResponses.h"
#include "GameUrlMaker.h"
#include "application/AppPrerequisites.h"


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

    u2uint64 ulLocalTime = Root::getSingleton().getTimer()->getMilliseconds();
    DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_NetworkTimeStart, ulLocalTime);
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
    rootJsonValue["hardid"] = "test_abc" + String(__TIME__);
    rootJsonValue["nickName"] = "test_nickname" + String(__TIME__);
//     rootJsonValue["hardid"] = StringUtil::toString(Root::getSingleton().getTimer()->getMicroseconds());
//     rootJsonValue["nickName"] = "test_nickname" + StringUtil::toString(Root::getSingleton().getTimer()->getMicroseconds());
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

    u2::LogManager::getSingleton().stream(LML_TRIVIAL) << "PlayHReq: " << m_szUrl;
}