//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "ApplicationViewComponents.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ShadeViewComponent::ShadeViewComponent(const u2::String& type, const u2::String& name)
    : CocosViewComponent(type, name)
{
}
//-----------------------------------------------------------------------
ShadeViewComponent::~ShadeViewComponent(void)
{

}
//-----------------------------------------------------------------------
u2::String ShadeViewComponent::getUiName() const
{
    static u2::String name = BLANK;
    return name;
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
//-----------------------------------------------------------------------
const u2::String LogoViewComponent::AN_LogoAction = "LogoAction";
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LogoViewComponent::LogoViewComponent(const u2::String& type, const u2::String& name)
    : CocosViewComponent(type, name)
{
}
//-----------------------------------------------------------------------
LogoViewComponent::~LogoViewComponent(void)
{

}
//-----------------------------------------------------------------------
u2::String LogoViewComponent::getUiName() const
{
    static u2::String name = BLANK;
    return name;
}
//-----------------------------------------------------------------------
void LogoViewComponent::loadUi()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Loading);

    // create logo
    cocos2d::Sprite *pSprite = cocos2d::Sprite::create("cmlogo.jpg");
    if (NULL != pSprite)
    {
        m_pRootNode = pSprite;
        m_pRootNode->retain();

        const cocos2d::Size& winSize = cocos2d::Director::getInstance()->getWinSize();

        pSprite->setPosition(winSize.width * 0.5, winSize.height * 0.5);
        pSprite->setOpacity(0);
    }

    onUiLoaded();
}
//-----------------------------------------------------------------------
void LogoViewComponent::runLogoAction()
{
    cocos2d::FadeIn *pFadeIn = cocos2d::FadeIn::create(2);
    cocos2d::Sequence *pSeq = cocos2d::Sequence::create(pFadeIn, NULL);
    this->_runAction(pSeq, AN_LogoAction);
}