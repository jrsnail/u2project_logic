#include "U2PredefinedComponents.h"

#include "tinyxml.h"
#include "U2XmlSerialize.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SpriteComponent::SpriteComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , pSprite(nullptr)
{
}
//-----------------------------------------------------------------------
SpriteComponent::SpriteComponent(const String& type, const String& name)
    : Component(type, name)
    , pSprite(nullptr)
{

}
//-----------------------------------------------------------------------
SpriteComponent::~SpriteComponent()
{
}
//-----------------------------------------------------------------------
void SpriteComponent::copy(const Component& src)
{
    Component::copy(src);

    const SpriteComponent& srcComp = dynamic_cast<const SpriteComponent&>(src);
    szFilename = srcComp.szFilename;
}
//-----------------------------------------------------------------------
bool SpriteComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do 
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        // parse every attribute
        const TiXmlElement* pFilenameElem = pCompElem->FirstChildElement("Filename");
        GET_ERROR_LINE_AND_BREAK(pFilenameElem, szError);
        const char* pszValue = pFilenameElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        szFilename = pszValue;

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PositionComponent::PositionComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , v2Pos(cocos2d::Vec2::ZERO)
{
}
//-----------------------------------------------------------------------
PositionComponent::PositionComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
PositionComponent::~PositionComponent()
{
}
//-----------------------------------------------------------------------
void PositionComponent::copy(const Component& src)
{
    Component::copy(src);

    const PositionComponent& srcComp = dynamic_cast<const PositionComponent&>(src);
    v2Pos = srcComp.v2Pos;
}
//-----------------------------------------------------------------------
bool PositionComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        // parse every attribute
        const TiXmlElement* pPositionElem = pCompElem->FirstChildElement("Position");
        GET_ERROR_LINE_AND_BREAK(pPositionElem, szError);
        const char* pszValue = pPositionElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        v2Pos = StringUtil::parseVector2(pszValue);

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
VelocityComponent::VelocityComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , v2Velocity(cocos2d::Vec2::ZERO)
{
}
//-----------------------------------------------------------------------
VelocityComponent::VelocityComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
VelocityComponent::~VelocityComponent()
{
}
//-----------------------------------------------------------------------
void VelocityComponent::copy(const Component& src)
{
    Component::copy(src);

    const VelocityComponent& srcComp = dynamic_cast<const VelocityComponent&>(src);
    v2Velocity = srcComp.v2Velocity;
}
//-----------------------------------------------------------------------
bool VelocityComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        // parse every attribute
        const TiXmlElement* pPositionElem = pCompElem->FirstChildElement("Velocity");
        GET_ERROR_LINE_AND_BREAK(pPositionElem, szError);
        const char* pszValue = pPositionElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        v2Velocity = StringUtil::parseVector2(pszValue);

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SpeedDirComponent::SpeedDirComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , v2Dir(cocos2d::Vec2::ZERO)
{
}
//-----------------------------------------------------------------------
SpeedDirComponent::SpeedDirComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
SpeedDirComponent::~SpeedDirComponent()
{
}
//-----------------------------------------------------------------------
void SpeedDirComponent::copy(const Component& src)
{
    Component::copy(src);

    const SpeedDirComponent& srcComp = dynamic_cast<const SpeedDirComponent&>(src);
    v2Dir = srcComp.v2Dir;
}
//-----------------------------------------------------------------------
bool SpeedDirComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        // parse every attribute
        const TiXmlElement* pDirElem = pCompElem->FirstChildElement("Dir");
        GET_ERROR_LINE_AND_BREAK(pDirElem, szError);
        const char* pszValue = pDirElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        v2Dir = StringUtil::parseVector2(pszValue);

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SpeedComponent::SpeedComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , fSpeed(0.0f)
    , fMaxSpeed(0.0f)
    , fMinSpeed(0.0f)
{
}
//-----------------------------------------------------------------------
SpeedComponent::SpeedComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
SpeedComponent::~SpeedComponent()
{
}
//-----------------------------------------------------------------------
void SpeedComponent::copy(const Component& src)
{
    Component::copy(src);

    const SpeedComponent& srcComp = dynamic_cast<const SpeedComponent&>(src);
    fSpeed = srcComp.fSpeed;
    fMaxSpeed = srcComp.fMaxSpeed;
    fMinSpeed = srcComp.fMinSpeed;
}
//-----------------------------------------------------------------------
bool SpeedComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        {
            // parse "Speed"
            const TiXmlElement* pSpeedElem = pCompElem->FirstChildElement("Speed");
            GET_ERROR_LINE_AND_BREAK(pSpeedElem, szError);
            const char* pszValue = pSpeedElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            fSpeed = StringUtil::parseReal(pszValue);
        }
        
        {
            // parse "MaxSpeed"
            const TiXmlElement* pMaxSpeedElem = pCompElem->FirstChildElement("MaxSpeed");
            GET_ERROR_LINE_AND_BREAK(pMaxSpeedElem, szError);
            const char* pszValue = pMaxSpeedElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            fMaxSpeed = StringUtil::parseReal(pszValue);
        }

        {
            // parse "MinSpeed"
            const TiXmlElement* pMinSpeedElem = pCompElem->FirstChildElement("MinSpeed");
            GET_ERROR_LINE_AND_BREAK(pMinSpeedElem, szError);
            const char* pszValue = pMinSpeedElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            fMinSpeed = StringUtil::parseReal(pszValue);
        }
        
    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HpComponent::HpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , m_uHp(0)
    , m_uMaxHp(0)
    , m_uMinHp(0)
{
}
//-----------------------------------------------------------------------
HpComponent::HpComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
HpComponent::~HpComponent()
{
}
//-----------------------------------------------------------------------
void HpComponent::copy(const Component& src)
{
    Component::copy(src);

    const HpComponent& srcComp = dynamic_cast<const HpComponent&>(src);
    m_uHp = srcComp.m_uHp;
    m_uMaxHp = srcComp.m_uMaxHp;
    m_uMinHp = srcComp.m_uMinHp;
}
//-----------------------------------------------------------------------
bool HpComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        {
            // parse base
            const TiXmlElement* pElem = pCompElem->FirstChildElement("Hp");
            GET_ERROR_LINE_AND_BREAK(pElem, szError);
            const char* pszValue = pElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_uHp = StringUtil::parseUnsignedInt(pszValue);
        }

        {
            // parse max
            const TiXmlElement* pMaxElem = pCompElem->FirstChildElement("MaxHp");
            GET_ERROR_LINE_AND_BREAK(pMaxElem, szError);
            const char* pszValue = pMaxElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_uMaxHp = StringUtil::parseUnsignedInt(pszValue);
        }

        {
            // parse min
            const TiXmlElement* pMinElem = pCompElem->FirstChildElement("MinHp");
            GET_ERROR_LINE_AND_BREAK(pMinElem, szError);
            const char* pszValue = pMinElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_uMinHp = StringUtil::parseUnsignedInt(pszValue);
        }

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
BaseHpComponent::BaseHpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , m_uBaseHp(0)
    , m_uBaseMaxHp(0)
    , m_uBaseMinHp(0)
{
}
//-----------------------------------------------------------------------
BaseHpComponent::BaseHpComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
BaseHpComponent::~BaseHpComponent()
{
}
//-----------------------------------------------------------------------
void BaseHpComponent::copy(const Component& src)
{
    Component::copy(src);

    const BaseHpComponent& srcComp = dynamic_cast<const BaseHpComponent&>(src);
    m_uBaseHp = srcComp.m_uBaseHp;
    m_uBaseMaxHp = srcComp.m_uBaseMaxHp;
    m_uBaseMinHp = srcComp.m_uBaseMinHp;
}
//-----------------------------------------------------------------------
bool BaseHpComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        {
            // parse base
            const TiXmlElement* pElem = pCompElem->FirstChildElement("BaseHp");
            GET_ERROR_LINE_AND_BREAK(pElem, szError);
            const char* pszValue = pElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_uBaseHp = StringUtil::parseUnsignedInt(pszValue);
        }

        {
            // parse max
            const TiXmlElement* pMaxElem = pCompElem->FirstChildElement("BaseMaxHp");
            GET_ERROR_LINE_AND_BREAK(pMaxElem, szError);
            const char* pszValue = pMaxElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_uBaseMaxHp = StringUtil::parseUnsignedInt(pszValue);
        }

        {
            // parse min
            const TiXmlElement* pMinElem = pCompElem->FirstChildElement("BaseMinHp");
            GET_ERROR_LINE_AND_BREAK(pMinElem, szError);
            const char* pszValue = pMinElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_uBaseMinHp = StringUtil::parseUnsignedInt(pszValue);
        }

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
DeltaHpComponent::DeltaHpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
    , m_nDeltaHp(0)
    , m_nDeltaMaxHp(0)
    , m_nDeltaMinHp(0)
{
}
//-----------------------------------------------------------------------
DeltaHpComponent::DeltaHpComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
DeltaHpComponent::~DeltaHpComponent()
{
}
//-----------------------------------------------------------------------
void DeltaHpComponent::copy(const Component& src)
{
    Component::copy(src);

    const DeltaHpComponent& srcComp = dynamic_cast<const DeltaHpComponent&>(src);
    m_nDeltaHp = srcComp.m_nDeltaHp;
    m_nDeltaMaxHp = srcComp.m_nDeltaMaxHp;
    m_nDeltaMinHp = srcComp.m_nDeltaMinHp;
}
//-----------------------------------------------------------------------
bool DeltaHpComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    const TiXmlElement* pCompElem = compElem;
    String& szError = error;

    do
    {
        const char* pszCompType = pCompElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszCompType, szError);

        if (isPrototype())
        {
        }
        else
        {
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
        }

        {
            // parse base
            const TiXmlElement* pElem = pCompElem->FirstChildElement("DeltaHp");
            GET_ERROR_LINE_AND_BREAK(pElem, szError);
            const char* pszValue = pElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_nDeltaHp = StringUtil::parseInt(pszValue);
        }

        {
            // parse max
            const TiXmlElement* pMaxElem = pCompElem->FirstChildElement("DeltaMaxHp");
            GET_ERROR_LINE_AND_BREAK(pMaxElem, szError);
            const char* pszValue = pMaxElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_nDeltaMaxHp = StringUtil::parseInt(pszValue);
        }

        {
            // parse min
            const TiXmlElement* pMinElem = pCompElem->FirstChildElement("DeltaMinHp");
            GET_ERROR_LINE_AND_BREAK(pMinElem, szError);
            const char* pszValue = pMinElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_nDeltaMinHp = StringUtil::parseInt(pszValue);
        }

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
JoystickComponent::JoystickComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, isManual, loader)
{
}
//-----------------------------------------------------------------------
JoystickComponent::JoystickComponent(const String& type, const String& name)
    : Component(type, name)
{

}
//-----------------------------------------------------------------------
JoystickComponent::~JoystickComponent()
{
}
//-----------------------------------------------------------------------
void JoystickComponent::copy(const Component& src)
{
    Component::copy(src);

    const JoystickComponent& srcComp = dynamic_cast<const JoystickComponent&>(src);
}
//-----------------------------------------------------------------------
bool JoystickComponent::_loadFromXml(const TiXmlElement* compElem, String& error)
{
    return true;
}