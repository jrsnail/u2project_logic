//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PredefinedProxies.h"

#include "U2Context.h"
#include "U2PredefinedFacade.h"
#include "U2PredefinedPrerequisites.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ContextProxy::ContextProxy(const String& type, const String& name)
    : Proxy(type, name)
{
    // context queue factory
    CREATE_FACTORY(SingleContextQueue);
    CREATE_FACTORY(InfiniteContextQueue);
}
//-----------------------------------------------------------------------
ContextProxy::~ContextProxy(void)
{
}
//-----------------------------------------------------------------------
void ContextProxy::pushBack(const String& name, u2::Context* context, ContextQueue::eTransType transType)
{
    ContextQueue* pQueue = _retrieveContextQueue(name);
    if (pQueue)
    {
        pQueue->pushBack(context, transType);
    }
}
//-----------------------------------------------------------------------
void ContextProxy::pushFront(const String& name, u2::Context* context, ContextQueue::eTransType transType)
{
    ContextQueue* pQueue = _retrieveContextQueue(name);
    if (pQueue)
    {
        pQueue->pushFront(context, transType);
    }
}
//-----------------------------------------------------------------------
void ContextProxy::pop(const String& name, ContextQueue::eTransType transType)
{
    ContextQueue* pQueue = _retrieveContextQueue(name);
    if (pQueue)
    {
        pQueue->pop(transType);
    }
}
//-----------------------------------------------------------------------
void ContextProxy::replace(const String& name, u2::Context* from, u2::Context* to, ContextQueue::eTransType transType)
{
    ContextQueue* pQueue = _retrieveContextQueue(name);
    if (pQueue)
    {
        pQueue->replace(from, to, transType);
    }
}
//-----------------------------------------------------------------------
ContextQueue* ContextProxy::_retrieveContextQueue(const String& name)
{
    ContextQueue* pQueue = retrieveObjectByName(name);
    if (pQueue == nullptr)
    {
        assert(0);
    }
    return pQueue;
}
//-----------------------------------------------------------------------
void ContextProxy::erase(const u2::Context* context)
{
    if (context == nullptr)
    {
        return;
    }
    for (NamedObjectMap::iterator it = m_NamedMap.begin(); it != m_NamedMap.end(); it++)
    {
        ContextQueue* pQueue = it->second;
        if (pQueue == nullptr)
        {
            continue;
        }
        if (pQueue->hasContext(context))
        {
            pQueue->erase(context);
            break;
        }
    }
}
//-----------------------------------------------------------------------
void ContextProxy::_switch(u2::Context* from, ContextQueue::eTransType transType, u2::Context* to)
{
    std::tuple<u2::Context*, u2::ContextQueue::eTransType, u2::Context*> data
        = std::make_tuple(from, transType, to);
    getFacade().sendNotification(NTF_Predefined_Trans, &data);
}
//-----------------------------------------------------------------------
ContextQueue* ContextProxy::createContextQueue(const String& type, const String& name
    , ContextQueue::eTransType defaultTransType, ContextQueue::eBackKeyPriority priority)
{
    ContextQueue* pQueue = retrieveObjectByName(name);
    if (pQueue == nullptr)
    {
        pQueue = createObject(type, name);
        pQueue->initialize(defaultTransType, priority, this);
        for (PriorityQueueList::iterator it = m_priorityQueues.begin();
            it != m_priorityQueues.end(); 
            it++)
        {
            if ((*it)->getPriority() < priority)
            {
                m_priorityQueues.insert(it, pQueue);
                break;
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        assert(0);
    }
    return pQueue;
}
//-----------------------------------------------------------------------
ContextQueue* ContextProxy::createObject(const String& type, const String& name)
{
    return SimpleObjectManager<ContextQueue>::createObject(type, name);
}