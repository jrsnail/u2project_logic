#include "U2Root.h"

#include "U2Archive.h"
#include "U2FileSystemArchive.h"
#include "U2ZipArchive.h"
#if U2_PLATFORM == U2_PLATFORM_ANDROID
#	include "platform/android/CCFileUtils-android.h"
#	include <android/asset_manager.h>
#	include <android/asset_manager_jni.h>
#	include "U2ApkFileSystemArchive.h"
#	include "U2ApkZipArchive.h"
#	include "U2AndroidLogListener.h"
#endif
#if U2_PLATFORM == U2_PLATFORM_APPLE_IOS
#	include "iosUtils.h"
#endif
#if U2_PLATFORM == U2_PLATFORM_APPLE
#	include "macUtils.h"
#endif
#include "U2LogManager.h"
#include "U2ConfigFile.h"
#include "U2Component.h"
#include "U2GameObject.h"
#include "U2Timer.h"
#include "U2Snapshot.h"
#include "U2DataPool.h"
#include "U2Script.h"
#include "U2LuaScript.h"
#include "U2FrameListenerCollection.h"
#include "U2Task.h"
#include "U2TaskLoop.h"
#include "U2FactoryManager.h"
#include "U2PredefinedPrerequisites.h"



U2EG_NAMESPACE_USING


    
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> Root* Singleton<Root>::msSingleton = 0;
Root* Root::getSingletonPtr(void)
{
    return msSingleton;
}
//-----------------------------------------------------------------------
Root& Root::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
Root::Root()
{
    
}
//-----------------------------------------------------------------------
Root::~Root()
{
    U2_DELETE m_pTimer;
    U2_DELETE m_pMovableSnapshotManager;
    U2_DELETE m_pControlSnapshotManager;
    U2_DELETE m_pGameObjectManager;
    U2_DELETE m_pDataPoolManager;
    U2_DELETE m_pScriptManager;
    U2_DELETE m_pFrameListenerCollection;
    U2_DELETE m_pTaskManager;
    U2_DELETE m_pTaskLoopManager;
    U2_DELETE m_pArchiveManager;
    U2_DELETE m_pResourceGroupManager;
    U2_DELETE m_pFactoryManager;
    U2_DELETE m_pLogManager;
}
//-----------------------------------------------------------------------
void Root::_initialize()
{
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

    // factory
    if (u2::FactoryManager::getSingletonPtr() == nullptr)
    {
        m_pFactoryManager = U2_NEW FactoryManager;
    }

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
        m_pComponentManager = U2_NEW ComponentManager;
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
        m_pFrameListenerCollection = U2_NEW FrameListenerCollection;
    }

    // task manager
    if (TaskManager::getSingletonPtr() == nullptr)
    {
        m_pTaskManager = U2_NEW TaskManager;
    }
    if (TaskLoopManager::getSingletonPtr() == nullptr)
    {
        m_pTaskLoopManager = U2_NEW TaskLoopManager;
    }

    // load resources
    _loadResources();

    initFactroy();
}
//-----------------------------------------------------------------------
void Root::_loadResources()
{
#if U2_PLATFORM == U2_PLATFORM_APPLE || U2_PLATFORM == U2_PLATFORM_APPLE_IOS
    Archive* pConfigArchive = ArchiveManager::getSingleton().createObject(
        "FileSystem", macResourcePath(), true);
#elif U2_PLATFORM == U2_PLATFORM_ANDROID
    Archive* pConfigArchive = ArchiveManager::getSingleton().createObject(
        "ApkFileSystem", "/", true);
#else
    Archive* pConfigArchive = ArchiveManager::getSingleton().createObject(
        "FileSystem", ".", true);
    // 		Archive* pConfigArchive = ArchiveManager::getSingleton().createObject(
    // 			"Zip", "./Test.zip", true);
#endif
    InStreamPtr stream = pConfigArchive->openForRead("resources.cfg");

    String secName, typeName, archName;
    ConfigFile cf;
    cf.load(stream);

    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if U2_PLATFORM == U2_PLATFORM_APPLE || U2_PLATFORM == U2_PLATFORM_APPLE_IOS
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = u2::String(macResourcePath() + "/" + archName);
#endif
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            u2::LogManager::getSingleton().stream(u2::LML_NORMAL)
                << "archName:" << archName
                << ", typeName:" << typeName
                << ", secName:" << secName << "\n";
        }
    }

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-----------------------------------------------------------------------
Timer* Root::getTimer()
{
    return m_pTimer;
}
//-----------------------------------------------------------------------
void Root::enter()
{

}
//-----------------------------------------------------------------------
void Root::exit()
{
    TaskLoopManager::getSingleton().quitAll();
}
//-----------------------------------------------------------------------
void Root::pause()
{
    TaskLoopManager::getSingleton().pauseAll();
}
//-----------------------------------------------------------------------
void Root::resume()
{
    TaskLoopManager::getSingleton().resumeAll();
}