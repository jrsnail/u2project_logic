﻿#include "U2Snapshot.h"

#include "U2FrameListenerCollection.h"
#include "U2GameObject.h"
#include "U2LogManager.h"
#include "U2Root.h"
#include "U2Timer.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> MovableSnapshotManager* Singleton<MovableSnapshotManager>::msSingleton = 0;
MovableSnapshotManager* MovableSnapshotManager::getSingletonPtr(void)
{
    return msSingleton;
}
MovableSnapshotManager& MovableSnapshotManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
MovableSnapshotManager::MovableSnapshotManager()
{
}
//-----------------------------------------------------------------------
MovableSnapshotManager::~MovableSnapshotManager()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ControlSnapshotManager* Singleton<ControlSnapshotManager>::msSingleton = 0;
ControlSnapshotManager* ControlSnapshotManager::getSingletonPtr(void)
{
    return msSingleton;
}
ControlSnapshotManager& ControlSnapshotManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ControlSnapshotManager::ControlSnapshotManager()
{
}
//-----------------------------------------------------------------------
ControlSnapshotManager::~ControlSnapshotManager()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> Scene* Singleton<Scene>::msSingleton = 0;
Scene* Scene::getSingletonPtr(void)
{
    return msSingleton;
}
Scene& Scene::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
Scene::Scene()
{
}
//-----------------------------------------------------------------------
Scene::~Scene()
{
}
//-----------------------------------------------------------------------
void Scene::addFrameSnapshot(FrameSnapshot* frame)
{
    if (frame == nullptr)
    {
        return;
    }

    // remove redundant data
    for (GameObjMoveableSnapshotMap::iterator it = m_Scene.begin(); 
    it != m_Scene.end(); it++)
    {
        FrameSnapshot::iterator it2 = frame->find(it->first);
        if (it2 == frame->end())
        {
            eraseMovableSnapshotByGameObjGuid(it->first);
        }
    }
    
    // insert new data
    for (FrameSnapshot::iterator it = frame->begin(); it != frame->end(); it++)
    {
        MoveableSnapshotMap& movableSnapshots = m_Scene[it->first];

        MovableSnapshot* pMovableSnapshot = it->second;
        MoveableSnapshotMap::iterator itMovable = movableSnapshots.find(pMovableSnapshot->ulTimestamp);
        if (itMovable == movableSnapshots.end())
        {
            if (movableSnapshots.size() > 0)
            {
                if (pMovableSnapshot->ulTimestamp < movableSnapshots.rbegin()->second->ulTimestamp)
                {
                    assert(0);
                }
            }
            
            movableSnapshots.insert(std::make_pair(pMovableSnapshot->ulTimestamp, pMovableSnapshot));
        }
        else
        {
            u2uint64 ulStart4 = Root::getSingleton().getTimer()->getMicroseconds();
            //assert(0);
            MovableSnapshotManager::getSingleton().recycleObject(pMovableSnapshot);
            u2uint64 ulDelta4 = Root::getSingleton().getTimer()->getMicroseconds() - ulStart4;
            LogManager::getSingleton().stream(LML_TRIVIAL) << "Scene::addFrameSnapshot 4: " << ulDelta4;
            continue;
        }
        
        if (movableSnapshots.size() > 2)
        {
            u2uint64 ulStart5 = Root::getSingleton().getTimer()->getMicroseconds();
            MoveableSnapshotMap::iterator it2 = movableSnapshots.begin();
            MovableSnapshotManager::getSingleton().recycleObject(it2->second);
            movableSnapshots.erase(it2);
            u2uint64 ulDelta5 = Root::getSingleton().getTimer()->getMicroseconds() - ulStart5;
            LogManager::getSingleton().stream(LML_TRIVIAL) << "Scene::addFrameSnapshot 5: " << ulDelta5;
        }
    }
}
//-----------------------------------------------------------------------
void Scene::eraseMovableSnapshotByGameObjGuid(const String& guid)
{
    GameObjMoveableSnapshotMap::iterator it = m_Scene.find(guid);
    if (it == m_Scene.end())
    {
        
    }
    else
    {
        MoveableSnapshotMap& movableSnapshots = m_Scene[guid];
        for (MoveableSnapshotMap::iterator itMovable = movableSnapshots.begin(); 
        itMovable != movableSnapshots.end(); itMovable++)
        {
            MovableSnapshotManager::getSingleton().recycleObject(itMovable->second);
        }
        movableSnapshots.clear();

        m_Scene.erase(it);
    }
}
//-----------------------------------------------------------------------
void Scene::clearMovableSnapshotByGameObjGuid(const String& guid)
{
    GameObjMoveableSnapshotMap::iterator it = m_Scene.find(guid);
    if (it == m_Scene.end())
    {

    }
    else
    {
        MoveableSnapshotMap& movableSnapshots = m_Scene[guid];
        for (MoveableSnapshotMap::iterator itMovable = movableSnapshots.begin();
        itMovable != movableSnapshots.end(); itMovable++)
        {
            MovableSnapshotManager::getSingleton().recycleObject(itMovable->second);
        }
        movableSnapshots.clear();
    }
}
//-----------------------------------------------------------------------
const Scene::MoveableSnapshotMap&
Scene::retrieveAllMoveableSnapshotsByGameObjGuid(const String& guid) const
{
    GameObjMoveableSnapshotMap::const_iterator it = m_Scene.find(guid);
    if (it == m_Scene.end())
    {
        assert(0);
        static MoveableSnapshotMap moveableSnapshotMap;
        return moveableSnapshotMap;
    }
    else
    {
        return it->second;
    }
}
//-----------------------------------------------------------------------
Scene::MoveableSnapshotMap&
Scene::retrieveAllMoveableSnapshotsByGameObjGuid(const String& guid)
{
    GameObjMoveableSnapshotMap::iterator it = m_Scene.find(guid);
    if (it == m_Scene.end())
    {
        assert(0);
        static MoveableSnapshotMap moveableSnapshotMap;
        return moveableSnapshotMap;
    }
    else
    {
        return it->second;
    }
}
//-----------------------------------------------------------------------
void Scene::pushbackControlSnapshot(ControlSnapshot* control)
{
    ControlSnapshotMap::iterator it = m_ControlSnapshotMap.find(control->ulTimestamp);
    if (it == m_ControlSnapshotMap.end())
    {
        m_ControlSnapshotMap[control->ulTimestamp] = control;
    }
    else
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
ControlSnapshot* Scene::retrieveControlSnapshotOnTimestamp(u2uint64 timestamp)
{
    // find the first control snapshot which latter than timestamp
    for (ControlSnapshotMap::iterator it = m_ControlSnapshotMap.begin(); 
    it != m_ControlSnapshotMap.end(); it++)
    {
        if (it->first >= timestamp)
        {
            return it->second;
        }
    }

    // if all control snapshots are earlier than timestamp, return the last one
    if (!m_ControlSnapshotMap.empty())
    {
        return m_ControlSnapshotMap.rbegin()->second;
    }

    assert(0);
    return nullptr;
}
//-----------------------------------------------------------------------
void Scene::eraseControlSnapshotBeforeTimestamp(u2uint64 timestamp)
{
    for (ControlSnapshotMap::iterator it = m_ControlSnapshotMap.begin();
    it != m_ControlSnapshotMap.end(); )
    {
        if (it->first < timestamp)
        {
            U2_DELETE it->second;
            m_ControlSnapshotMap.erase(it++);
        }
        else
        {
            break;
        }
    }
}
//-----------------------------------------------------------------------
const Scene::ControlSnapshotMap& Scene::retrieveAllControlSnapshots()
{
    return m_ControlSnapshotMap;
}
//-----------------------------------------------------------------------
GameObject* Scene::createGameObject(const String& type, const String& name, const String& guid)
{
    GameObject* pGameObj = GameObjectManager::getSingleton().createObject(type, name, guid);
    if (pGameObj != nullptr)
    {
        m_GameObjectMap[pGameObj->getGuid()] = pGameObj;
    }
    return pGameObj;
}
//-----------------------------------------------------------------------
void Scene::destroyGameObject(GameObject* gameObj)
{
    m_GameObjectMap.erase(gameObj->getGuid());
    GameObjectManager::getSingleton().destoryObject(gameObj);
}
//-----------------------------------------------------------------------
void Scene::start()
{
    u2::FrameListenerCollection::getSingleton().addFrameListener(this
        , std::bind(&Scene::_onUpdate, this, std::placeholders::_1));
}
//-----------------------------------------------------------------------
void Scene::end()
{
    u2::FrameListenerCollection::getSingleton().removeFrameListener(this);
}
//-----------------------------------------------------------------------
void Scene::_onUpdate(float dt)
{
    _updateGameObjWithSnapshot();
}
//-----------------------------------------------------------------------
void Scene::_updateGameObjWithSnapshot()
{
    // remove redundant game objects
    GameObjectMap originGameObjMap = m_GameObjectMap;
    for (GameObjectMap::iterator it = originGameObjMap.begin();
    it != originGameObjMap.end(); it++)
    {
        GameObjMoveableSnapshotMap::iterator itSS = m_Scene.find(it->first);
        if (itSS == m_Scene.end())
        {
            destroyGameObject(it->second);
        }
    }

    // create new game object
    for (GameObjMoveableSnapshotMap::iterator it = m_Scene.begin(); 
    it != m_Scene.end(); it++)
    {
        GameObjectMap::iterator itGameObj = m_GameObjectMap.find(it->first);
        if (itGameObj == m_GameObjectMap.end())
        {
            MoveableSnapshotMap& moveableSnapshotMap = it->second;
            MovableSnapshot* pMovableSnapshot = moveableSnapshotMap.begin()->second;
            GameObject* pGameObj = createGameObject(
                pMovableSnapshot->szGameObjType, BLANK, pMovableSnapshot->szGameObjGuid);
            if (pGameObj == nullptr)
            {
                assert(0);
            }
            else
            {
                _initGameObj(pGameObj, pMovableSnapshot);
            }
        }
    }
}