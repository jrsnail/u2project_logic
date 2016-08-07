//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Object.h"

#include "U2NameGenerator.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Object::Object(const String& type, const String& name)
    : m_szType(type)
    , m_szName(name)
    , m_bDeserializeSucceed(true)
{
    NameGeneratorManager::getSingleton().registerNameGenerator(m_szType);
    m_szGuid = NameGeneratorManager::getSingleton().generator(m_szType);

    if (m_szName == BLANK)
    {
        m_szName = m_szGuid;
    }
}
//-----------------------------------------------------------------------
Object::~Object()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ReusableObject::ReusableObject(const String& type)
    : Object(type)
	, m_bUsing(false)
{
}
//-----------------------------------------------------------------------
ReusableObject::~ReusableObject()
{
}