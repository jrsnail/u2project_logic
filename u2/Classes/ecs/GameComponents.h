#ifndef __PredefinedComponents__
#define __PredefinedComponents__


#include "U2Core.h"
#include "cocos2d.h"


class TiXmlElement;


U2EG_NAMESPACE_USING


class SpriteComponent : public u2::Component
{
public:
    SpriteComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    SpriteComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~SpriteComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

public:
    u2::String szFilename;
    cocos2d::Sprite*    pSprite;
};


class PositionComponent : public u2::Component
{
public:
    PositionComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    PositionComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~PositionComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

public:
    cocos2d::Vec2   v2Pos;
};


class VelocityComponent : public u2::Component
{
public:
    VelocityComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    VelocityComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~VelocityComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

public:
    cocos2d::Vec2   v2Velocity;
};


class SpeedDirComponent : public u2::Component
{
public:
    SpeedDirComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    SpeedDirComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~SpeedDirComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

public:
    cocos2d::Vec2   v2Dir;
};


class SpeedComponent : public u2::Component
{
public:
    SpeedComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    SpeedComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~SpeedComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

public:
    u2real   fSpeed;
    u2real   fMaxSpeed;
    u2real   fMinSpeed;
};


class HpComponent : public u2::Component
{
public:
    HpComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    HpComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~HpComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

protected:
    u2uint32   m_uHp;
    u2uint32   m_uMaxHp;
    u2uint32   m_uMinHp;
};


class BaseHpComponent : public u2::Component
{
public:
    BaseHpComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    BaseHpComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~BaseHpComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

protected:
    u2uint32   m_uBaseHp;
    u2uint32   m_uBaseMaxHp;
    u2uint32   m_uBaseMinHp;
};


class DeltaHpComponent : public u2::Component
{
public:
    DeltaHpComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    DeltaHpComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~DeltaHpComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

protected:
    u2int32   m_nDeltaHp;
    u2int32   m_nDeltaMaxHp;
    u2int32   m_nDeltaMinHp;
};


class JoystickComponent : public u2::Component
{
public:
    JoystickComponent(ResourceManager* creator, const u2::String& type, ResourceHandle handle,
        const u2::String& group, bool isManual = false, ManualResourceLoader* loader = 0);
    JoystickComponent(const u2::String& type, const u2::String& name, const u2::String& guid = BLANK);
    virtual ~JoystickComponent();

    virtual void copy(const u2::Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, u2::String& error) override;

protected:
};



#endif /* defined(__PredefinedComponents__) */
