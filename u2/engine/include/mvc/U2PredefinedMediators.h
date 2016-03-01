//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2PredefinedMediators__
#define __U2PredefinedMediators__


#include "U2Core.h"
#include "U2Mediator.h"
#include "U2ContextQueue.h"
#include "U2TransStep.h"


U2EG_NAMESPACE_USING


class TransMediator : public Mediator
{
public:
    TransMediator(const String& type, const String& name);

    virtual ~TransMediator(void);

    virtual void startup(const u2::Context* from, ContextQueue::eTransType type, const u2::Context* to);

	virtual void end() override;

    virtual void onUpdate(float dt);

private:
    /// don't call this method in TransMediator
    virtual void startup(const u2::Context* context) override;

protected:
    virtual void onViewCompStateChanged(ViewComponent* viewComp, ViewComponent::ViewCompState newState) override;

    void _trans(ViewComponent* from, ContextQueue::eTransType type, ViewComponent* to);
    void _onCrossToAttached();
    void _onOneByOneFromExited();
    void _onTransOver();

    void _destroyTransMediator();
    void _destroyFromContext();
    void _startupToContext(const u2::Context* context);

    TransStep* _createVoidStep(const TransStep::Key& key, VoidStep::CallbackFun func = nullptr);
    TransStep* _createParamStep(const TransStep::Key& key, ParamStep::CallbackFun func = nullptr);
    void _destroyTransStep(TransStep* step);

protected:
    virtual void _registerFrameListener() override;
    virtual void _unregisterFrameListener() override;

protected:
    ViewComponent*      m_pFrom;
    ViewComponent*      m_pTo;
    const u2::Context*  m_pFromContext;
    const u2::Context*  m_pToContext;
    ContextQueue::eTransType           m_transType;
    void*               m_pParent;
    std::vector<TransStep*>      m_steps;
    TransStep::Key      m_curKey;
};


#endif /* defined(__U2PredefinedMediators__) */
