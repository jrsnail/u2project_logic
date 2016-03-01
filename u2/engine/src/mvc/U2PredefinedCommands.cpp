//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PredefinedCommands.h"

#include "U2Context.h"
#include "U2Mediator.h"
#include "U2Facade.h"
#include "U2PredefinedProxies.h"
#include "U2PredefinedPrerequisites.h"
#include "U2PredefinedMediators.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
DestoryContextCommand::DestoryContextCommand(const String& type, const String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
DestoryContextCommand::~DestoryContextCommand()
{
}
//-----------------------------------------------------------------------
void DestoryContextCommand::go(const Notification& notification)
{
    const void* pData = notification.getData();
    _destroyContext((u2::Context*)pData);
}
//-----------------------------------------------------------------------
void DestoryContextCommand::_destroyContext(u2::Context* context)
{
    if (context == nullptr)
    {
        return;
    }

    // destroy children
    u2::Context::ContextMapIterator it = context->getChildIterator();
    while (it.hasMoreElements())
    {
        _destroyContext(it.getNext());
    }

    // destroy mediator
    Mediator* pMediator = MediatorManager::getSingleton().retrieveObject(context->getMediatorName());
    if (pMediator != nullptr)
    {
        pMediator->end();
        Facade* pFacade = FacadeManager::getSingleton().retrieveObject(context->getFacadeName());
        if (pFacade != nullptr)
        {
            pFacade->removeMediator(context->getMediatorName());
        }
        MediatorManager::getSingleton().destoryObject(pMediator);
    }

    // destroy context
    u2::Context* pParent = context->getParent();
    if (pParent == nullptr)
    {
        pParent->removeChild(context);
    }
    ContextProxy& contextProxy = getFacade().retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy.erase(context);
    ContextManager::getSingleton().destoryObject(context);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TransCommand::TransCommand(const String& type, const String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
TransCommand::~TransCommand()
{
}
//-----------------------------------------------------------------------
void TransCommand::go(const Notification& notification)
{
    if (NTF_Predefined_Trans != notification.getName())
    {
        return;
    }
    typedef std::tuple<u2::Context*, ContextQueue::eTransType, u2::Context*>   TransCommandData;
    const TransCommandData* pData = static_cast<const TransCommandData*>(notification.getData());
    u2::Context* pFrom              = std::get<0>(*pData);
    ContextQueue::eTransType eType  = std::get<1>(*pData);
    u2::Context* pTo                = std::get<2>(*pData);
    _createMediator(pFrom, eType, pTo);
}
//-----------------------------------------------------------------------
void TransCommand::_createMediator(const u2::Context* from, ContextQueue::eTransType type, const u2::Context* to)
{
    // to mediator
    Mediator* pMediator = MediatorManager::getSingleton().createObject(to->getMediatorClass(), to->getMediatorName());
    if (pMediator)
    {
        getFacade().registerMediator(pMediator);
    }
    
    // trans mediator name
    NameGeneratorManager::getSingleton().registerNameGenerator("TransMediator");
    u2::String szTransName = NameGeneratorManager::getSingleton().generator("TransMediator");

    // trans mediator
    TransMediator* pTransMediator = dynamic_cast<TransMediator*>(
        MediatorManager::getSingleton().createObject(OT_TransMediator, szTransName));
    if (pTransMediator)
    {
        getFacade().registerMediator(pTransMediator);
        pTransMediator->startup(from, type, to);
    }
    
    // children
    u2::Context::ConstContextMapIterator it = to->getChildIterator();
    while (it.hasMoreElements())
    {
        _createMediator(to, ContextQueue::eTransType::TT_Overlay, it.getNext());
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
BackKeyCommand::BackKeyCommand(const String& type, const String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
BackKeyCommand::~BackKeyCommand()
{
}
//-----------------------------------------------------------------------
void BackKeyCommand::go(const Notification& notification)
{
    ContextProxy* pProxy = dynamic_cast<ContextProxy*>(ProxyManager::getSingleton().retrieveObject("ContextProxy"));
    if (pProxy == nullptr)
    {
        return;
    }

    ContextProxy::ContextQueueVectorIterator it = pProxy->getContextQueueIterator();
    while (it.hasMoreElements())
    {
        ContextQueue* pQueue = it.getNext();
        if (pQueue == nullptr)
        {
            continue;
        }
        u2::Context* pContext = pQueue->top();
        if (pContext == nullptr)
        {
            continue;
        }

        if (_dispatchBack(pContext))
        {
            break;
        }
    }
}
//-----------------------------------------------------------------------
bool BackKeyCommand::_dispatchBack(u2::Context* context)
{
    if (context == nullptr)
    {
        return false;
    }

    Mediator* pMediator = MediatorManager::getSingleton().retrieveObject(context->getMediatorName());
    if (pMediator == nullptr)
    {
        return false;
    }

    bool bCanEnd = pMediator->preEnd(true);
    if (bCanEnd)
    {
        getFacade().sendNotification(NTF_Predefined_DestroyContext, context);
    }
    else
    {
        u2::Context::ContextMapIterator it = context->getChildIterator();
        while (it.hasMoreElements())
        {
            return _dispatchBack(it.getNext());
        }
    }
    return bCanEnd;
}