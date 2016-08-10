#include "GameSystems.h"

#include "GameComponents.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
RenderSystem::RenderSystem(const u2::String& type, const u2::String& name, const u2::String& guid)
    : System(type, name, guid)
{
}
//-----------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
}
//-----------------------------------------------------------------------
void RenderSystem::enter()
{

}
//-----------------------------------------------------------------------
void RenderSystem::exit()
{

}
//-----------------------------------------------------------------------
void RenderSystem::execute(u2real dt)
{
    GameObjectManager::CompRefMapIterator it 
        = GameObjectManager::getSingleton().retrieveAllGameObjectsByCompType("component_sprite");
    while (it.hasMoreElements())
    {
        
        GameObjectManager::StGameObjRef stGameObjRef = it.getNext();
        if (!stGameObjRef.pGameObj->isPrototype())
        {
            _execute(stGameObjRef.pGameObj, dt);
        }
    }
}
//-----------------------------------------------------------------------
void RenderSystem::pause()
{

}
//-----------------------------------------------------------------------
void RenderSystem::resume()
{

}
//-----------------------------------------------------------------------
void RenderSystem::_enter()
{

}
//-----------------------------------------------------------------------
void RenderSystem::_exit()
{

}
//-----------------------------------------------------------------------
void RenderSystem::_execute(GameObject* gameObj, u2real dt)
{
    // root game object
    bool bRootGameObj = (gameObj->retrieveParentGameObject() == nullptr);

    GameObject::ComponentMapIterator it
        = gameObj->retrieveComponentsByType("component_sprite");
    while (it.hasMoreElements())
    {
        SpriteComponent* pSpriteComp = dynamic_cast<SpriteComponent*>(it.getNext());
        if (pSpriteComp == nullptr)
        {
            assert(0);
            continue;
        }
        PositionComponent* pPositionComp = dynamic_cast<PositionComponent*>(
            gameObj->retrieveComponentByType("component_position"));
        if (pPositionComp == nullptr)
        {
            assert(0);
            continue;
        }

        if (pSpriteComp->getState() == Component::CS_Attaching 
            && pSpriteComp->pSprite == nullptr)
        {
            pSpriteComp->pSprite = cocos2d::CCSprite::create(pSpriteComp->szFilename);
            pSpriteComp->setState(Component::CS_Active);

            // set init position
            pSpriteComp->pSprite->setPosition(pPositionComp->v2Pos);

            // set init scale
            ScaleComponent* pScaleComp = dynamic_cast<ScaleComponent*>(
                gameObj->retrieveComponentByType("component_scale"));
            if (pScaleComp == nullptr)
            {
                assert(0);
            }
            else
            {
                pSpriteComp->pSprite->setScaleX(pScaleComp->v2Scale.x);
                pSpriteComp->pSprite->setScaleY(pScaleComp->v2Scale.y);
            }

            // attach to scene
            cocos2d::Scene* pScene = cocos2d::Director::getInstance()->getRunningScene();
            if (pScene == nullptr)
            {
                assert(0);
            }
            else
            {
                pScene->addChild(pSpriteComp->pSprite);
            }
        }
        else if (pSpriteComp->getState() == Component::CS_Active)
        {
            pSpriteComp->pSprite->setPosition(pPositionComp->v2Pos);
        }
    }
}
//-----------------------------------------------------------------------
void RenderSystem::_pause()
{

}
//-----------------------------------------------------------------------
void RenderSystem::_resume()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
InputSystem::InputSystem(const u2::String& type, const u2::String& name, const u2::String& guid)
    : System(type, name, guid)
{
}
//-----------------------------------------------------------------------
InputSystem::~InputSystem()
{
}
//-----------------------------------------------------------------------
void InputSystem::enter()
{

}
//-----------------------------------------------------------------------
void InputSystem::exit()
{

}
//-----------------------------------------------------------------------
void InputSystem::execute(u2real dt)
{
    GameObjectManager::CompRefMapIterator it
        = GameObjectManager::getSingleton().retrieveAllGameObjectsByCompType("component_joystick");
    while (it.hasMoreElements())
    {
        GameObjectManager::StGameObjRef stGameObjRef = it.getNext();
        if (!stGameObjRef.pGameObj->isPrototype())
        {
            _execute(stGameObjRef.pGameObj, dt);
        }
    }
}
//-----------------------------------------------------------------------
void InputSystem::pause()
{

}
//-----------------------------------------------------------------------
void InputSystem::resume()
{

}
//-----------------------------------------------------------------------
void InputSystem::_enter()
{

}
//-----------------------------------------------------------------------
void InputSystem::_exit()
{

}
//-----------------------------------------------------------------------
void InputSystem::_execute(GameObject* gameObj, u2real dt)
{
    Component* pJoystickComp = gameObj->retrieveComponentByType("component_joystick");
    if (pJoystickComp != nullptr)
    {
        cocos2d::Vec2 v2Dir = cocos2d::Vec2::ZERO;
        bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryVec2Data("joystick_dir", v2Dir);
        if (!bSuc)
        {
            //assert(0);
        }

        float fSpeedRate = 0.0f;
        bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryFloatData("joystick_speed_rate", fSpeedRate);
        if (!bSuc)
        {
            //assert(0);
        }

        SpeedDirComponent* pSpeedDirComp = dynamic_cast<SpeedDirComponent*>(
            gameObj->retrieveComponentByType("component_speed_dir"));
        if (pSpeedDirComp == nullptr)
        {
            assert(0);
        }
        else
        {
            pSpeedDirComp->v2Dir = v2Dir;
        }

        SpeedComponent* pSpeedComp = dynamic_cast<SpeedComponent*>(
            gameObj->retrieveComponentByType("component_speed"));
        if (pSpeedComp == nullptr)
        {
            assert(0);
        }
        else
        {
            pSpeedComp->fSpeed = (pSpeedComp->fMaxSpeed - pSpeedComp->fMinSpeed) * fSpeedRate;
        }
    }
}
//-----------------------------------------------------------------------
void InputSystem::_pause()
{

}
//-----------------------------------------------------------------------
void InputSystem::_resume()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
MoveSystem::MoveSystem(const u2::String& type, const u2::String& name, const u2::String& guid)
    : System(type, name, guid)
{
}
//-----------------------------------------------------------------------
MoveSystem::~MoveSystem()
{
}
//-----------------------------------------------------------------------
void MoveSystem::enter()
{

}
//-----------------------------------------------------------------------
void MoveSystem::exit()
{

}
//-----------------------------------------------------------------------
void MoveSystem::execute(u2real dt)
{
    GameObjectManager::CompRefMapIterator it
        = GameObjectManager::getSingleton().retrieveAllGameObjectsByCompType("component_velocity");
    while (it.hasMoreElements())
    {
        GameObjectManager::StGameObjRef stGameObjRef = it.getNext();
        if (!stGameObjRef.pGameObj->isPrototype())
        {
            _execute(stGameObjRef.pGameObj, dt);
        }
    }
}
//-----------------------------------------------------------------------
void MoveSystem::pause()
{

}
//-----------------------------------------------------------------------
void MoveSystem::resume()
{

}
//-----------------------------------------------------------------------
void MoveSystem::_enter()
{

}
//-----------------------------------------------------------------------
void MoveSystem::_exit()
{

}
//-----------------------------------------------------------------------
void MoveSystem::_execute(GameObject* gameObj, u2real dt)
{
    SpeedDirComponent* pSpeedDirComp = dynamic_cast<SpeedDirComponent*>(
        gameObj->retrieveComponentByType("component_speed_dir"));
    if (pSpeedDirComp == nullptr)
    {
        assert(0);
        return;
    }

    SpeedComponent* pSpeedComp = dynamic_cast<SpeedComponent*>(
        gameObj->retrieveComponentByType("component_speed"));
    if (pSpeedComp == nullptr)
    {
        assert(0);
        return;
    }

    VelocityComponent* pVelocityComp = dynamic_cast<VelocityComponent*>(
        gameObj->retrieveComponentByType("component_velocity"));
    if (pVelocityComp == nullptr)
    {
        assert(0);
        return;
    }
    
    PositionComponent* pPositionComp = dynamic_cast<PositionComponent*>(
        gameObj->retrieveComponentByType("component_position"));
    if (pPositionComp == nullptr)
    {
        assert(0);
        return;
    }

    pVelocityComp->v2Velocity = pSpeedDirComp->v2Dir * pSpeedComp->fSpeed;
    pPositionComp->v2Pos = ccpAdd(pPositionComp->v2Pos, pVelocityComp->v2Velocity);
}
//-----------------------------------------------------------------------
void MoveSystem::_pause()
{

}
//-----------------------------------------------------------------------
void MoveSystem::_resume()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ScaleSystem::ScaleSystem(const u2::String& type, const u2::String& name, const u2::String& guid)
    : System(type, name, guid)
{
}
//-----------------------------------------------------------------------
ScaleSystem::~ScaleSystem()
{
}
//-----------------------------------------------------------------------
void ScaleSystem::enter()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::exit()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::execute(u2real dt)
{
    GameObjectManager::CompRefMapIterator it
        = GameObjectManager::getSingleton().retrieveAllGameObjectsByCompType("component_scale");
    while (it.hasMoreElements())
    {
        GameObjectManager::StGameObjRef stGameObjRef = it.getNext();
        if (!stGameObjRef.pGameObj->isPrototype())
        {
            _execute(stGameObjRef.pGameObj, dt);
        }
    }
}
//-----------------------------------------------------------------------
void ScaleSystem::pause()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::resume()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::_enter()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::_exit()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::_execute(GameObject* gameObj, u2real dt)
{
    ScaleComponent* pScaleComp = dynamic_cast<ScaleComponent*>(
        gameObj->retrieveComponentByType("component_scale"));
    if (pScaleComp == nullptr)
    {
        assert(0);
        return;
    }

    GameObject::ComponentMapIterator it
        = gameObj->retrieveComponentsByType("component_sprite");
    while (it.hasMoreElements())
    {
        SpriteComponent* pSpriteComp = dynamic_cast<SpriteComponent*>(it.getNext());
        if (pSpriteComp == nullptr)
        {
            assert(0);
            continue;
        }

        if (pSpriteComp->pSprite != nullptr)
        {
            pSpriteComp->pSprite->setScaleX(pScaleComp->v2Scale.x);
            pSpriteComp->pSprite->setScaleY(pScaleComp->v2Scale.y);
        }
    }
}
//-----------------------------------------------------------------------
void ScaleSystem::_pause()
{

}
//-----------------------------------------------------------------------
void ScaleSystem::_resume()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PredictSelfSystem::PredictSelfSystem(const u2::String& type, const u2::String& name, const u2::String& guid)
    : System(type, name, guid)
{
}
//-----------------------------------------------------------------------
PredictSelfSystem::~PredictSelfSystem()
{
}
//-----------------------------------------------------------------------
void PredictSelfSystem::enter()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::exit()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::execute(u2real dt)
{
    GameObjectManager::CompRefMapIterator it
        = GameObjectManager::getSingleton().retrieveAllGameObjectsByCompType("component_predict_self");
    while (it.hasMoreElements())
    {
        GameObjectManager::StGameObjRef stGameObjRef = it.getNext();
        if (!stGameObjRef.pGameObj->isPrototype())
        {
            _execute(stGameObjRef.pGameObj, dt);
        }
    }
}
//-----------------------------------------------------------------------
void PredictSelfSystem::pause()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::resume()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::_enter()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::_exit()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::_execute(GameObject* gameObj, u2real dt)
{
    PredictSelfComponent* pPredictSelfComp = dynamic_cast<PredictSelfComponent*>(
        gameObj->retrieveComponentByType("component_predict_self"));
    if (pPredictSelfComp == nullptr)
    {
        assert(0);
        return;
    }


    cocos2d::Vec2 v2Dir = cocos2d::Vec2::ZERO;
    bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryVec2Data("joystick_dir", v2Dir);
    if (!bSuc)
    {
        //assert(0);
    }

    float fSpeedRate = 0.0f;
    bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryFloatData("joystick_speed_rate", fSpeedRate);
    if (!bSuc)
    {
        //assert(0);
    }

    GameControlSnapshot snapshot;
    snapshot.v2Velocity = v2Dir * fSpeedRate;

    pPredictSelfComp->m_ControlSnapshotList.push_back(snapshot);
}
//-----------------------------------------------------------------------
void PredictSelfSystem::_pause()
{

}
//-----------------------------------------------------------------------
void PredictSelfSystem::_resume()
{

}