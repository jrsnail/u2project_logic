#include "GameRoot.h"

#include "U2CocosFrameListenerCollection.h"
#include "ecs/GameComponentManager.h"
#include "ecs/GameComponents.h"
#include "ecs/GameSystems.h"
#include "ecs/GameSnapshot.h"
#include "tasks/GameHttpRequests.h"
#include "tasks/GameHttpResponses.h"
#include "tasks/GameSendSocketTasks.h"
#include "tasks/GameRecvSocketTasks.h"
#include "tasks/GameWsClientImpl.h"
#include "application/AppCommands.h"
#include "application/AppViewComponents.h"
#include "application/AppLuaTasks.h"
#include "application/AppFacade.h"
#include "battle/BattleCommands.h"
#include "battle/JoystickViewComponent.h"
#if U2_PLATFORM == U2_PLATFORM_ANDROID
#	include "CCFileUtils-android.h"
#	include <android/asset_manager.h>
#	include <android/asset_manager_jni.h>
#endif
#include <google/protobuf/stubs/common.h>


    
static void initGameFactories()
{
    // proxy factory
    // command factory
    CREATE_FACTORY(StartupCommand);
    CREATE_FACTORY(Trans2ShadeCommand);
    CREATE_FACTORY(StartupLuaCommand);
    CREATE_FACTORY(Trans2BattleCommand);

    // lua script factory
    CREATE_FACTORY(CocosLuaScript);

    // lua 2 c task
    CREATE_FACTORY(CreateLuaScriptLuaTask);
    CREATE_FACTORY(SetViewCompUiNameLuaTask);
    CREATE_FACTORY(SetViewCompEnterActionLuaTask);
    CREATE_FACTORY(SetViewCompExitActionLuaTask);
    CREATE_FACTORY(CenterViewCompLuaTask);
    CREATE_FACTORY(PreloadLuaTask);

    // c 2 lua task
    CREATE_LUATASK_FACTORY(OT_C2LTask_ViewCompCreated);
    CREATE_LUATASK_FACTORY(OT_C2LTask_ButtonCliecked);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesBegan);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesMoved);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesEnded);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesCancelled);
    CREATE_LUATASK_FACTORY(OT_C2LTask_KeyPressed);
    CREATE_LUATASK_FACTORY(OT_C2LTask_KeyReleased);
    CREATE_LUATASK_FACTORY(OT_C2LTask_PreloadEnd);
    CREATE_LUATASK_FACTORY("Test_L2CType");

    // view component factory
    CREATE_FACTORY(CocosViewComponent);
    CREATE_FACTORY(ShadeViewComponent);
    CREATE_FACTORY(JoystickViewComponent);

    // component
    CREATE_FACTORY_WITH_TYPE(SpriteComponent, "component_sprite");
    CREATE_FACTORY_WITH_TYPE(PositionComponent, "component_position");
    CREATE_FACTORY_WITH_TYPE(VelocityComponent, "component_velocity");
    CREATE_FACTORY_WITH_TYPE(SpeedDirComponent, "component_speed_dir");
    CREATE_FACTORY_WITH_TYPE(SpeedComponent, "component_speed");
    CREATE_FACTORY_WITH_TYPE(HpComponent, "component_hp");
    CREATE_FACTORY_WITH_TYPE(BaseHpComponent, "component_base_hp");
    CREATE_FACTORY_WITH_TYPE(DeltaHpComponent, "component_delta_hp");
    CREATE_FACTORY_WITH_TYPE(JoystickComponent, "component_joystick");
    CREATE_FACTORY_WITH_TYPE(ScaleComponent, "component_scale");
    CREATE_FACTORY_WITH_TYPE(PredictSelfComponent, "component_predict_self");
    CREATE_FACTORY_WITH_TYPE(PredictOtherComponent, "component_predict_other");

    // system
    CREATE_FACTORY_WITH_TYPE(RenderSystem, "system_render");
    CREATE_FACTORY_WITH_TYPE(InputSystem, "system_input");
    CREATE_FACTORY_WITH_TYPE(MoveSystem, "system_move");
    CREATE_FACTORY_WITH_TYPE(ScaleSystem, "system_scale");
    CREATE_FACTORY_WITH_TYPE(PredictSelfSystem, "system_predict_self");
    CREATE_FACTORY_WITH_TYPE(PredictOtherSystem, "system_predict_other");

    // snapshot
    CREATE_FACTORY(GameMovableSnapshot);
    CREATE_FACTORY(GameControlSnapshot);

    // task loop
    CREATE_FACTORY(GameWsTaskLoop);

    // tasks
    CREATE_FACTORY(GameWsCloseRST);
    CREATE_FACTORY(GameWsErrorRST);
    CREATE_FACTORY(GameWsOpenRST);
    CREATE_FACTORY(GameWsHeartBeatSST);
    CREATE_FACTORY(TimeHReq);
    CREATE_FACTORY(TimeHRsp);
    CREATE_FACTORY(RegisterHReq);
    CREATE_FACTORY(RegisterHRsp);
    CREATE_FACTORY(PlayHReq);
    CREATE_FACTORY(PlayHRsp);
    CREATE_FACTORY(MoveSST);
    CREATE_FACTORY_WITH_TYPE(SnapshotRST, "plane");
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameRoot::GameRoot()
    : Root()
{
    _initialize();
}
//-----------------------------------------------------------------------
GameRoot::~GameRoot()
{
    U2_DELETE m_pGameScene;
}
//-----------------------------------------------------------------------
void GameRoot::_initialize()
{
    initGameFactories();
    initFactroy();

    // Create log manager and default log file if there is no log manager yet
    if (u2::LogManager::getSingletonPtr() == nullptr)
    {
        m_pLogManager = U2_NEW u2::LogManager();
        m_pLogManager->createLog("u2.log", true, true);
#if U2_DEBUG_MODE == 1
        m_pLogManager->setLogDetail(LoggingLevel::LL_BOREME);
#endif
    }

#if U2_PLATFORM == U2_PLATFORM_ANDROID
    AndroidLogListener* mAndroidLogger = U2_NEW AndroidLogListener();
    m_pLogManager->getDefaultLog()->addListener(mAndroidLogger);
#endif

    // archive
    m_pArchiveManager = U2_NEW ArchiveManager;
    if (!FactoryManager::getSingleton().hasObjectFactory("FileSystem"))
    {
        ObjectFactory* pObjectFactory = U2_NEW FileSystemArchiveFactory;
        FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
    }
    if (!u2::FactoryManager::getSingleton().hasObjectFactory("Zip"))
    {
        ObjectFactory* pObjectFactory = U2_NEW ZipArchiveFactory;
        FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
    }
    if (!FactoryManager::getSingleton().hasObjectFactory("EmbeddedZip"))
    {
        ObjectFactory* pObjectFactory = U2_NEW EmbeddedZipArchiveFactory;
        FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
    }
#if U2_PLATFORM == U2_PLATFORM_ANDROID
    //AAssetManager* pAssetMgr = AAssetManager_fromJava(env, assetManager);
    AAssetManager* pAssetMgr = cocos2d::FileUtilsAndroid::getAssetManager();
    if (pAssetMgr)
    {
        if (!FactoryManager::getSingleton().hasObjectFactory("ApkFileSystem"))
        {
            ObjectFactory* pObjectFactory = U2_NEW ApkFileSystemArchiveFactory(pAssetMgr);
            FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
        }
        if (!FactoryManager::getSingleton().hasObjectFactory("ApkZip"))
        {
            ObjectFactory* pObjectFactory = U2_NEW ApkZipArchiveFactory(pAssetMgr);
            FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
        }
    }
#endif

    // ResourceGroupManager
    if (ResourceGroupManager::getSingletonPtr() == nullptr)
    {
        m_pResourceGroupManager = U2_NEW ResourceGroupManager;
    }

    // data pool
    if (DataPoolManager::getSingletonPtr() == nullptr)
    {
        m_pDataPoolManager = U2_NEW DataPoolManager;
        m_pDataPoolManager->createObject(GET_OBJECT_TYPE(DataPool), ON_DataPool_Memory);
        m_pDataPoolManager->createObject(GET_OBJECT_TYPE(DataPool), ON_DataPool_DB);
    }

    // script manager
    if (ScriptManager::getSingletonPtr() == nullptr)
    {
        m_pScriptManager = U2_NEW LuaScriptManager;
    }

    // component manager
    if (ComponentManager::getSingletonPtr() == nullptr)
    {
        m_pComponentManager = U2_NEW GameComponentManager;
    }
    // game object manager
    if (GameObjectManager::getSingletonPtr() == nullptr)
    {
        m_pGameObjectManager = U2_NEW GameObjectManager;
    }

    // timer
    m_pTimer = U2_NEW Timer;

    // network synchronize
    if (MovableSnapshotManager::getSingletonPtr() == nullptr)
    {
        m_pMovableSnapshotManager = U2_NEW MovableSnapshotManager;
    }
    if (ControlSnapshotManager::getSingletonPtr() == nullptr)
    {
        m_pControlSnapshotManager = U2_NEW ControlSnapshotManager;
    }

    // frame listener collection
    if (FrameListenerCollection::getSingletonPtr() == nullptr)
    {
        m_pFrameListenerCollection = U2_NEW CocosFrameListenerCollection;
    }

    // task manager
    if (TaskLoopManager::getSingletonPtr() == nullptr)
    {
        m_pTaskLoopManager = U2_NEW TaskLoopManager;
    }

    // load resources
    _loadResources();




    // Logic task loop
    TaskLoop* pLogicTaskLoop 
        = TaskLoopManager::getSingleton().createObject(GET_OBJECT_TYPE(LogicTaskLoop), ON_Logic_TaskLoop);
    pLogicTaskLoop->run();

    // predefined facade
    Facade::createFacade<PredefinedFacade>(ON_Facade_Predefined);
    
}
//-----------------------------------------------------------------------
void GameRoot::enter()
{
    // game facade
    AppFacade* pAppFacade = Facade::createFacade<AppFacade>(ON_Facade_App);
    if (pAppFacade != nullptr)
    {
        pAppFacade->startup();
    }

    {
        //------------------------------- ECS ----------------------------------------
        m_pGameScene = U2_NEW GameScene;

        //          GameObject* pSelf = GameObjectManager::getSingleton().createObject("aircraft", "self_aircraft");
        //          pSelf->addComponent(ComponentManager::getSingleton().createObject("component_joystick"));

        SystemManager::getSingleton().createObject("system_input", "system_input", BLANK, 9980);
        //SystemManager::getSingleton().createObject("system_move", "system_move", BLANK, 9990);
        SystemManager::getSingleton().createObject("system_predict_self", "system_predict_self", BLANK, 9990);
        SystemManager::getSingleton().createObject("system_predict_other", "system_predict_other", BLANK, 9991);
        SystemManager::getSingleton().createObject("system_scale", "system_scale", BLANK, 9995);
        SystemManager::getSingleton().createObject("system_render", "system_render", BLANK, 10000);

    }

    {
        //------------------------------- Net ----------------------------------------
        // http
        TaskLoop* pHttpTaskLoop
            = TaskLoopManager::getSingleton().createObject(GET_OBJECT_TYPE(ActiveHttpTaskLoop), "http");
        pHttpTaskLoop->run();
        Task* pHttpReq = TaskManager::getSingleton().createObject(GET_OBJECT_TYPE(TimeHReq));
        pHttpTaskLoop->postTask(pHttpReq);
        Task* pRegisterReq = TaskManager::getSingleton().createObject(GET_OBJECT_TYPE(RegisterHReq));
        pHttpTaskLoop->postTask(pRegisterReq);
    }

    {
        //------------------------------- protobuf ----------------------------------------
        // Verify that the version of the library that we linked against is
        // compatible with the version of the headers we compiled against.
        GOOGLE_PROTOBUF_VERIFY_VERSION;
    }
    

    SystemManager::getSingleton().enter();
}
//-----------------------------------------------------------------------
void GameRoot::exit()
{
    SystemManager::getSingleton().exit();

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    Root::exit();
}
//-----------------------------------------------------------------------
void GameRoot::pause()
{
    SystemManager::getSingleton().pause();
    Root::pause();
}
//-----------------------------------------------------------------------
void GameRoot::resume()
{
    SystemManager::getSingleton().resume();
    Root::resume();
}