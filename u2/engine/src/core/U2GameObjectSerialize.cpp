#include "U2GameObjectSerialize.h"

#include "tinyxml.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameObjectSerialize_0_0_1::GameObjectSerialize_0_0_1(const String& type, const String& name)
    : XmlSerialize(type, name)
{
}
//-----------------------------------------------------------------------
GameObjectSerialize_0_0_1::~GameObjectSerialize_0_0_1()
{
}
//-----------------------------------------------------------------------
bool GameObjectSerialize_0_0_1::load(const TiXmlElement* elem, const String& groupName)
{
    return false;
}
//-----------------------------------------------------------------------
bool GameObjectSerialize_0_0_1::save()
{
    return false;
}