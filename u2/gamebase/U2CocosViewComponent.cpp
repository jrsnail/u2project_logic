//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2CocosViewComponent.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CocosViewComponent::CocosViewComponent(const u2::String& type, const u2::String& name)
    : ViewComponent(type, name)
    , m_pRootNode(nullptr)
    , m_pParent(nullptr)
    , m_bTransEnd(false)
{ }
//-----------------------------------------------------------------------
CocosViewComponent::~CocosViewComponent(void)
{ }
//-----------------------------------------------------------------------
void CocosViewComponent::loadUi()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Loading);

    const u2::String& szCsbName = getUiName();
    m_pRootNode = cocos2d::CSLoader::createNode(szCsbName);
    m_pRootNode->retain();
    if (nullptr == m_pRootNode)
    {
        return;
    }
    cocos2d::ui::Helper::doLayout(m_pRootNode);

    onUiLoaded();
}
//-----------------------------------------------------------------------
void CocosViewComponent::onUiLoaded()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Loaded);
}
//-----------------------------------------------------------------------
void CocosViewComponent::unloadUi()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Unloading);

    if (m_pRootNode != nullptr)
    {
        m_pRootNode->release();
    }

    _changeViewCompState(ViewComponent::ViewCompState::VCS_Unloaded);
}
//-----------------------------------------------------------------------
void CocosViewComponent::onWillUiUnload()
{

}
//-----------------------------------------------------------------------
void CocosViewComponent::enter()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Entering);

    m_bTransEnd = false;
    cocos2d::Action* pAction = createEnterAction();
    if (pAction == nullptr)
    {
        onEntered();
    }
    else
    {
        if (typeid(*pAction) == typeid(cocos2d::FiniteTimeAction))
        {
            cocos2d::CallFunc* pFunc = cocos2d::CCCallFunc::create([=]{
                onEntered();
            });
            cocos2d::Sequence* pSequence = cocos2d::Sequence::create((cocos2d::FiniteTimeAction*)pAction, pFunc, nullptr);
            m_pRootNode->runAction(pSequence);
        }
        else if (typeid(*pAction) == typeid(cocostudio::timeline::ActionTimeline))
        {
            cocostudio::timeline::ActionTimeline* pTimeline = (cocostudio::timeline::ActionTimeline*)pAction;
            pTimeline->setLastFrameCallFunc([=](){
                onEntered();
            });
            m_pRootNode->runAction(pTimeline);
        }
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::onEntered()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Entered);

    m_bTransEnd = true;
}
//-----------------------------------------------------------------------
cocos2d::Action* CocosViewComponent::createEnterAction()
{
    return nullptr;
}
//-----------------------------------------------------------------------
void CocosViewComponent::exit()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Exiting);

    onWillExit();

    m_bTransEnd = false;

    cocos2d::Action* pAction = createExitAction();
    if (pAction == nullptr)
    {
        _changeViewCompState(ViewComponent::ViewCompState::VCS_Exited);
    }
    else 
    {
        if (typeid(*pAction) == typeid(cocos2d::FiniteTimeAction))
        {
            cocos2d::CallFunc* pFunc = cocos2d::CCCallFunc::create([=]{
                _changeViewCompState(ViewComponent::ViewCompState::VCS_Exited);
            });
            cocos2d::Sequence* pSequence = cocos2d::Sequence::create((cocos2d::FiniteTimeAction*)pAction, pFunc, nullptr);
            m_pRootNode->runAction(pSequence);
        }
        else if (typeid(*pAction) == typeid(cocostudio::timeline::ActionTimeline))
        {
            cocostudio::timeline::ActionTimeline* pTimeline = (cocostudio::timeline::ActionTimeline*)pAction;
            pTimeline->setLastFrameCallFunc([=](){
                _changeViewCompState(ViewComponent::ViewCompState::VCS_Exited);
            });
            m_pRootNode->runAction(pTimeline);
        }
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::onWillExit()
{

}
//-----------------------------------------------------------------------
cocos2d::Action* CocosViewComponent::createExitAction()
{
    return nullptr;
}
//-----------------------------------------------------------------------
void CocosViewComponent::attach(void* parent)
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Attaching);

    cocos2d::Node* pParent = (cocos2d::Node*)parent;
    if (pParent == nullptr)
    {
        assert(0);
    }
    else
    {
        pParent->addChild(m_pRootNode);
        m_pParent = pParent;
    }

    _changeViewCompState(ViewComponent::ViewCompState::VCS_Attached);
}
//-----------------------------------------------------------------------
void CocosViewComponent::detach(void* parent)
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Detaching);

    if (m_pRootNode != nullptr)
    {
        m_pRootNode->stopAllActions();
        m_pRootNode->removeFromParentAndCleanup(true);
    }

    _changeViewCompState(ViewComponent::ViewCompState::VCS_Detached);
}
//-----------------------------------------------------------------------
cocos2d::Node* CocosViewComponent::seekNodeByName(const String& name)
{
    return seekNodeByName(m_pRootNode, name);
}
//-----------------------------------------------------------------------
cocos2d::Node* CocosViewComponent::seekNodeByName(cocos2d::Node* root, const String& name)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    if (root->getName() == name)
    {
        return root;
    }
    const cocos2d::Vector<cocos2d::Node*>& arrayRootChildren = root->getChildren();
    for (cocos2d::Node* pSubWidget : arrayRootChildren)
    {
        if (pSubWidget != nullptr)
        {
            cocos2d::Node* pResult = seekNodeByName(pSubWidget, name);
            if (pResult != nullptr)
            {
                return pResult;
            }
        }
    }

    return nullptr;
}
//-----------------------------------------------------------------------
bool CocosViewComponent::isTransEnd() const
{
    return m_bTransEnd;
}
//-----------------------------------------------------------------------
void* CocosViewComponent::getParent() const
{
    return m_pParent;
}
//-----------------------------------------------------------------------
void CocosViewComponent::_runAction(cocos2d::FiniteTimeAction* action, const String& actionName)
{
    cocos2d::CallFunc *pCallFunc = cocos2d::CallFunc::create([=](){
        _emitCommonStateChange(actionName, "ActionEnd");
    });
    cocos2d::Sequence *pSeq = cocos2d::Sequence::create(action, pCallFunc, NULL);
    m_pRootNode->runAction(pSeq);
}
//-----------------------------------------------------------------------
cocostudio::timeline::ActionTimeline* CocosViewComponent::_runTimeline(const u2::String csb, const String& timelineName)
{
    return _runTimeline(m_pRootNode, csb, timelineName);
}
//-----------------------------------------------------------------------
cocostudio::timeline::ActionTimeline* CocosViewComponent::_runTimeline(cocos2d::Node* pNode, const u2::String csb, const String& timelineName)
{
    cocostudio::timeline::ActionTimeline* pTimeline = cocos2d::CSLoader::createTimeline(csb);
    pTimeline->setLastFrameCallFunc([=](){
        _emitCommonStateChange(timelineName, "ActionEnd");
    });
    pNode->runAction(pTimeline);
    return pTimeline;
}