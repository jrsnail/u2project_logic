//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PredefinedMediators.h"

#include "U2ViewComponent.h"
#include "U2Facade.h"
#include "U2Context.h"
#include "U2FrameListenerCollection.h"
#include "U2PredefinedPrerequisites.h"
#include "cocos2d.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TransMediator::TransMediator(const String& type, const String& name, const String& guid)
    : u2::Object(type, name, guid)
    , m_pFrom(nullptr)
    , m_pParent(nullptr)
    , m_pCur(nullptr)
    , m_pFromContext(nullptr)
    , m_pParentContext(nullptr)
    , m_pCurContext(nullptr)
    , m_pParentNode(nullptr)
{
    CREATE_FACTORY(VoidStep);
    CREATE_FACTORY(ParamStep);
}
//-----------------------------------------------------------------------
TransMediator::~TransMediator(void)
{

}
//-----------------------------------------------------------------------
void TransMediator::startup(const u2::Context* from, const u2::Context* parent, ContextQueue::eTransType type, const u2::Context* cur)
{
    if (from == nullptr && cur == nullptr)
    {
        assert(0);
    }

    m_pFromContext = from;
    m_pParentContext = parent;
    m_pCurContext = cur;

    ViewComponent* pFromViewComp = nullptr;
    if (from != nullptr)
    {
        pFromViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(from->getViewCompName());
        if (pFromViewComp == nullptr)
        {
            // here 'from' view component should not be null
            assert(0);
            pFromViewComp = ViewComponentManager::getSingletonPtr()->createObject(from->getViewCompClass(), from->getViewCompName());
        }
        pFromViewComp->addListener(this);
    }
    else
    {
        if (type == ContextQueue::eTransType::TT_Overlay)
        {
            const u2::Context* pParent = m_pCurContext->getParent();

            if (pParent && pParent->getName() != BLANK)
            {
                m_pFromContext = ContextManager::getSingletonPtr()->retrieveObjectByName(pParent->getName());
                if (m_pFromContext)
                {
                    pFromViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(m_pFromContext->getViewCompName());
                    pFromViewComp->addListener(this);
                }
            }
        }
    }

    ViewComponent* pParentViewComp = nullptr;
    if (parent != nullptr)
    {
        pParentViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(parent->getViewCompName());
        pParentViewComp->addListener(this);
    }

    ViewComponent* pCurViewComp = nullptr;
    if (cur != nullptr)
    {
        pCurViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(cur->getViewCompName());
        if (pCurViewComp == nullptr)
        {
            pCurViewComp = ViewComponentManager::getSingletonPtr()->createObject(cur->getViewCompClass(), cur->getViewCompName());
        }
        Facade* pFacade = FacadeManager::getSingletonPtr()->retrieveObjectByName(cur->getFacadeName());
        if (pFacade)
        {
            pFacade->registerViewComp(pCurViewComp);
        }
        pCurViewComp->addListener(this);
    }

    _trans(pFromViewComp, pParentViewComp, type, pCurViewComp);
    _registerFrameListener();
}
//-----------------------------------------------------------------------
void TransMediator::end()
{
    if (m_pParent != nullptr)
    {
        m_pParent->removeListener(this);
    }

    if (m_pParent != nullptr)
    {
        m_pParent->removeListener(this);
    }

    if (m_pCur != nullptr)
    {
        m_pCur->removeListener(this);
    }

    _unregisterFrameListener();
}
//-----------------------------------------------------------------------
void TransMediator::_registerFrameListener()
{
    FrameListenerCollection::getSingletonPtr()->addFrameListener(this, std::bind(&TransMediator::onUpdate, this, std::placeholders::_1));
}
//-----------------------------------------------------------------------
void TransMediator::_unregisterFrameListener()
{
    FrameListenerCollection::getSingletonPtr()->removeFrameListener(this);
}
//-----------------------------------------------------------------------
void TransMediator::_trans(ViewComponent* from, ViewComponent* parent, ContextQueue::eTransType type, ViewComponent* cur)
{
    m_pCur = cur;
    m_pParent = parent;
    m_pFrom = from;
    m_transType = type;

    if (cur == nullptr)
    {
        assert(0);
    }

    if (from == nullptr)
    {
        //         m_steps.push_back(_createVoidStep(TransStep::Key(m_pTo, ViewComponent::ViewCompState::VCS_Loaded), std::bind(&ViewComponent::attach, m_pTo)));
        //         m_steps.push_back(_createVoidStep(TransStep::Key(m_pTo, ViewComponent::ViewCompState::VCS_Attached), std::bind(&ViewComponent::enter, m_pTo)));
        m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&TransMediator::_onTransOver, this)));

        m_pCur->loadUi();
    }
    else
    {
        switch (type)
        {
        case ContextQueue::eTransType::TT_Overlay:
        {
            m_pParentNode = m_pFrom->getParent();

            m_steps.push_back(_createParamStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Loaded), std::bind(&ViewComponent::attach, m_pCur, std::placeholders::_1)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Attached), std::bind(&ViewComponent::enter, m_pCur)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&TransMediator::_onTransOver, this)));

            m_pCur->loadUi();
            break;
        }
        case ContextQueue::eTransType::TT_Overlay_Child:
        {
            m_pParentNode = m_pParent->getSelf();
            if (m_pParentNode == nullptr)
            {
                assert(0);
            }

            m_steps.push_back(_createParamStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Loaded), std::bind(&ViewComponent::attach, m_pCur, std::placeholders::_1)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Attached), std::bind(&ViewComponent::enter, m_pCur)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&TransMediator::_onTransOver, this)));

            m_pCur->loadUi();
            break;
        }
        case ContextQueue::eTransType::TT_OneByOne:
        {
            m_pParentNode = m_pFrom->getParent();

            m_steps.push_back(_createVoidStep(TransStep::Key(m_pFrom, ViewComponent::ViewCompState::VCS_Exited), std::bind(&TransMediator::_onOneByOneFromExited, this)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Attached), std::bind(&ViewComponent::enter, m_pCur)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&TransMediator::_onTransOver, this)));

            // delay call exit() int update()
            // m_pFrom->exit();
            break;
        }
        case ContextQueue::eTransType::TT_OneByOne_Child:
        {
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pFrom, ViewComponent::ViewCompState::VCS_Exited), std::bind(&TransMediator::_onOneByOneChildFromExited, this)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Attached), std::bind(&ViewComponent::enter, m_pCur)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&TransMediator::_onTransOver, this)));
            break;
        }
        case ContextQueue::eTransType::TT_Cross:
        {
            m_pParentNode = m_pFrom->getParent();

            m_steps.push_back(_createParamStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Loaded), std::bind(&ViewComponent::attach, m_pCur, std::placeholders::_1)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Attached), std::bind(&TransMediator::_onCrossToAttached, this)));
            m_steps.push_back(_createParamStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&ViewComponent::detach, m_pFrom, std::placeholders::_1)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pFrom, ViewComponent::ViewCompState::VCS_Detached), std::bind(&ViewComponent::unloadUi, m_pFrom)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pFrom, ViewComponent::ViewCompState::VCS_Unloaded), std::bind(&TransMediator::_onTransOver, this)));

            m_pCur->loadUi();
            break;
        }
        case ContextQueue::eTransType::TT_Cross_Child:
        {
            m_pParentNode = m_pParent->getSelf();
            if (m_pParentNode == nullptr)
            {
                assert(0);
            }

            m_steps.push_back(_createParamStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Loaded), std::bind(&ViewComponent::attach, m_pCur, std::placeholders::_1)));
            m_steps.push_back(_createVoidStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Attached), std::bind(&ViewComponent::enter, m_pCur)));
            m_steps.push_back(_createParamStep(TransStep::Key(m_pCur, ViewComponent::ViewCompState::VCS_Entered), std::bind(&TransMediator::_onTransOver, this)));

            m_pCur->loadUi();
            break;
        }
        default:
            assert(0);
            break;
        }
    }
}
//-----------------------------------------------------------------------
void TransMediator::_onOneByOneFromExited()
{
    m_pFrom->detach(m_pParentNode);
    m_pFrom->unloadUi();
    m_pCur->loadUi();
    m_pCur->attach(m_pParentNode);
}
//-----------------------------------------------------------------------
void TransMediator::_onOneByOneChildFromExited()
{
    m_pParentNode = m_pParent->getSelf();
    if (m_pParentNode == nullptr)
    {
        assert(0);
    }

    m_pCur->loadUi();
    m_pCur->attach(m_pParentNode);
}
//-----------------------------------------------------------------------
void TransMediator::_onCrossToAttached()
{
    m_pFrom->exit();
    m_pCur->enter();
}
//-----------------------------------------------------------------------
void TransMediator::_onTransOver()
{
    _destroyFromContext();
    _startupToContext(m_pCurContext);
    //_destroyTransMediator();
}
//-----------------------------------------------------------------------
void TransMediator::_destroyTransMediator()
{
    this->end();

    m_pFrom = nullptr;
    m_pParent = nullptr;
    m_pCur = nullptr;
    m_pParentContext = nullptr;
    m_pCurContext = nullptr;
    m_pParentNode = nullptr;
    m_KeyList.clear();

    TransMediatorManager::getSingletonPtr()->destoryObject(this);
}
//-----------------------------------------------------------------------
void TransMediator::_destroyFromContext()
{
    if (m_pFromContext != nullptr)
    {
        if (m_transType == ContextQueue::eTransType::TT_OneByOne
            || m_transType == ContextQueue::eTransType::TT_Cross)
        {
            Facade::broadcastNotification(NTF_Predefined_DestroyContext, m_pFromContext);
            m_pFromContext = nullptr;
            m_pFrom = nullptr;
        }
    }
}
//-----------------------------------------------------------------------
void TransMediator::_startupToContext(const u2::Context* context)
{
    if (context == nullptr)
    {
        return;
    }

    ViewComponent* pViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(context->getViewCompName());
    if (pViewComp != nullptr)
    {
        pViewComp->startup();
    }

//     u2::Context::ConstContextMapIterator it = context->getChildIterator();
//     while (it.hasMoreElements())
//     {
//         _startupToContext(it.getNext());
//     }
}
//-----------------------------------------------------------------------
TransStep* TransMediator::_createVoidStep(const TransStep::Key& key, VoidStep::CallbackFun func)
{
    VoidStep* pVoidStep = dynamic_cast<VoidStep*>(
        FactoryManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(VoidStep), BLANK, BLANK));
    if (pVoidStep != nullptr)
    {
        pVoidStep->initialize(key, func);
    }
    return pVoidStep;
}
//-----------------------------------------------------------------------
TransStep* TransMediator::_createParamStep(const TransStep::Key& key, ParamStep::CallbackFun func)
{
    ParamStep* pParamStep = dynamic_cast<ParamStep*>(
        FactoryManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(ParamStep), BLANK, BLANK));
    if (pParamStep != nullptr)
    {
        pParamStep->initialize(key, func);
    }
    return pParamStep;
}
//-----------------------------------------------------------------------
void TransMediator::_destroyTransStep(TransStep* step)
{
    FactoryManager::getSingletonPtr()->destroyObject(step);
}
//-----------------------------------------------------------------------
void TransMediator::onUpdate(float dt)
{
    if (m_pCur == nullptr)
    {
        return;
    }

    // we should delay call exit() of from view component in TT_OneByOne trans type, 
    // as child view component need listen exit event.
    if (m_transType == ContextQueue::eTransType::TT_OneByOne)
    {
        if (m_pFrom->getViewCompState() == ViewComponent::ViewCompState::VCS_Entered)
        {
            m_pFrom->exit();
        }
    }
    

    TransStepKeyList keyList = m_KeyList;
    for (TransStepKeyList::const_iterator it = keyList.begin();
        it != keyList.end(); it++)
    {
        const TransStep::Key& curKey = *it;

        std::vector<TransStep*>::iterator itStep = m_steps.begin();
        if (itStep != m_steps.end())
        {
            TransStep* pStep = *itStep;
            if (pStep->isThisStep(curKey))
            {
                String szType = pStep->getType();
                if (szType == GET_OBJECT_TYPE(VoidStep))
                {
                    VoidStep* pVoidStep = (VoidStep*)pStep;
                    pVoidStep->call();
                    m_steps.erase(itStep);
                    _destroyTransStep(pVoidStep);
                }
                else if (szType == GET_OBJECT_TYPE(ParamStep))
                {
                    ParamStep* pParamStep = (ParamStep*)pStep;
                    pParamStep->call(m_pParentNode);
                    m_steps.erase(itStep);
                    _destroyTransStep(pParamStep);
                }
                else
                {
                    assert(0);
                }
            }
        }
    }

    if (m_steps.size() == 0)
    {
        _destroyTransMediator();
    }
}
//-----------------------------------------------------------------------
void TransMediator::onViewCompStateChanged(ViewComponent* viewComp, ViewComponent::ViewCompState newState)
{
    m_KeyList.push_back(TransStep::Key(viewComp, newState));
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> TransMediatorManager* Singleton<TransMediatorManager>::msSingleton = 0;
TransMediatorManager* TransMediatorManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new TransMediatorManager;
    }
    return msSingleton;
}
//-----------------------------------------------------------------------
TransMediatorManager::TransMediatorManager()
{
}
//-----------------------------------------------------------------------
TransMediatorManager::~TransMediatorManager()
{
}