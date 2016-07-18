//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2CocosViewComponent.h"

#include "U2CocosPrerequisites.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
static cocos2d::FiniteTimeAction* createFadeInAction()
{
    return cocos2d::FadeIn::create(2);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CocosViewComponent::CocosViewComponent(const String& type, const String& name)
    : ViewComponent(type, name)
    , m_pRootNode(nullptr)
    , m_pParent(nullptr)
    , m_bTransEnd(false)
    , m_eEnterTransAction(TA_None)
    , m_eExitTransAction(TA_None)
    , m_pTouchListener(nullptr)
    , m_pKeyboardListener(nullptr)
{ }
//-----------------------------------------------------------------------
CocosViewComponent::~CocosViewComponent(void)
{ }
//-----------------------------------------------------------------------
void CocosViewComponent::loadUi()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Loading);

    const String& szUiName = getUiName();

    String szBaseName;
    String szExtName;
    StringUtil::splitBaseFilename(getUiName(), szBaseName, szExtName);

    // create as node
    if (szUiName == BLANK)
    {
        m_pRootNode = cocos2d::Node::create();
    }
    // create as csb
    else if (szExtName == "csb")
    {
        m_pRootNode = cocos2d::CSLoader::createNode(szUiName);
        if (m_pRootNode != nullptr)
        {
            cocos2d::ui::Helper::doLayout(m_pRootNode);
        }
    }
    else
    {
        // create as sprite frame
        if (cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(szUiName))
        {
            m_pRootNode = cocos2d::Sprite::createWithSpriteFrameName(szUiName);
        }
        // create as image
        if (m_pRootNode == nullptr)
        {
            m_pRootNode = cocos2d::Sprite::create(szUiName);
        }
    }
    
    // here also means a simple node
    if (m_pRootNode == nullptr)
    {
        assert(0);
    }
    m_pRootNode->retain();

    onUiLoaded();
}
//-----------------------------------------------------------------------
void CocosViewComponent::onUiLoaded()
{
    // add button listener and language replace
    _initAllWidget(m_pRootNode);
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
        String szActionName = typeid(*pAction).name();
        if (szActionName == typeid(cocostudio::timeline::ActionTimeline).name())
        {
            cocostudio::timeline::ActionTimeline* pTimeline = (cocostudio::timeline::ActionTimeline*)pAction;
            pTimeline->setLastFrameCallFunc([=](){
                onEntered();
            });
            m_pRootNode->runAction(pTimeline);
        }
        else
        {
            cocos2d::CallFunc* pFunc = cocos2d::CCCallFunc::create([=]{
                onEntered();
            });
            cocos2d::Sequence* pSequence = cocos2d::Sequence::create((cocos2d::FiniteTimeAction*)pAction, pFunc, nullptr);
            m_pRootNode->runAction(pSequence);
        }
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::onEntered()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Entered);

    m_bTransEnd = true;

    m_pRootNode->setVisible(true);
    m_pRootNode->setPosition(m_InitPos);

    // send view component created notification
    std::tuple<String, String, String> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName);
    getFacade().sendNotification(NTF_Predefined_ViewComp_Created, &data);
}
//-----------------------------------------------------------------------
cocos2d::Action* CocosViewComponent::createEnterAction()
{
    switch (m_eEnterTransAction)
    {
    case TA_FadeIn:
        return createFadeInAction();
        break;
    default:
        break;
    }

    return nullptr;
}
//-----------------------------------------------------------------------
void CocosViewComponent::exit()
{
    _changeViewCompState(ViewComponent::ViewCompState::VCS_Exiting);

    onWillExit();

    m_bTransEnd = false;

    // remove event listeners
    cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    if (pDispatcher != nullptr)
    {
        if (m_pTouchListener != nullptr)
        {
            pDispatcher->removeEventListener(m_pTouchListener);
        }
        if (m_pKeyboardListener != nullptr)
        {
            pDispatcher->removeEventListener(m_pKeyboardListener);
        }
    }

    // exit animation
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
    switch (m_eExitTransAction)
    {
    default:
        break;
    }

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

        m_pRootNode->setVisible(false);
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
void* CocosViewComponent::getSelf() const
{
    return m_pRootNode;
}
//-----------------------------------------------------------------------
void CocosViewComponent::setEnterTransAction(eTransAction trans)
{
    m_eEnterTransAction = trans;
}
//-----------------------------------------------------------------------
void CocosViewComponent::setExitTransAction(eTransAction trans)
{
    m_eExitTransAction = trans;
}
//-----------------------------------------------------------------------
void CocosViewComponent::_runAction(cocos2d::FiniteTimeAction* action, const String& actionName)
{
    cocos2d::CallFunc *pCallFunc = cocos2d::CallFunc::create([=](){
        _onActionEnd(actionName);
    });
    cocos2d::Sequence *pSeq = cocos2d::Sequence::create(action, pCallFunc, NULL);
    m_pRootNode->runAction(pSeq);
}
//-----------------------------------------------------------------------
cocostudio::timeline::ActionTimeline* CocosViewComponent::_runTimeline(const String csb, const String& timelineName)
{
    return _runTimeline(m_pRootNode, csb, timelineName);
}
//-----------------------------------------------------------------------
cocostudio::timeline::ActionTimeline* CocosViewComponent::_runTimeline(cocos2d::Node* pNode, const String csb, const String& timelineName)
{
    cocostudio::timeline::ActionTimeline* pTimeline = cocos2d::CSLoader::createTimeline(csb);
    pTimeline->setLastFrameCallFunc([=](){
        _onActionEnd(timelineName);
    });
    pNode->runAction(pTimeline);
    return pTimeline;
}
//-----------------------------------------------------------------------
void CocosViewComponent::_initAllWidget(cocos2d::Node* pWidget)
{
    // init current widget
    _initWidget(pWidget);

    const cocos2d::Vector<cocos2d::Node*>& children = pWidget->getChildren();
    ssize_t nCount = pWidget->getChildrenCount();
    // init all children widgets
    for (ssize_t i = 0; i < nCount; i++)
    {
        _initAllWidget(children.at(i));
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::_initWidget(cocos2d::Node* pWidget)
{
    if (pWidget == nullptr)
    {
        return;
    }

    String szWidgetId = typeid(*pWidget).name();
    // button
    if (szWidgetId == typeid(cocos2d::ui::Button).name())
    {
        cocos2d::ui::Button* pButton = static_cast<cocos2d::ui::Button*>(pWidget);
        if (pButton != nullptr)
        {
            pButton->addClickEventListener(
                std::bind(&CocosViewComponent::_onButtonClicked, this, std::placeholders::_1)
                );
        }
    }
    // text
    else if (szWidgetId == typeid(cocos2d::ui::Text).name())
    {
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onButtonClicked(cocos2d::Ref *ref)
{
    cocos2d::ui::Button *pButton = dynamic_cast<cocos2d::ui::Button*>(ref);
    if (pButton != nullptr)
    {
        std::tuple<String, String, String, String> data
            = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, pButton->getName());
        getFacade().sendNotification(NTF_Predefined_Button_Clicked, &data);
    }
    else
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::registerEventListenerWithFixedPri(cocos2d::EventListener::Type type, int fixedPriority)
{
    switch (type)
    {
    case cocos2d::EventListener::Type::TOUCH_ONE_BY_ONE:
    {
        if (m_pTouchListener != nullptr)
        {
            assert(0);
            return;
        }
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        if (pDispatcher != nullptr)
        {
            cocos2d::EventListenerTouchOneByOne* pListener = cocos2d::EventListenerTouchOneByOne::create();
            pListener->onTouchBegan = CC_CALLBACK_2(CocosViewComponent::_onTouchBegan, this);
            pListener->onTouchMoved = CC_CALLBACK_2(CocosViewComponent::_onTouchMoved, this);
            pListener->onTouchEnded = CC_CALLBACK_2(CocosViewComponent::_onTouchEnd, this);
            pListener->onTouchCancelled = CC_CALLBACK_2(CocosViewComponent::_onTouchCancelled, this);
            pDispatcher->addEventListenerWithFixedPriority(pListener, fixedPriority);
            m_pTouchListener = pListener;
        }
        break;
    }
    case cocos2d::EventListener::Type::TOUCH_ALL_AT_ONCE:
    {
        if (m_pTouchListener != nullptr)
        {
            assert(0);
            return;
        }
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        if (pDispatcher != nullptr)
        {
            cocos2d::EventListenerTouchAllAtOnce* pListener = cocos2d::EventListenerTouchAllAtOnce::create();
            pListener->onTouchesBegan = CC_CALLBACK_2(CocosViewComponent::_onTouchesBegan, this);
            pListener->onTouchesMoved = CC_CALLBACK_2(CocosViewComponent::_onTouchesMoved, this);
            pListener->onTouchesEnded = CC_CALLBACK_2(CocosViewComponent::_onTouchesEnded, this);
            pListener->onTouchesCancelled = CC_CALLBACK_2(CocosViewComponent::_onTouchesCancelled, this);
            pDispatcher->addEventListenerWithFixedPriority(pListener, fixedPriority);
            m_pTouchListener = pListener;
        }
        break;
    }
    case cocos2d::EventListener::Type::KEYBOARD:
    {
        if (m_pKeyboardListener != nullptr)
        {
            assert(0);
            return;
        }
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        if (pDispatcher != nullptr)
        {
            cocos2d::EventListenerKeyboard* pListener = cocos2d::EventListenerKeyboard::create();
            pListener->onKeyPressed = CC_CALLBACK_2(CocosViewComponent::_onKeyPressed, this);
            pListener->onKeyReleased = CC_CALLBACK_2(CocosViewComponent::_onKeyReleased, this);
            pDispatcher->addEventListenerWithFixedPriority(pListener, fixedPriority);
            m_pKeyboardListener = pListener;
        }
        break;
    }
    default:
        assert(0);
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::registerEventListenerWithSceneGraphPri(cocos2d::EventListener::Type type)
{
    switch (type)
    {
    case cocos2d::EventListener::Type::TOUCH_ONE_BY_ONE:
    {
        if (m_pTouchListener != nullptr)
        {
            assert(0);
            return;
        }
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        if (pDispatcher != nullptr)
        {
            cocos2d::EventListenerTouchOneByOne* pListener = cocos2d::EventListenerTouchOneByOne::create();
            pListener->onTouchBegan = CC_CALLBACK_2(CocosViewComponent::_onTouchBegan, this);
            pListener->onTouchMoved = CC_CALLBACK_2(CocosViewComponent::_onTouchMoved, this);
            pListener->onTouchEnded = CC_CALLBACK_2(CocosViewComponent::_onTouchEnd, this);
            pListener->onTouchCancelled = CC_CALLBACK_2(CocosViewComponent::_onTouchCancelled, this);
            pDispatcher->addEventListenerWithSceneGraphPriority(pListener, m_pRootNode);
            m_pTouchListener = pListener;
        }
        break;
    }
    case cocos2d::EventListener::Type::TOUCH_ALL_AT_ONCE:
    {
        if (m_pTouchListener != nullptr)
        {
            assert(0);
            return;
        }
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        if (pDispatcher != nullptr)
        {
            cocos2d::EventListenerTouchAllAtOnce* pListener = cocos2d::EventListenerTouchAllAtOnce::create();
            pListener->onTouchesBegan = CC_CALLBACK_2(CocosViewComponent::_onTouchesBegan, this);
            pListener->onTouchesMoved = CC_CALLBACK_2(CocosViewComponent::_onTouchesMoved, this);
            pListener->onTouchesEnded = CC_CALLBACK_2(CocosViewComponent::_onTouchesEnded, this);
            pListener->onTouchesCancelled = CC_CALLBACK_2(CocosViewComponent::_onTouchesCancelled, this);
            pDispatcher->addEventListenerWithSceneGraphPriority(pListener, m_pRootNode);
            m_pTouchListener = pListener;
        }
        break;
    }
    case cocos2d::EventListener::Type::KEYBOARD:
    {
        if (m_pKeyboardListener != nullptr)
        {
            assert(0);
            return;
        }
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        if (pDispatcher != nullptr)
        {
            cocos2d::EventListenerKeyboard* pListener = cocos2d::EventListenerKeyboard::create();
            pListener->onKeyPressed = CC_CALLBACK_2(CocosViewComponent::_onKeyPressed, this);
            pListener->onKeyReleased = CC_CALLBACK_2(CocosViewComponent::_onKeyReleased, this);
            pDispatcher->addEventListenerWithSceneGraphPriority(pListener, m_pRootNode);
            m_pKeyboardListener = pListener;
        }
        break;
    }
    default:
        assert(0);
    }
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev)
{
    std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, touches);
    getFacade().sendNotification(NTF_Predefined_Touches_Began, &data);
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev)
{
    std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, touches);
    getFacade().sendNotification(NTF_Predefined_Touches_Moved, &data);
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev)
{
    std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, touches);
    getFacade().sendNotification(NTF_Predefined_Touches_Ended, &data);
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev)
{
    std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, touches);
    getFacade().sendNotification(NTF_Predefined_Touches_Cancelled, &data);
}
//-----------------------------------------------------------------------
bool CocosViewComponent::_onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev)
{
    return false;
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev)
{

}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* ev)
{

}
//-----------------------------------------------------------------------
void CocosViewComponent::_onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev)
{

}
//-----------------------------------------------------------------------
void CocosViewComponent::_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* ev)
{
    std::tuple<String, String, String, cocos2d::EventKeyboard::KeyCode> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, keyCode);
    getFacade().sendNotification(NTF_Predefined_KeyPressed, &data);
}
//-----------------------------------------------------------------------
void CocosViewComponent::_onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* ev)
{
    std::tuple<String, String, String, cocos2d::EventKeyboard::KeyCode> data
        = std::make_tuple(this->getGuid(), this->getName(), m_szUiName, keyCode);
    getFacade().sendNotification(NTF_Predefined_KeyReleased, &data);
}
//-----------------------------------------------------------------------
void CocosViewComponent::centerRootNode()
{
    const cocos2d::Size& winSize = cocos2d::Director::getInstance()->getWinSize();
    m_InitPos = cocos2d::Vec2(winSize.width * 0.5, winSize.height * 0.5);
}