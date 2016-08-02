#include "U2PredefinedSystems.h"

#include "U2GameObject.h"
#include "U2PredefinedComponents.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
RenderSystem::RenderSystem(const String& type, const String& name)
    : System(type, name)
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
        = GameObjectManager::getSingleton().retrieveAllGameObjectsByCompType(GET_OBJECT_TYPE(SpriteComponent));
    while (it.hasMoreElements())
    {
        GameObjectManager::StGameObjRef stGameObjRef = it.getNext();
        _execute(stGameObjRef.pGameObj, dt);
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
    GameObject::ComponentMapIterator it
        = gameObj->retrieveComponentsByType(GET_OBJECT_TYPE(SpriteComponent));
    while (it.hasMoreElements())
    {
        SpriteComponent* pSpriteComp = dynamic_cast<SpriteComponent*>(it.getNext());
        if (pSpriteComp == nullptr)
        {
            assert(0);
            continue;
        }
        PositionComponent* pPositionComp = dynamic_cast<PositionComponent*>(
            gameObj->retrieveComponentByType(GET_OBJECT_TYPE(PositionComponent)));
        if (pPositionComp == nullptr)
        {
            assert(0);
            continue;
        }

        if (pSpriteComp->getState() == Component::CS_Attaching 
            && pSpriteComp->pSprite->getParent() == nullptr)
        {
            pSpriteComp->pSprite = cocos2d::CCSprite::create(pSpriteComp->szFilename);
            pSpriteComp->setState(Component::CS_Active);

            pSpriteComp->pSprite->setPosition(pPositionComp->v2Pos);

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