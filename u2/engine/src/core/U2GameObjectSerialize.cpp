#include "U2GameObjectSerialize.h"

#include "U2LogManager.h"
#include "U2GameObject.h"
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
    String szError = BLANK;
    do
    {
        const TiXmlElement* pRootElem = elem;

        // check root elem and version
        const char* pszRootValue = pRootElem->Value();
        const char* pszVersion = pRootElem->Attribute("version");
        if (pszRootValue == nullptr || String(pszRootValue) != "PrototypeGameObjects")
        {
            break;
        }

        // parse every component
        for (const TiXmlElement* pCompElem = pRootElem->FirstChildElement("GameObject");
        pCompElem; pCompElem = pCompElem->NextSiblingElement("GameObject"))
        {
            if (!_loadGameObject(pCompElem, szError, groupName))
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
bool GameObjectSerialize_0_0_1::_loadGameObject(const TiXmlElement* compElem, String& error
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

        const char* pszGameObjType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszGameObjType, szError);
        const char* pszGameObjGuid = pCompElem->Attribute("guid");
        GET_ERROR_LINE_AND_BREAK(pszGameObjGuid, szError);
        const char* pszGameObjName = pCompElem->Attribute("name");
        GET_ERROR_LINE_AND_BREAK(pszGameObjName, szError);
        GameObjectPtr spGameObjPtr = GameObjectManager::getSingleton().create(pszGameObjType, groupName);
        if (!spGameObjPtr->_loadFromXml(pCompElem, szError))
        {
            GameObjectManager::getSingleton().remove(spGameObjPtr->getHandle());
            break;
        }

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
bool GameObjectSerialize_0_0_1::save()
{
    return false;
}