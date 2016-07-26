#include "U2XmlSerialize.h"

#include "U2GameObjectSerialize.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
XmlSerialize::XmlSerialize(const String& type, const String& name)
    : Serialize(type, name)
{
    _initialize();
}
//-----------------------------------------------------------------------
XmlSerialize::~XmlSerialize()
{
}
//-----------------------------------------------------------------------
void XmlSerialize::_initialize()
{
    addTypeVersionPair("GameObjects", Version("0.0.1"), GET_OBJECT_TYPE(GameObjectSerialize));
}
//-----------------------------------------------------------------------
void XmlSerialize::addTypeVersionPair(const String& xmlRoot, const Version& version
    , const String& objType)
{
    m_TypeVersionMap.insert(make_pair(xmlRoot + "_" + version.getVersionNumAsStr(), objType));
}
//-----------------------------------------------------------------------
void XmlSerialize::load(const void* data)
{

}
//-----------------------------------------------------------------------
void XmlSerialize::save()
{

}