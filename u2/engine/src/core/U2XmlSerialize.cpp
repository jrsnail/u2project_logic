#include "U2XmlSerialize.h"

#include "U2ObjectCollection.h"
#include "tinyxml.h"
#include "U2ComponentSerialize.h"
#include "U2GameObjectSerialize.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
XmlSerialize::XmlSerialize(const String& type, const String& name)
    : Object(type, name)
{
}
//-----------------------------------------------------------------------
XmlSerialize::~XmlSerialize()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> XmlSerializeManager* Singleton<XmlSerializeManager>::msSingleton = 0;
XmlSerializeManager* XmlSerializeManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new XmlSerializeManager;
    }
    return msSingleton;
}
//-----------------------------------------------------------------------
XmlSerializeManager& XmlSerializeManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
XmlSerializeManager::XmlSerializeManager()
{
    _initialize();
}
//-----------------------------------------------------------------------
XmlSerializeManager::~XmlSerializeManager()
{
}
//-----------------------------------------------------------------------
void XmlSerializeManager::_initialize()
{
    addTypeVersionPair("PrototypeComponents", Version("0.0.1"), GET_OBJECT_TYPE(ComponentSerialize_0_0_1));
    addTypeVersionPair("PrototypeEntities", Version("0.0.1"), GET_OBJECT_TYPE(GameObjectSerialize_0_0_1));
}
//-----------------------------------------------------------------------
void XmlSerializeManager::addTypeVersionPair(const String& xmlRoot, const Version& version
    , const String& objType)
{
    m_TypeVersionMap.insert(make_pair(xmlRoot + "_" + version.getVersionNumAsStr(), objType));
}
//-----------------------------------------------------------------------
void XmlSerializeManager::load(InStreamPtr& stream, const String& groupName)
{
    String szError = BLANK;
    do 
    {
        // parse xml
        TiXmlDocument *pDoc = new TiXmlDocument;
        if (pDoc == nullptr)
        {
            break;
        }
        pDoc->Parse(stream->getAsString().c_str());

        // parse root element and version
        TiXmlElement *pRootElement = pDoc->RootElement();
        if (pRootElement == nullptr)
        {
            break;
        }
        String szRootValue = pRootElement->Value();
        String szVersion = pRootElement->Attribute("version");

        // find concrete serialize class
        TypeVersionMap::iterator it = m_TypeVersionMap.find(szRootValue + "_" + szVersion);
        if (it == m_TypeVersionMap.end())
        {
            szError = "could not find root and version in map.";
            break;
        }
        else
        {
            XmlSerialize* pObj = dynamic_cast<XmlSerialize*>(ObjectCollection::getSingletonPtr()->createObject(it->second));
            pObj->load(pRootElement, groupName);
            ObjectCollection::getSingletonPtr()->destoryObject(pObj);
        }
    } while (0);
    
    if (szError != BLANK)
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
void XmlSerializeManager::save()
{

}