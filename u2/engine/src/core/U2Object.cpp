//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Object.h"

#include "U2NameGenerator.h"
#include "U2StringUtil.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Object::Object(const String& type, const String& name, const String& guid)
    : m_szType(type)
    , m_szName(name)
    , m_szGuid(guid)
    , m_bDeserializeSucceed(true)
{
    if (m_szGuid == BLANK)
    {
        NameGeneratorManager::getSingleton().registerNameGenerator("Auto_" + m_szType);
        m_szGuid = NameGeneratorManager::getSingleton().generator("Auto_" + m_szType);
    }
    else
    {
        // with user defined guid, it could not start with "Auto_"
        if (StringUtil::startsWith(m_szGuid, "Auto_"))
        {
            assert(0);
        }
    }

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
ReusableObject::ReusableObject(const String& type, const String& name, const String& guid)
    : Object(type, name, guid)
	, m_bUsing(false)
{
}
//-----------------------------------------------------------------------
ReusableObject::~ReusableObject()
{
}
//-----------------------------------------------------------------------
void ReusableObject::renameAsGuid(const String& guid)
{
    // with user defined guid, it could not start with "Auto_"
    if (StringUtil::startsWith(guid, "Auto_"))
    {
        assert(0);
    }
    m_szGuid = guid;
}