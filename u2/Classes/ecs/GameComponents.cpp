﻿#include "GameComponents.h"

#include "tinyxml.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SpriteComponent::SpriteComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , pSprite(nullptr)
{
}
//-----------------------------------------------------------------------
SpriteComponent::SpriteComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
    , pSprite(nullptr)
{

}
//-----------------------------------------------------------------------
SpriteComponent::~SpriteComponent()
{
}
//-----------------------------------------------------------------------
void SpriteComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const SpriteComponent& srcComp = dynamic_cast<const SpriteComponent&>(src);
    szFilename = srcComp.szFilename;
}
//-----------------------------------------------------------------------
bool SpriteComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
PositionComponent::PositionComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , v2Pos(cocos2d::Vec2::ZERO)
{
}
//-----------------------------------------------------------------------
PositionComponent::PositionComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
PositionComponent::~PositionComponent()
{
}
//-----------------------------------------------------------------------
void PositionComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const PositionComponent& srcComp = dynamic_cast<const PositionComponent&>(src);
    v2Pos = srcComp.v2Pos;
}
//-----------------------------------------------------------------------
bool PositionComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
VelocityComponent::VelocityComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , v2Velocity(cocos2d::Vec2::ZERO)
{
}
//-----------------------------------------------------------------------
VelocityComponent::VelocityComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
VelocityComponent::~VelocityComponent()
{
}
//-----------------------------------------------------------------------
void VelocityComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const VelocityComponent& srcComp = dynamic_cast<const VelocityComponent&>(src);
    v2Velocity = srcComp.v2Velocity;
}
//-----------------------------------------------------------------------
bool VelocityComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
SpeedDirComponent::SpeedDirComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , v2Dir(cocos2d::Vec2::ZERO)
{
}
//-----------------------------------------------------------------------
SpeedDirComponent::SpeedDirComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
SpeedDirComponent::~SpeedDirComponent()
{
}
//-----------------------------------------------------------------------
void SpeedDirComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const SpeedDirComponent& srcComp = dynamic_cast<const SpeedDirComponent&>(src);
    v2Dir = srcComp.v2Dir;
}
//-----------------------------------------------------------------------
bool SpeedDirComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
SpeedComponent::SpeedComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , fSpeed(0.0f)
    , fMaxSpeed(0.0f)
    , fMinSpeed(0.0f)
{
}
//-----------------------------------------------------------------------
SpeedComponent::SpeedComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
SpeedComponent::~SpeedComponent()
{
}
//-----------------------------------------------------------------------
void SpeedComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const SpeedComponent& srcComp = dynamic_cast<const SpeedComponent&>(src);
    fSpeed = srcComp.fSpeed;
    fMaxSpeed = srcComp.fMaxSpeed;
    fMinSpeed = srcComp.fMinSpeed;
}
//-----------------------------------------------------------------------
bool SpeedComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
HpComponent::HpComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , m_uHp(0)
    , m_uMaxHp(0)
    , m_uMinHp(0)
{
}
//-----------------------------------------------------------------------
HpComponent::HpComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
HpComponent::~HpComponent()
{
}
//-----------------------------------------------------------------------
void HpComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const HpComponent& srcComp = dynamic_cast<const HpComponent&>(src);
    m_uHp = srcComp.m_uHp;
    m_uMaxHp = srcComp.m_uMaxHp;
    m_uMinHp = srcComp.m_uMinHp;
}
//-----------------------------------------------------------------------
bool HpComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
BaseHpComponent::BaseHpComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , m_uBaseHp(0)
    , m_uBaseMaxHp(0)
    , m_uBaseMinHp(0)
{
}
//-----------------------------------------------------------------------
BaseHpComponent::BaseHpComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
BaseHpComponent::~BaseHpComponent()
{
}
//-----------------------------------------------------------------------
void BaseHpComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const BaseHpComponent& srcComp = dynamic_cast<const BaseHpComponent&>(src);
    m_uBaseHp = srcComp.m_uBaseHp;
    m_uBaseMaxHp = srcComp.m_uBaseMaxHp;
    m_uBaseMinHp = srcComp.m_uBaseMinHp;
}
//-----------------------------------------------------------------------
bool BaseHpComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
DeltaHpComponent::DeltaHpComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , m_nDeltaHp(0)
    , m_nDeltaMaxHp(0)
    , m_nDeltaMinHp(0)
{
}
//-----------------------------------------------------------------------
DeltaHpComponent::DeltaHpComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
DeltaHpComponent::~DeltaHpComponent()
{
}
//-----------------------------------------------------------------------
void DeltaHpComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const DeltaHpComponent& srcComp = dynamic_cast<const DeltaHpComponent&>(src);
    m_nDeltaHp = srcComp.m_nDeltaHp;
    m_nDeltaMaxHp = srcComp.m_nDeltaMaxHp;
    m_nDeltaMinHp = srcComp.m_nDeltaMinHp;
}
//-----------------------------------------------------------------------
bool DeltaHpComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
JoystickComponent::JoystickComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
{
}
//-----------------------------------------------------------------------
JoystickComponent::JoystickComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
JoystickComponent::~JoystickComponent()
{
}
//-----------------------------------------------------------------------
void JoystickComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const JoystickComponent& srcComp = dynamic_cast<const JoystickComponent&>(src);
}
//-----------------------------------------------------------------------
bool JoystickComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    return true;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ScaleComponent::ScaleComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader)
    : u2::Component(creator, type, handle, group, isManual, loader)
    , v2Scale(1.0f, 1.0f)
{
}
//-----------------------------------------------------------------------
ScaleComponent::ScaleComponent(const u2::String& type, const u2::String& name, const u2::String& guid)
    : u2::Component(type, name, guid)
{

}
//-----------------------------------------------------------------------
ScaleComponent::~ScaleComponent()
{
}
//-----------------------------------------------------------------------
void ScaleComponent::copy(const u2::Component& src)
{
    u2::Component::copy(src);

    const ScaleComponent& srcComp = dynamic_cast<const ScaleComponent&>(src);
    v2Scale = srcComp.v2Scale;
}
//-----------------------------------------------------------------------
bool ScaleComponent::_loadFromXml(const TiXmlElement* compElem, u2::String& error)
{
    const TiXmlElement* pCompElem = compElem;
    u2::String& szError = error;

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
        const TiXmlElement* pPositionElem = pCompElem->FirstChildElement("Scale");
        GET_ERROR_LINE_AND_BREAK(pPositionElem, szError);
        const char* pszValue = pPositionElem->Attribute("value");
        GET_ERROR_LINE_AND_BREAK(pszValue, szError);
        v2Scale = StringUtil::parseVector2(pszValue);

    } while (0);

    return szError == BLANK;
}