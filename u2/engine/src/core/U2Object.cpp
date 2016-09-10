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
    , m_bDeserializeSucceed(false)
    , m_bSerializeSucceed(false)
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
inline const String& Object::getType() const
{
    return m_szType;
}
//-----------------------------------------------------------------------
inline const String& Object::getName() const
{
    return m_szName;
}
//-----------------------------------------------------------------------
inline const String& Object::getGuid() const
{
    return m_szGuid;
}
//-----------------------------------------------------------------------
void Object::serialize()
{
}
//-----------------------------------------------------------------------
void Object::deserialize()
{
}
//-----------------------------------------------------------------------
inline bool Object::isDeserializeSucceed()
{
    return m_bDeserializeSucceed;
}
//-----------------------------------------------------------------------
inline bool Object::isSerializeSucceed()
{
    return m_bSerializeSucceed;
}
//-----------------------------------------------------------------------
inline void Object::setData(const vector<u2char>::type& data)
{
    m_Data = data;
}
//-----------------------------------------------------------------------
inline const vector<u2char>::type& Object::getData() const
{
    return m_Data;
}
//-----------------------------------------------------------------------
inline vector<u2char>::type& Object::getData()
{
    return m_Data;
}
//-----------------------------------------------------------------------
inline size_t Object::getDataSize()
{
    return m_Data.size();
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
void ReusableObject::renameAsName(const String& name)
{ 
    if (name != BLANK)
    {
        m_szName = name;
    }
}
//-----------------------------------------------------------------------
void ReusableObject::renameAsGuid(const String& guid)
{
    if (guid != BLANK)
    {
        // with user defined guid, it could not start with "Auto_"
        if (StringUtil::startsWith(guid, "Auto_"))
        {
            assert(0);
        }
        m_szGuid = guid;
    }
}