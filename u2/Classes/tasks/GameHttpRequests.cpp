#include "GameHttpRequests.h"

#include <rapidjson/stringbuffer.h>  
#include <rapidjson/writer.h>
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
    rapidjson::Document document;
    document.SetObject();

//     document.AddMember("hardid"
//         , rapidjson::Value("test_abcCCC", document.GetAllocator()).Move()
//         , document.GetAllocator());
//     document.AddMember("nickName"
//         , rapidjson::Value("test_nicknameCCC", document.GetAllocator()).Move()
//         , document.GetAllocator());

    String szHardId = "test_abc" + StringUtil::toString(u2::Root::getSingleton().getTimer()->getMicroseconds());
    String szNickname = "test_nickname" +StringUtil::toString(u2::Root::getSingleton().getTimer()->getMicroseconds());
    document.AddMember("hardid"
        , rapidjson::Value(szHardId.c_str(), document.GetAllocator()).Move()
        , document.GetAllocator());
    document.AddMember("nickName"
        , rapidjson::Value(szNickname.c_str(), document.GetAllocator()).Move()
        , document.GetAllocator());
    document.AddMember("icon", 1, document.GetAllocator());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer< rapidjson::StringBuffer > writer(buffer);
    document.Accept(writer);
    String szContentJsonStr = buffer.GetString();
    LogManager::getSingleton().stream(LML_TRIVIAL) << "RegisterHReq: " << szContentJsonStr;

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
    rapidjson::Document document;
    document.SetObject();

    String szSelfPlayerGuid;
    bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfPlayerGuid", szSelfPlayerGuid);
    document.AddMember("userId"
        , rapidjson::Value(szSelfPlayerGuid.c_str(), document.GetAllocator()).Move()
        , document.GetAllocator());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer< rapidjson::StringBuffer > writer(buffer);
    document.Accept(writer);
    String szContentJsonStr = buffer.GetString();
    LogManager::getSingleton().stream(LML_TRIVIAL) << "PlayHReq: " << szContentJsonStr;


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