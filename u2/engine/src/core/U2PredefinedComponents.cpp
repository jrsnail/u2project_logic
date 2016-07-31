#include "U2PredefinedComponents.h"

#include "tinyxml.h"
#include "U2XmlSerialize.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SpriteComponent::SpriteComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
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
    m_szFilename = srcComp.m_szFilename;
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
        }

        // parse every attribute
        const TiXmlElement* pFilenameElem = pCompElem->FirstChildElement("Filename");
        GET_ERROR_LINE_AND_BREAK(pFilenameElem, szError);
        const char* pszValue = pFilenameElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        m_szFilename = pszValue;

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PositionComponent::PositionComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
    , m_v2Pos(cocos2d::Vec2::ZERO)
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
    m_v2Pos = srcComp.m_v2Pos;
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
        }

        // parse every attribute
        const TiXmlElement* pPositionElem = pCompElem->FirstChildElement("Position");
        GET_ERROR_LINE_AND_BREAK(pPositionElem, szError);
        const char* pszValue = pPositionElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        m_v2Pos = StringUtil::parseVector2(pszValue);

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
VelocityComponent::VelocityComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
    , m_v2Velocity(cocos2d::Vec2::ZERO)
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
    m_v2Velocity = srcComp.m_v2Velocity;
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
        }

        // parse every attribute
        const TiXmlElement* pPositionElem = pCompElem->FirstChildElement("Velocity");
        GET_ERROR_LINE_AND_BREAK(pPositionElem, szError);
        const char* pszValue = pPositionElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        m_v2Velocity = StringUtil::parseVector2(pszValue);

    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SpeedComponent::SpeedComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
    , m_fSpeed(0.0f)
    , m_fMaxSpeed(0.0f)
    , m_fMinSpeed(0.0f)
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
    m_fSpeed = srcComp.m_fSpeed;
    m_fMaxSpeed = srcComp.m_fMaxSpeed;
    m_fMinSpeed = srcComp.m_fMinSpeed;
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
        }

        {
            // parse "Speed"
            const TiXmlElement* pSpeedElem = pCompElem->FirstChildElement("Speed");
            GET_ERROR_LINE_AND_BREAK(pSpeedElem, szError);
            const char* pszValue = pSpeedElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_fSpeed = StringUtil::parseReal(pszValue);
        }
        
        {
            // parse "MaxSpeed"
            const TiXmlElement* pMaxSpeedElem = pCompElem->FirstChildElement("MaxSpeed");
            GET_ERROR_LINE_AND_BREAK(pMaxSpeedElem, szError);
            const char* pszValue = pMaxSpeedElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_fMaxSpeed = StringUtil::parseReal(pszValue);
        }

        {
            // parse "MinSpeed"
            const TiXmlElement* pMinSpeedElem = pCompElem->FirstChildElement("MinSpeed");
            GET_ERROR_LINE_AND_BREAK(pMinSpeedElem, szError);
            const char* pszValue = pMinSpeedElem->Attribute("value");
            GET_ERROR_LINE_AND_BREAK(pszValue, szError);
            m_fMinSpeed = StringUtil::parseReal(pszValue);
        }
        
    } while (0);

    return szError == BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HpComponent::HpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
    , m_uHp(0)
    , m_uMaxHp(0)
    , m_uMinHp(0)
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
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
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
    , m_uBaseHp(0)
    , m_uBaseMaxHp(0)
    , m_uBaseMinHp(0)
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
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
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Component(creator, type, handle, group, name, isManual, loader)
    , m_nDeltaHp(0)
    , m_nDeltaMaxHp(0)
    , m_nDeltaMinHp(0)
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
            const char* pszCompGuid = pCompElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszCompGuid, szError);
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