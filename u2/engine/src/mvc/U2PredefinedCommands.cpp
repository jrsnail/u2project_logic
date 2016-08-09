//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PredefinedCommands.h"

#include "U2Context.h"
#include "U2Facade.h"
#include "U2PredefinedProxies.h"
#include "U2PredefinedPrerequisites.h"
#include "U2PredefinedMediators.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
DestoryContextCommand::DestoryContextCommand(const String& type, const String& name, const String& guid)
    : SimpleCommand(type, name, guid)
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


    // destroy view component
    ViewComponent* pViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(context->getViewCompName());
    if (pViewComp != nullptr)
    {
        //pViewComp->end();
        Facade* pFacade = FacadeManager::getSingletonPtr()->retrieveObjectByName(context->getFacadeName());
        if (pFacade != nullptr)
        {
            pFacade->removeViewComp(context->getViewCompName());
        }
        ViewComponentManager::getSingletonPtr()->destoryObject(pViewComp);
    }

    // destroy context
    u2::Context* pParent = context->getParent();
    if (pParent == nullptr)
    {
        pParent->removeChild(context);
    }
    ContextProxy* pContextProxy = getFacade().retrieveProxy<ContextProxy>(ON_Proxy_Context);
    pContextProxy->erase(context);
    ContextManager::getSingletonPtr()->destoryObject(context);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TransCommand::TransCommand(const String& type, const String& name, const String& guid)
    : SimpleCommand(type, name, guid)
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
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::Context* pFrom = std::get<0>(*pData);
        ContextQueue::eTransType eType = std::get<1>(*pData);
        u2::Context* pTo = std::get<2>(*pData);
        _createMediator(pFrom, nullptr, eType, pTo);
    }
}
//-----------------------------------------------------------------------
void TransCommand::_createMediator(const u2::Context* from, const u2::Context* parent
    , ContextQueue::eTransType type, const u2::Context* cur)
{
    // trans mediator name
    NameGeneratorManager::getSingletonPtr()->registerNameGenerator("TransMediator");
    String szTransName = NameGeneratorManager::getSingletonPtr()->generator("TransMediator");

    // trans mediator
    TransMediator* pTransMediator = dynamic_cast<TransMediator*>(
        TransMediatorManager::getSingletonPtr()->createObject(OT_TransMediator, szTransName));
    if (pTransMediator)
    {
        pTransMediator->startup(from, parent, type, cur);
    }
    
    // children
    u2::Context::ConstContextMapIterator it = cur->getChildIterator();
    while (it.hasMoreElements())
    {
        //_createMediator(to, ContextQueue::eTransType::TT_Overlay, it.getNext());
        //_createMediator(to, ContextQueue::eTransType::TT_Child, it.getNext());
        ContextQueue::eTransType childType = type;
        switch (type)
        {
        case ContextQueue::eTransType::TT_Overlay:
            childType = ContextQueue::eTransType::TT_Overlay_Child;
            break;
        case ContextQueue::eTransType::TT_OneByOne:
            childType = ContextQueue::eTransType::TT_OneByOne_Child;
            break;
        case ContextQueue::eTransType::TT_Cross:
            childType = ContextQueue::eTransType::TT_Cross_Child;
            break;
        }
        _createMediator(from, cur, childType, it.getNext());
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
BackKeyCommand::BackKeyCommand(const String& type, const String& name, const String& guid)
    : SimpleCommand(type, name, guid)
{
}
//-----------------------------------------------------------------------
BackKeyCommand::~BackKeyCommand()
{
}
//-----------------------------------------------------------------------
void BackKeyCommand::go(const Notification& notification)
{
    ContextProxy* pProxy = dynamic_cast<ContextProxy*>(ProxyManager::getSingletonPtr()->retrieveObjectByName(ON_Proxy_Context));
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
            cocos2d::Value data(pContext->getName());
            getFacade().sendNotification(NTF_Predefined_DestroyContext, &data);
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

    ViewComponent* pViewComp = ViewComponentManager::getSingletonPtr()->retrieveObjectByName(context->getViewCompName());
    if (pViewComp == nullptr)
    {
        return false;
    }

    bool bCanEnd = pViewComp->preEnd(true);
    if (bCanEnd)
    {
        // recursively checked children
        u2::Context::ContextMapIterator it = context->getChildIterator();
        while (it.hasMoreElements())
        {
            bCanEnd = _dispatchBack(it.getNext());
            // if anyone child could not be end
            if (!bCanEnd)
            {
                break;
            }
        }
    }
    return bCanEnd;
}