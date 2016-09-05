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
#include "U2ContextQueue.h"
#include "U2TransStep.h"


U2EG_NAMESPACE_USING


class TransMediator : public u2::Object, public ViewComponent::Listener, public GeneralAllocatedObject
{
public:
    TransMediator(const String& type, const String& name = BLANK, const String& guid = BLANK);

    virtual ~TransMediator(void);

    virtual void startup(const u2::Context* from, const u2::Context* parent
        , ContextQueue::eTransType type, const u2::Context* cur);

	virtual void end();

    virtual void onUpdate(float dt);

protected:
    virtual void onViewCompStateChanged(ViewComponent* viewComp, ViewComponent::ViewCompState newState) override;

    void _trans(ViewComponent* from, ViewComponent* parent, ContextQueue::eTransType type, ViewComponent* cur);
    void _onCrossToAttached();
    void _onOneByOneFromExited();
    void _onOneByOneChildFromExited();
    void _onTransOver();

    void _destroyTransMediator();
    void _destroyFromContext();
    void _startupToContext(const u2::Context* context);

    TransStep* _createVoidStep(const TransStep::Key& key, VoidStep::CallbackFun func = nullptr);
    TransStep* _createParamStep(const TransStep::Key& key, ParamStep::CallbackFun func = nullptr);
    void _destroyTransStep(TransStep* step);

protected:
    virtual void _registerFrameListener();
    virtual void _unregisterFrameListener();

protected:
    ViewComponent*      m_pFrom;
    ViewComponent*      m_pParent;
    ViewComponent*      m_pCur;
    const u2::Context*  m_pFromContext;
    const u2::Context*  m_pParentContext;
    const u2::Context*  m_pCurContext;
    ContextQueue::eTransType           m_transType;
    void*               m_pParentNode;
    std::vector<TransStep*>      m_steps;

    typedef std::vector<TransStep::Key>     TransStepKeyList;
    TransStepKeyList    m_KeyList;
};



class TransMediatorManager : public SimpleObjectManager<TransMediator>, public Singleton < TransMediatorManager >
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    TransMediatorManager();

public:
    /** Default destructor.
    */
    virtual ~TransMediatorManager();

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static TransMediatorManager* getSingletonPtr(void);
};


#endif /* defined(__U2PredefinedMediators__) */
