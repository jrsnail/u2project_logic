//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Model.h"

#include "U2Proxy.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Model::Model(const String& type, const String& name)
    : Object(type, name)
{
	
}
//-----------------------------------------------------------------------
inline void Model::initializeModel(void)
{ }
//-----------------------------------------------------------------------
void Model::registerProxy(Proxy* proxy)
{
	proxy->initializeNotifier(m_szName);
	do
	{
		U2_LOCK_AUTO_MUTEX;
		ProxyMap::iterator result = m_ProxyMap.find(proxy->getName());
		m_ProxyMap.insert(std::make_pair(proxy->getName(), (proxy)));
	} while (false);
	proxy->onRegister();
}
//-----------------------------------------------------------------------
Proxy const& Model::retrieveProxy(const String& proxy_name) const
{
    return const_cast<Proxy const&>(static_cast<Model const&>(*this).retrieveProxy(proxy_name));
}
//-----------------------------------------------------------------------
inline Proxy& Model::retrieveProxy(const String& proxy_name)
{
    U2_LOCK_AUTO_MUTEX;

    ProxyMap::const_iterator result = m_ProxyMap.find(proxy_name);
    if (result == m_ProxyMap.end())
    {
        assert(0);
        //throwException<std::runtime_error>("Cannot find any proxy with name: [%s].", proxy_name.c_str());
    }

    return *result->second;
}
//-----------------------------------------------------------------------
inline bool Model::hasProxy(const String& proxy_name)
{
    U2_LOCK_AUTO_MUTEX;
	return m_ProxyMap.find(proxy_name) != m_ProxyMap.end();
}
//-----------------------------------------------------------------------
Proxy* Model::removeProxy(const String& proxy_name)
{
	ProxyMap::value_type::second_type proxy = NULL;

	do
	{
        U2_LOCK_AUTO_MUTEX;
		// Retrieve the named mediator
		ProxyMap::iterator result = m_ProxyMap.find(proxy_name);

		if (result == m_ProxyMap.end())
			break;

		// get mediator
		proxy = result->second;
		// remove the mediator from the map
		m_ProxyMap.erase(result);
	} while (false);

	if (proxy != NULL) proxy->onRemove();
	return proxy;
}
//-----------------------------------------------------------------------
void Model::removeModel(const String& name)
{
    Model* pObj = ModelManager::getSingleton().retrieveObject(name);
    if (pObj == nullptr)
    {
        return;
    }
    ModelManager::getSingleton().destoryObject(pObj);
}
//-----------------------------------------------------------------------
Model::ProxyNames Model::listProxyNames(void) const
{
    ProxyNames names;

    ModelManager::ObjectMapIterator mapIterator = ModelManager::getSingleton().getObjectIterator();
    while (mapIterator.hasMoreElements())
    {
        String szKey = mapIterator.peekNextKey();
        names.push_back(szKey);
        mapIterator.moveNext();
    }

    return names;
}
//-----------------------------------------------------------------------
Model::~Model(void)
{
	//removeModel(_multiton_key);
	m_ProxyMap.clear();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ModelManager* Singleton<ModelManager>::msSingleton = 0;
ModelManager* ModelManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ModelManager;
	}
	return msSingleton;
}
ModelManager& ModelManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ModelManager::ModelManager()
{
}
//-----------------------------------------------------------------------
ModelManager::~ModelManager()
{
}