//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2NameGenerator.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
template<> NameGeneratorManager* Singleton<NameGeneratorManager>::msSingleton = 0;
NameGeneratorManager* NameGeneratorManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new NameGeneratorManager;
	}
	return msSingleton;
}
NameGeneratorManager& NameGeneratorManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
NameGeneratorManager::NameGeneratorManager()
{
}
//-----------------------------------------------------------------------
NameGeneratorManager::~NameGeneratorManager()
{
    // Unload & delete resources in turn
    for (NameGeneratorMap::iterator itr = m_NameGenerators.begin(); itr != m_NameGenerators.end(); ++itr)
    {
        NameGenerator* pNameGenerator = itr->second;
        if (pNameGenerator != nullptr)
        {
            delete pNameGenerator;
            pNameGenerator = nullptr;
        }
    }
    // Empty the list
    m_NameGenerators.clear();
}
//-----------------------------------------------------------------------
void NameGeneratorManager::registerNameGenerator(const String& prefix)
{
    NameGeneratorMap::iterator itr = m_NameGenerators.find(prefix);
    if (itr == m_NameGenerators.end())
    {
        m_NameGenerators.insert(std::make_pair(prefix, new NameGenerator(prefix)));
    }
}
//-----------------------------------------------------------------------
void NameGeneratorManager::unregisterNameGenerator(const String& prefix)
{
    NameGeneratorMap::iterator itr = m_NameGenerators.find(prefix);
    if (itr != m_NameGenerators.end())
    {
        NameGenerator* pNameGenerator = itr->second;
        if (pNameGenerator != nullptr)
        {
            delete pNameGenerator;
            pNameGenerator = nullptr;
        }

        m_NameGenerators.erase(itr);
    }
}
//-----------------------------------------------------------------------
NameGenerator* NameGeneratorManager::getNameGenerator(const String& prefix) const
{
    NameGeneratorMap::const_iterator itr = m_NameGenerators.find(prefix);
    if (itr != m_NameGenerators.end())
    {
        return itr->second;
    }
    else
    {
        return nullptr;
    }
}
//-----------------------------------------------------------------------
String NameGeneratorManager::generator(const String& prefix)
{
    NameGenerator* pNameGenerator = getNameGenerator(prefix);
    if (pNameGenerator != nullptr)
    {
        return pNameGenerator->generate();
    }
    else
    {
        return prefix;
    }
}