#include "GameHttpResponses.h"

#include "application/AppPrerequisites.h"
#include "GameHttpRequests.h"
#include "ecs/GameComponents.h"
#include "ecs/GameSnapshot.h"
#include "tasks/GameWsClientImpl.h"



#define CHECK_JSON_MEMBER(JsonValue, member)                            \
    if (!JsonValue.isMember(member))                                    \
    {                                                                   \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: "                                         \
            << "Parse failed, as no member "                            \
            << member                                                   \
            << ". [ " << __FUNCTION__ << ", " << __LINE__ << " ]";      \
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
//     DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_ServerBaseTime, m_ulServerBaseTime);
//     u2uint64 ulLocalTime = Root::getSingleton().getTimer()->getMilliseconds();
//     DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_ClientBaseTime, ulLocalTime);
//     DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_DeltaBaseTime, ulLocalTime - m_ulServerBaseTime);

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

        Json::Reader reader;
        Json::Value rootJsonVal;
        if (!reader.parse(szJson, rootJsonVal))
        {
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: RegisterHRsp parse failed!";
            break;
        }

        CHECK_JSON_MEMBER(rootJsonVal, "code");
        m_nCode = rootJsonVal["code"].asInt();

        CHECK_JSON_MEMBER(rootJsonVal, "message");
        m_szMsg = rootJsonVal["message"].asString();

        CHECK_JSON_MEMBER(rootJsonVal, "data");
        m_ulSelfPlayerGuid = rootJsonVal["data"].asUInt64();

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
    , m_ulTimestamp(0L)
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

        Json::Reader reader;
        Json::Value rootJsonVal;
        if (!reader.parse(szJson, rootJsonVal))
        {
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: PlayHRsp parse failed!";
            break;
        }

        CHECK_JSON_MEMBER(rootJsonVal, "code");
        m_nCode = rootJsonVal["code"].asInt();

        CHECK_JSON_MEMBER(rootJsonVal, "message");
        m_szMsg = rootJsonVal["message"].asString();

        CHECK_JSON_MEMBER(rootJsonVal, "data");
        Json::Value dataJsonVal = rootJsonVal["data"];

        CHECK_JSON_MEMBER(dataJsonVal, "timestamp");
        m_ulTimestamp = dataJsonVal["timestamp"].asUInt64();

        CHECK_JSON_MEMBER(dataJsonVal, "heros");
        Json::Value herosJsonVal = dataJsonVal["heros"];

        for (Json::ArrayIndex i = 0; i < herosJsonVal.size(); i++)
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

        CHECK_JSON_MEMBER(dataJsonVal, "room");
        Json::Value roomJsonVal = dataJsonVal["room"];

        CHECK_JSON_MEMBER(roomJsonVal, "roomId");
        m_szRoomGuid = roomJsonVal["roomId"].asString();

        CHECK_JSON_MEMBER(roomJsonVal, "state");
        m_nRoomState = roomJsonVal["state"].asInt();

        CHECK_JSON_MEMBER(roomJsonVal, "createTime");
        m_ulCreateRoomTimestamp = roomJsonVal["createTime"].asUInt64();

        CHECK_JSON_MEMBER(roomJsonVal, "beginTime");
        m_ulStartRoomTimestamp = roomJsonVal["beginTime"].asUInt64();
        

        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
//-----------------------------------------------------------------------
bool PlayHRsp::_deserializeHero(Json::Value& jsonValue, GameMovableSnapshot* gameMovableSnapshot)
{
    do
    {
        CHECK_JSON_MEMBER(jsonValue, "type");
        gameMovableSnapshot->szGameObjType = jsonValue["type"].asString();

        CHECK_JSON_MEMBER(jsonValue, "userId");
        gameMovableSnapshot->szPlayerId = jsonValue["userId"].asString();

        CHECK_JSON_MEMBER(jsonValue, "heroId");
        gameMovableSnapshot->szGameObjGuid = jsonValue["heroId"].asString();

        CHECK_JSON_MEMBER(jsonValue, "nickName");
        gameMovableSnapshot->szPlayerName = jsonValue["nickName"].asString();

        CHECK_JSON_MEMBER(jsonValue, "hp");
        gameMovableSnapshot->uCurHp = jsonValue["hp"].asInt();

        CHECK_JSON_MEMBER(jsonValue, "speed");
        gameMovableSnapshot->uCurSpeed = jsonValue["speed"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "effDistance");
        gameMovableSnapshot->uAtkDistance = jsonValue["effDistance"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "alive");
        gameMovableSnapshot->bAlive = jsonValue["alive"].asBool();

        CHECK_JSON_MEMBER(jsonValue, "point");
        Json::Value pointJsonVal = jsonValue["point"];

        CHECK_JSON_MEMBER(pointJsonVal, "timestamp");
        gameMovableSnapshot->ulTimestamp = pointJsonVal["timestamp"].asUInt64();

        CHECK_JSON_MEMBER(pointJsonVal, "x");
        CHECK_JSON_MEMBER(pointJsonVal, "y");
        gameMovableSnapshot->v2Position.x = pointJsonVal["x"].asFloat();
        gameMovableSnapshot->v2Position.y = pointJsonVal["y"].asFloat();

        CHECK_JSON_MEMBER(pointJsonVal, "vx");
        CHECK_JSON_MEMBER(pointJsonVal, "vy");
        gameMovableSnapshot->v2Velocity.x = pointJsonVal["vx"].asFloat();
        gameMovableSnapshot->v2Velocity.y = pointJsonVal["vy"].asFloat();

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

        // save room start time
        DATAPOOL(ON_DataPool_Memory)->saveMemoryUint64Data(ON_ServerStartRoomTime, m_ulStartRoomTimestamp);
        Root::getSingleton().getTimer()->reset();

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