//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "AppViewComponents.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ShadeViewComponent::ShadeViewComponent(const String& type, const String& name, const String& guid)
    : CocosViewComponent(type, name, guid)
{
}
//-----------------------------------------------------------------------
ShadeViewComponent::~ShadeViewComponent(void)
{

}
//-----------------------------------------------------------------------
void ShadeViewComponent::loadUi()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Loading);

    // clear color
    cocos2d::Director::getInstance()->setClearColor(cocos2d::Color4F::WHITE);

    // create a scene. it's an autorelease object
    cocos2d::Scene* pScene = cocos2d::Scene::create();

    const cocos2d::Size& winSize = cocos2d::Director::getInstance()->getWinSize();

    // create layer
    cocos2d::LayerColor* pLayer = cocos2d::LayerColor::create(cocos2d::Color4B(166, 166, 166, 166), winSize.width, winSize.height);
    if (nullptr != pLayer)
    {
        m_pRootNode = pLayer;

        pLayer->setPosition(0, 0);
    }

    // run
    cocos2d::Director::getInstance()->runWithScene(pScene);

    onUiLoaded();


    // delete it when use trans
    attach(pScene);
    enter();
}
//-----------------------------------------------------------------------
void ShadeViewComponent::unloadUi()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Unloading);

    _changeViewCompState(ViewComponent::ViewCompState::VCS_Unloaded);
}
//-----------------------------------------------------------------------
ViewComponent::NotificationNames ShadeViewComponent::listNotificationInterests(void) const
{
    return std::list<String>();
}
//-----------------------------------------------------------------------
void ShadeViewComponent::handleNotification(const Notification& notification)
{
    const String& szNtfName = notification.getName();
    const void* pData = notification.getData();

}
//-----------------------------------------------------------------------
void ShadeViewComponent::startup()
{
    getFacade().sendNotification(NTF_App_Trans2Battle);
}