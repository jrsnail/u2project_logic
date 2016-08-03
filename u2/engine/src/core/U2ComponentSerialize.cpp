#include "U2ComponentSerialize.h"

#include "U2LogManager.h"
#include "tinyxml.h"
#include "U2Version.h"
#include "U2Component.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ComponentSerialize_0_0_1::ComponentSerialize_0_0_1(const String& type, const String& name)
    : XmlSerialize(type, name)
{
}
//-----------------------------------------------------------------------
ComponentSerialize_0_0_1::~ComponentSerialize_0_0_1()
{
}
//-----------------------------------------------------------------------
bool ComponentSerialize_0_0_1::load(const TiXmlElement* elem, const String& groupName)
{
    String szError = BLANK;
    do 
    {
        const TiXmlElement* pRootElem = elem;

        // check root elem and version
        const char* pszRootValue = pRootElem->Value();
        const char* pszVersion = pRootElem->Attribute("version");
        if (pszRootValue == nullptr || String(pszRootValue) != "PrototypeComponents")
        {
            break;
        }

        // parse every component
        for (const TiXmlElement* pCompElem = pRootElem->FirstChildElement("Component"); 
        pCompElem; pCompElem = pCompElem->NextSiblingElement("Component"))
        {
            if (!_loadComponent(pCompElem, szError, groupName))
            {
                break;
            }
        }
        

    } while (0);

    if (szError == BLANK)
    {
        return true;
    }
    else
    {
        LogManager::getSingleton().stream(LML_CRITICAL) << szError;
        return false;
    }
}
//-----------------------------------------------------------------------
bool ComponentSerialize_0_0_1::_loadComponent(const TiXmlElement* compElem, String& error
    , const String& groupName)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do 
    {
        if (pCompElem == nullptr)
        {
            break;
        }

        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);
        ComponentPtr spCompPtr = ComponentManager::getSingleton().create(pszCompType, groupName);
        if (!spCompPtr->_loadFromXml(pCompElem, szError))
        {
            ComponentManager::getSingleton().remove(spCompPtr->getHandle());
            break;
        }

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
bool ComponentSerialize_0_0_1::save()
{
    return false;
}