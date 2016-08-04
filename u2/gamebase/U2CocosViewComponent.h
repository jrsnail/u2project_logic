//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2CocosViewComponent__
#define __U2CocosViewComponent__


#include "U2Core.h"
#include "U2Mvc.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


U2EG_NAMESPACE_USING


class CocosViewComponent : public ViewComponent
{
public:
    enum eTransAction
    {
        TA_None,
        TA_FadeIn,
    };

public:
    /**
    * Constructor.
    */
    CocosViewComponent(const String& type, const String& name);

    /**
    * Virtual destructor.
    */
    virtual ~CocosViewComponent(void);

    virtual void loadUi() override;
    virtual void onUiLoaded() override;
    virtual void unloadUi() override;
    virtual void onWillUiUnload() override;
    virtual void enter() override;
    virtual void onEntered() override;
    virtual void exit() override;
    virtual void onWillExit() override;
    virtual void attach(void* parent) override;
    virtual void detach(void* parent) override;
    virtual void* getParent() const override;
    virtual void* getSelf() const override;

    cocos2d::Node* seekNodeByName(const String& name);
    cocos2d::Node* seekNodeByName(cocos2d::Node* root, const String& name);

    void setEnterTransAction(eTransAction trans);
    void setExitTransAction(eTransAction trans);
    bool isTransEnd() const;
    void centerRootNode();

    void registerEventListenerWithFixedPri(cocos2d::EventListener::Type type, int fixedPriority);
    void registerEventListenerWithSceneGraphPri(cocos2d::EventListener::Type type);

protected:
    virtual cocos2d::Action* createEnterAction();
    virtual cocos2d::Action* createExitAction();
    void _runAction(cocos2d::FiniteTimeAction* action, const String& actionName);
    cocostudio::timeline::ActionTimeline* _runTimeline(const String csb, const String& timelineName);
    cocostudio::timeline::ActionTimeline* _runTimeline(cocos2d::Node* pNode, const String csb, const String& timelineName);
    virtual void _onActionEnd(const String& actionName) {};

    void _initAllWidget(cocos2d::Node* pWidget);
    void _initWidget(cocos2d::Node* pWidget);
    void _onButtonClicked(cocos2d::Ref *ref);

    virtual void _onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev);
    virtual void _onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev);
    virtual void _onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev);
    virtual void _onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* ev);

    virtual bool _onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev);
    virtual void _onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev);
    virtual void _onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* ev);
    virtual void _onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev);

    virtual void _onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* ev);
    virtual void _onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* ev);

protected:
    cocos2d::Node*      m_pRootNode;
    cocos2d::Node*      m_pParent;
    bool                m_bTransEnd;
    eTransAction        m_eEnterTransAction;
    eTransAction        m_eExitTransAction;
    cocos2d::Vec2       m_InitPos;

    cocos2d::EventListener* m_pTouchListener;
    cocos2d::EventListener* m_pKeyboardListener;
};

#endif /* defined(__U2CocosViewComponent__) */
