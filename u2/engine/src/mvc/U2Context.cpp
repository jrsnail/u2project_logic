//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Context.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Context::Context(const String& type, const String& name)
    : Object(type, name)
    , m_pParent(nullptr)
{

}
//-----------------------------------------------------------------------
Context::~Context()
{
}
//-----------------------------------------------------------------------
void Context::initialize(const u2::String& facadeName
    , const u2::String& mediatorClass, const u2::String& mediatorName
    , const u2::String& viewCompClass, const u2::String& viewCompName)
{
    m_szFacadeName = facadeName;
    m_szMediatorClass = mediatorClass;
    m_szMediatorName = mediatorName;
    m_szViewCompClass = viewCompClass;
    m_szViewCompName = viewCompName;
}
//-----------------------------------------------------------------------
Context* Context::createChild(const String& type, const String& name
    , const u2::String& facadeName
    , const u2::String& mediatorClass, const u2::String& mediatorName
    , const u2::String& viewCompClass, const u2::String& viewCompName)
{
    u2::Context* pChild = ContextManager::getSingleton().createObject(
        type, name
        , facadeName
        , mediatorClass, mediatorName
        , viewCompClass, viewCompName
        );
    this->addChild(pChild);
    pChild->setParent(this);
    return pChild;
}
//-----------------------------------------------------------------------
void Context::addChild(Context* child)
{
    if (child == nullptr)
    {
        return;
    }

    ContextMap::const_iterator it = m_children.find(child->getName());
    if (it == m_children.end())
    {
        m_children.insert(make_pair(child->getName(), child));
    }
}
//-----------------------------------------------------------------------
void Context::removeChild(Context* child)
{
    if (child == nullptr)
    {
        return;
    }

    ContextMap::iterator it = m_children.find(child->getName());
    if (it != m_children.end())
    {
        m_children.erase(it);
    }
}
//-----------------------------------------------------------------------
void Context::destroyChild(Context* child)
{
    if (child == nullptr)
    {
        return;
    }

    ContextMap::iterator it = m_children.find(child->getName());
    if (it != m_children.end())
    {
        m_children.erase(it);
        ContextManager::getSingleton().destoryObject(child);
    }
}
//-----------------------------------------------------------------------
Context::ConstContextMapIterator Context::getChildIterator() const
{
    return ConstContextMapIterator(m_children.begin(), m_children.end());
}
//-----------------------------------------------------------------------
Context::ContextMapIterator Context::getChildIterator()
{
    return ContextMapIterator(m_children.begin(), m_children.end());
}
//-----------------------------------------------------------------------
void Context::setParent(Context* parent)
{
    m_pParent = parent;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ContextManager* Singleton<ContextManager>::msSingleton = 0;
ContextManager* ContextManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ContextManager;
	}
	return msSingleton;
}
ContextManager& ContextManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ContextManager::ContextManager()
{
    CREATE_FACTORY(Context);
}
//-----------------------------------------------------------------------
ContextManager::~ContextManager()
{
}
//-----------------------------------------------------------------------
Context* ContextManager::createObject(const String& type, const String& name
    , const u2::String& facadeName
    , const u2::String& mediatorClass, const u2::String& mediatorName
    , const u2::String& viewCompClass, const u2::String& viewCompName)
{
    Context* pContext = createObject(type, name);
    pContext->initialize(facadeName
        , mediatorClass, mediatorName, viewCompClass, viewCompName);
    return pContext;
}
//-----------------------------------------------------------------------
Context* ContextManager::createObject(const String& type, const String& name)
{
    return SimpleObjectManager<Context>::createObject(type, name);
}