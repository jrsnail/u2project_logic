#include "GameHttpResponses.h"

#include <rapidjson/error/en.h>
#include "application/AppPrerequisites.h"
#include "GameHttpRequests.h"
#include "ecs/GameComponents.h"
#include "ecs/GameSnapshot.h"
#include "tasks/GameWsClientImpl.h"



#define CHECK_RAPIDJSON_MEMBER(JsonValue, member)                       \
    if (!JsonValue.HasMember(member))                                   \
    {                                                                   \
        assert(0);                                                      \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: "                                         \
            << "Parse failed, as no member "                            \
            << member                                                   \
            << ". [ " << __FILE__                                       \
            << ", " << __FUNCTION__                                     \
            << ", " << __LINE__ << " ]";                                \
        break;                                                          \
    }

#define CHECK_RAPIDJSON_VALIDITY(result)                                \
    if (!result)                                                        \
    {                                                                   \
        assert(0);                                                      \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: "                                         \
            << "Parse failed, invalid json object. "                    \
            << "[ " << __FILE__                                         \
            << ", " << __FUNCTION__                                     \
            << ", " << __LINE__                                         \
            << " ]";                                                    \
        break;                                                          \
    }


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TimeHRsp::TimeHRsp(const u2::String& type, const u2::String& name, const u2::String& guid)
    : HttpResponse(type, name, guid)
    , m_ulServerBaseTime(0L)
{
}
//-----------------------------------------------------------------------
TimeHRsp::~TimeHRsp()
{
}
//-----------------------------------------------------------------------
void TimeHRsp::deserialize()
{
    const vector<u2char>::type& v = getData();
    m_ulServerBaseTime = StringUtil::parseUnsignedInt64(String(v.begin(), v.end()));
}
//-----------------------------------------------------------------------
void TimeHRsp::run()
{
    deserialize();

    u2uint64 ulStartTime = 0L;
    DATAPOOL(ON_DataPool_Memory)->loadMemoryUint64Data(ON_NetworkTimeStart, ulStartTime);
    u2uint64 ulEndTime = Root::getSingleton().getTimer()->getMilliseconds();
    DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_TripLatency, (ulEndTime - ulStartTime) / 2);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
RegisterHRsp::RegisterHRsp(const u2::String& type, const u2::String& name, const u2::String& guid)
    : HttpResponse(type, name, guid)
    , m_bDeserializeSucceed(false)
    , m_nCode(0)
    , m_ulSelfPlayerGuid(0L)
{
}
//-----------------------------------------------------------------------
RegisterHRsp::~RegisterHRsp()
{
}
//-----------------------------------------------------------------------
void RegisterHRsp::deserialize()
{
    do
    {
        std::string szJson(m_Data.begin(), m_Data.end());

        rapidjson::Document document;
        document.Parse(szJson.c_str());

        if (document.HasParseError())
        {
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: Parse failed, "
                << "error = " << document.GetParseError()
                << ", offset = " << document.GetErrorOffset()
                << ", info = " << rapidjson::GetParseError_En(document.GetParseError())
                << ", json = " << szJson;
            break;
        }

        CHECK_RAPIDJSON_VALIDITY(document.IsObject());

        CHECK_RAPIDJSON_MEMBER(document, "code");
        CHECK_RAPIDJSON_VALIDITY(document["code"].IsInt());
        m_nCode = document["code"].GetInt();

        CHECK_RAPIDJSON_MEMBER(document, "message");
        CHECK_RAPIDJSON_VALIDITY(document["message"].IsString());
        m_szMsg = document["message"].GetString();

        CHECK_RAPIDJSON_MEMBER(document, "data");
        CHECK_RAPIDJSON_VALIDITY(document["data"].IsUint64());
        m_ulSelfPlayerGuid = document["data"].IsUint64();

        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
//-----------------------------------------------------------------------
void RegisterHRsp::run()
{
    deserialize();
    if (m_nCode == 0)
    {
        bool bSuc = DATAPOOL(ON_DataPool_Memory)->saveMemoryStringData("SelfPlayerGuid"
            , StringUtil::toString(m_ulSelfPlayerGuid));

        TaskLoop* pHttpTaskLoop = TaskLoopManager::getSingleton().retrieveObjectByName("http");
        HttpRequest* pHttpReq = static_cast<HttpRequest*>(
            TaskManager::getSingleton().createObject(GET_OBJECT_TYPE(PlayHReq))
            );
        pHttpTaskLoop->postTask(pHttpReq);
    }
    else
    {
        LogManager::getSingleton().stream(LML_NORMAL)
            << "Register failed, code = " << m_nCode
            << ", " << m_szMsg;
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PlayHRsp::PlayHRsp(const u2::String& type, const u2::String& name, const u2::String& guid)
    : HttpResponse(type, name, guid)
    , m_bDeserializeSucceed(false)
    , m_nCode(0)
    , m_nRoomState(0)
    , m_ulEnterRoomTimestamp(0L)
    , m_ulCreateRoomTimestamp(0L)
    , m_ulStartRoomTimestamp(0L)
{
}
//-----------------------------------------------------------------------
PlayHRsp::~PlayHRsp()
{
}
//-----------------------------------------------------------------------
void PlayHRsp::deserialize()
{
    do
    {
        std::string szJson(m_Data.begin(), m_Data.end());
        LogManager::getSingleton().stream(LML_TRIVIAL) << "PlayHRsp: " << szJson;

        rapidjson::Document document;
        document.Parse(szJson.c_str());

        if (document.HasParseError())
        {
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: Parse failed, "
                << "error = " << document.GetParseError() 
                << ", offset = " << document.GetErrorOffset() 
                << ", info = " << rapidjson::GetParseError_En(document.GetParseError())
                << ", json = " << szJson;
            break;
        }

        CHECK_RAPIDJSON_VALIDITY(document.IsObject());

        CHECK_RAPIDJSON_MEMBER(document, "code");
        CHECK_RAPIDJSON_VALIDITY(document["code"].IsInt());
        m_nCode = document["code"].GetInt();

        CHECK_RAPIDJSON_MEMBER(document, "message");
        CHECK_RAPIDJSON_VALIDITY(document["message"].IsString());
        m_szMsg = document["message"].GetString();

        CHECK_RAPIDJSON_MEMBER(document, "data");
        CHECK_RAPIDJSON_VALIDITY(document["data"].IsObject());
        const rapidjson::Value& dataJsonVal = document["data"];

        CHECK_RAPIDJSON_MEMBER(dataJsonVal, "enterRoomTime");
        CHECK_RAPIDJSON_VALIDITY(dataJsonVal["enterRoomTime"].IsUint64());
        m_ulEnterRoomTimestamp = dataJsonVal["enterRoomTime"].GetUint64();

        CHECK_RAPIDJSON_MEMBER(dataJsonVal, "heros");
        CHECK_RAPIDJSON_VALIDITY(dataJsonVal["heros"].IsArray());
        const rapidjson::Value& herosJsonVal = dataJsonVal["heros"];

        for (rapidjson::SizeType i = 0; i < herosJsonVal.Size(); i++)
        {
            GameMovableSnapshot* pPlayerSnapshot = dynamic_cast<GameMovableSnapshot*>(
                MovableSnapshotManager::getSingleton().reuseObject(GET_OBJECT_TYPE(GameMovableSnapshot)));
            if (_deserializeHero(herosJsonVal[i], pPlayerSnapshot))
            {
                m_FrameSnapshot[pPlayerSnapshot->szGameObjGuid] = pPlayerSnapshot;
            }
            else
            {
                MovableSnapshotManager::getSingleton().recycleObject(pPlayerSnapshot);
                m_bDeserializeSucceed = false;
            }
        }

        CHECK_RAPIDJSON_MEMBER(dataJsonVal, "room");
        CHECK_RAPIDJSON_VALIDITY(dataJsonVal["room"].IsObject());
        const rapidjson::Value& roomJsonVal = dataJsonVal["room"];

        CHECK_RAPIDJSON_MEMBER(roomJsonVal, "roomId");
        CHECK_RAPIDJSON_VALIDITY(roomJsonVal["roomId"].IsUint());
        m_szRoomGuid = StringUtil::toString(roomJsonVal["roomId"].GetUint());

        CHECK_RAPIDJSON_MEMBER(roomJsonVal, "state");
        CHECK_RAPIDJSON_VALIDITY(roomJsonVal["state"].IsInt());
        m_nRoomState = roomJsonVal["state"].GetInt();

        CHECK_RAPIDJSON_MEMBER(roomJsonVal, "createTime");
        CHECK_RAPIDJSON_VALIDITY(roomJsonVal["createTime"].IsUint64());
        m_ulCreateRoomTimestamp = roomJsonVal["createTime"].GetUint64();

        CHECK_RAPIDJSON_MEMBER(roomJsonVal, "beginTime");
        CHECK_RAPIDJSON_VALIDITY(roomJsonVal["beginTime"].IsUint64());
        m_ulStartRoomTimestamp = roomJsonVal["beginTime"].GetUint64();


        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
//-----------------------------------------------------------------------
bool PlayHRsp::_deserializeHero(const rapidjson::Value& jsonValue, GameMovableSnapshot* gameMovableSnapshot)
{
    do
    {
        CHECK_RAPIDJSON_MEMBER(jsonValue, "type");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["type"].IsInt());
        gameMovableSnapshot->szGameObjType = StringUtil::toString(jsonValue["type"].GetInt());

        CHECK_RAPIDJSON_MEMBER(jsonValue, "userId");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["userId"].IsUint64());
        gameMovableSnapshot->szPlayerId = StringUtil::toString(jsonValue["userId"].GetUint64());

        CHECK_RAPIDJSON_MEMBER(jsonValue, "heroId");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["heroId"].IsUint());
        gameMovableSnapshot->szGameObjGuid = StringUtil::toString(jsonValue["heroId"].GetUint());

        CHECK_RAPIDJSON_MEMBER(jsonValue, "nickName");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["nickName"].IsString());
        gameMovableSnapshot->szPlayerName = jsonValue["nickName"].GetString();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "hp");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["hp"].IsInt());
        gameMovableSnapshot->uCurHp = jsonValue["hp"].GetInt();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "speed");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["speed"].IsNumber());
        gameMovableSnapshot->rCurSpeed = jsonValue["speed"].GetDouble();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "effDistance");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["effDistance"].IsNumber());
        gameMovableSnapshot->rAtkDistance = jsonValue["effDistance"].GetDouble();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "alive");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["alive"].IsBool());
        gameMovableSnapshot->bAlive = jsonValue["alive"].GetBool();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "point");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["point"].IsObject());
        const rapidjson::Value& pointJsonVal = jsonValue["point"];

        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "timestamp");
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["timestamp"].IsUint64());
        gameMovableSnapshot->ulTimestamp = pointJsonVal["timestamp"].GetUint64();

        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "x");
        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "y");
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["x"].IsNumber());
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["y"].IsNumber());
        gameMovableSnapshot->v2Position.x = pointJsonVal["x"].GetDouble();
        gameMovableSnapshot->v2Position.y = pointJsonVal["y"].GetDouble();

        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "vx");
        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "vy");
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["vx"].IsNumber());
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["vy"].IsNumber());
        gameMovableSnapshot->v2Velocity.x = pointJsonVal["vx"].GetDouble();
        gameMovableSnapshot->v2Velocity.y = pointJsonVal["vy"].GetDouble();

        return true;
    } while (0);

    return false;
}
//-----------------------------------------------------------------------
void PlayHRsp::run()
{
    deserialize();
    if (m_nCode == 0)
    {
        // save room id
        DATAPOOL(ON_DataPool_Memory)->saveMemoryStringData("SelfRoomId", m_szRoomGuid);
        Root::getSingleton().getTimer()->reset();

        // save time elapse between enter room time and start room time on server
        DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_ServerTimeElapse_EnterRoom
            , m_ulEnterRoomTimestamp - m_ulStartRoomTimestamp);
        Root::getSingleton().getTimer()->reset();

        // save locale time of enter room
        DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_LocaleTime_EnterRoom
            , Root::getSingleton().getTimer()->getMilliseconds());

        // load self player id
        String szSelfPlayerGuid;
        bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfPlayerGuid", szSelfPlayerGuid);

        // find self game object guid
        bool bFound = false;
        for (Scene::FrameSnapshot::iterator it = m_FrameSnapshot.begin(); 
        it != m_FrameSnapshot.end(); it++)
        {
            GameMovableSnapshot* pMovableSnapshot = dynamic_cast<GameMovableSnapshot*>(it->second);
            if (pMovableSnapshot == nullptr)
            {
                assert(0);
            }
            else
            {
                if (pMovableSnapshot->szPlayerId == szSelfPlayerGuid)
                {
                    bSuc = DATAPOOL(ON_DataPool_Memory)->saveMemoryStringData("SelfGameObjGuid"
                        , pMovableSnapshot->szGameObjGuid);
                    bFound = true;
                    break;
                }
            }
        }

        // add frame snapshot
        Scene::getSingleton().addFrameSnapshot(&m_FrameSnapshot);

        if (bFound)
        {
            ((GameScene*)Scene::getSingletonPtr())->connect();
        }
        else
        {
            assert(0);
        }


    }
    else
    {
        LogManager::getSingleton().stream(LML_NORMAL)
            << "Play failed, code = " << m_nCode
            << ", " << m_szMsg 
            << "[ " << __FUNCTION__ << ", " << __LINE__ << " ]";
    }
}