//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2ContextQueue.h"

#include "U2PredefinedPrerequisites.h"
#include "U2PredefinedFacade.h"
#include "U2PredefinedMediators.h"
#include "U2PredefinedProxies.h"
#include "U2Context.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ContextQueue::ContextQueue(const String& type, const String& name)
    : Object(type, name)
    , m_pContextProxy(nullptr)
{

}
//-----------------------------------------------------------------------
ContextQueue::~ContextQueue()
{
}
//-----------------------------------------------------------------------
u2::Context* ContextQueue::top()
{
    Queue::iterator it = m_queue.begin();
    return (it == m_queue.end()) ? nullptr : *it;
}
//-----------------------------------------------------------------------
bool ContextQueue::hasContext(const u2::String& name)
{
    Context* pContext = ContextManager::getSingleton().retrieveObjectByName(name);
    return hasContext(pContext);
}
//-----------------------------------------------------------------------
bool ContextQueue::hasContext(const u2::Context* context)
{
    if (context == nullptr)
    {
        return false;
    }

    for (Queue::iterator it = m_queue.begin(); it != m_queue.end(); it++)
    {
        if (*it == context)
        {
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------------
void ContextQueue::erase(const u2::String& name)
{
    Context* pContext = ContextManager::getSingleton().retrieveObjectByName(name);
    erase(pContext);
}
//-----------------------------------------------------------------------
void ContextQueue::erase(const u2::Context* context)
{
    if (context == nullptr)
    {
        return;
    }

    for (Queue::iterator it = m_queue.begin(); it != m_queue.end(); it++)
    {
        if (*it == context)
        {
            m_queue.erase(it);
            return;
        }
    }
}
//-----------------------------------------------------------------------
void ContextQueue::_switch(u2::Context* from, eTransType transType, u2::Context* to)
{
    if (m_pContextProxy != nullptr)
    {
        m_pContextProxy->_switch(from, transType, to);
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SingleContextQueue::SingleContextQueue(const String& type, const String& name)
    : ContextQueue(type, name)
{

}
//-----------------------------------------------------------------------
SingleContextQueue::~SingleContextQueue()
{
}
//-----------------------------------------------------------------------
void SingleContextQueue::initialize(eTransType defaultTransType, eBackKeyPriority priority, ContextProxy* contextProxy)
{
    m_eDefaultTransType = defaultTransType;
    m_eCapacity = eCapacity::Cap_1;
    m_ePriority = priority;
    m_pContextProxy = contextProxy;
}
//-----------------------------------------------------------------------
void SingleContextQueue::pushBack(u2::Context* context, eTransType transType)
{
    u2::Context* pFrom = nullptr;
    u2::Context* pTo = nullptr;

    size_t uSize = m_queue.size();
    if (uSize > (size_t)m_eCapacity)
    {
        assert(0);
    }

    if (uSize == (size_t)m_eCapacity)
    {
        pFrom = m_queue.front();
        m_queue.pop_front();
    }

    m_queue.push_back(context);
    pTo = m_queue.front();

    _switch(pFrom, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, pTo);
}
//-----------------------------------------------------------------------
void SingleContextQueue::pushFront(u2::Context* context, eTransType transType)
{
    u2::Context* pFrom = nullptr;

    size_t uSize = m_queue.size();
    if (uSize > (size_t)m_eCapacity)
    {
        assert(0);
    }

    if (uSize == (size_t)m_eCapacity)
    {
        pFrom = m_queue.front();
        m_queue.pop_front();
    }

    m_queue.push_front(context);
    u2::Context* pTo = m_queue.front();

    _switch(pFrom, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, pTo);
}
//-----------------------------------------------------------------------
void SingleContextQueue::pop(eTransType transType)
{
    u2::Context* pFrom = nullptr;
    u2::Context* pTo = nullptr;

    size_t uSize = m_queue.size();
    if (uSize > (size_t)m_eCapacity)
    {
        assert(0);
    }

    if (uSize == (size_t)m_eCapacity)
    {
        pFrom = m_queue.front();
        m_queue.pop_front();
    }

    uSize = m_queue.size();
    if (uSize > 0)
    {
        pTo = m_queue.front();
    }
    
    _switch(pFrom, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, pTo);
}
//-----------------------------------------------------------------------
void SingleContextQueue::replace(u2::Context* from, u2::Context* to, eTransType transType)
{
    for (Queue::iterator it = m_queue.begin(); it != m_queue.end(); it++)
    {
        if (*it == from)
        {
            *it = to;
            _switch(from, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, to);
            break;
        }
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
InfiniteContextQueue::InfiniteContextQueue(const String& type, const String& name)
    : ContextQueue(type, name)
{

}
//-----------------------------------------------------------------------
InfiniteContextQueue::~InfiniteContextQueue()
{
}
//-----------------------------------------------------------------------
void InfiniteContextQueue::initialize(eTransType defaultTransType, eBackKeyPriority priority, ContextProxy* contextProxy)
{
    m_eDefaultTransType = defaultTransType;
    m_eCapacity = eCapacity::Cap_Infinite;
    m_ePriority = priority;
    m_pContextProxy = contextProxy;
}
//-----------------------------------------------------------------------
void InfiniteContextQueue::pushBack(u2::Context* context, eTransType transType)
{
    u2::Context* pFrom = nullptr;
    u2::Context* pTo = nullptr;

    size_t uSize = m_queue.size();
    if (uSize == 0)
    {
        m_queue.push_back(context);
        pTo = m_queue.front();
        _switch(pFrom, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, pTo);
    }
    else
    {
        m_queue.push_back(context);
    }
}
//-----------------------------------------------------------------------
void InfiniteContextQueue::pushFront(u2::Context* context, eTransType transType)
{
    u2::Context* pFrom = nullptr;
    u2::Context* pTo = nullptr;

    size_t uSize = m_queue.size();
    if (uSize > 0)
    {
        pFrom = m_queue.front();
    }

    m_queue.push_front(context);
    pTo = m_queue.front();

    _switch(pFrom, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, pTo);
}
//-----------------------------------------------------------------------
void InfiniteContextQueue::pop(eTransType transType)
{
    u2::Context* pFrom = nullptr;
    u2::Context* pTo = nullptr;

    size_t uSize = m_queue.size();
    if (uSize > 0)
    {
        pFrom = m_queue.front();
        m_queue.pop_front();
    }

    uSize = m_queue.size();
    if (uSize > 0)
    {
        pTo = m_queue.front();
    }
    
    _switch(pFrom, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, pTo);
}
//-----------------------------------------------------------------------
void InfiniteContextQueue::replace(u2::Context* from, u2::Context* to, eTransType transType)
{
    for (Queue::iterator it = m_queue.begin(); it != m_queue.end(); it++)
    {
        if (*it == from)
        {
            *it = to;
            _switch(from, (transType == eTransType::TT_None) ? m_eDefaultTransType : transType, to);
            break;
        }
    }
}