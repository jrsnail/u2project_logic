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
    /**
    * Constructor.
    */
    CocosViewComponent(const u2::String& type, const u2::String& name);

    /**
    * Virtual destructor.
    */
    virtual ~CocosViewComponent(void);

    //virtual const String& getUiName() const = 0;
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

    cocos2d::Node* seekNodeByName(const u2::String& name);
    cocos2d::Node* seekNodeByName(cocos2d::Node* root, const u2::String& name);

    bool isTransEnd() const;

protected:
    cocos2d::Action* createEnterAction();
    cocos2d::Action* createExitAction();
    void _runAction(cocos2d::FiniteTimeAction* action, const u2::String& actionName);
    cocostudio::timeline::ActionTimeline* _runTimeline(const u2::String csb, const u2::String& timelineName);
    cocostudio::timeline::ActionTimeline* _runTimeline(cocos2d::Node* pNode, const u2::String csb, const u2::String& timelineName);

protected:
    cocos2d::Node*      m_pRootNode;
    cocos2d::Node*      m_pParent;
    bool                m_bTransEnd;
};

#endif /* defined(__U2CocosViewComponent__) */
