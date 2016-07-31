#ifndef __U2PredefinedComponents__
#define __U2PredefinedComponents__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Component.h"
#include "cocos2d.h"
#include "U2HeaderPrefix.h"


class TiXmlElement;


U2EG_NAMESPACE_BEGIN


class SpriteComponent : public Component
{
public:
    SpriteComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~SpriteComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    String m_szFilename;
};


class PositionComponent : public Component
{
public:
    PositionComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~PositionComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    cocos2d::Vec2   m_v2Pos;
};


class VelocityComponent : public Component
{
public:
    VelocityComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~VelocityComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    cocos2d::Vec2   m_v2Velocity;
};


class SpeedComponent : public Component
{
public:
    SpeedComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~SpeedComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    u2real   m_fSpeed;
    u2real   m_fMaxSpeed;
    u2real   m_fMinSpeed;
};


class HpComponent : public Component
{
public:
    HpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~HpComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    u2uint32   m_uHp;
    u2uint32   m_uMaxHp;
    u2uint32   m_uMinHp;
};


class BaseHpComponent : public Component
{
public:
    BaseHpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~BaseHpComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    u2uint32   m_uBaseHp;
    u2uint32   m_uBaseMaxHp;
    u2uint32   m_uBaseMinHp;
};


class DeltaHpComponent : public Component
{
public:
    DeltaHpComponent(ResourceManager* creator, const String& type, ResourceHandle handle,
        const String& group, const String& name = BLANK, bool isManual = false
        , ManualResourceLoader* loader = 0);
    virtual ~DeltaHpComponent();

    virtual void copy(const Component& src) override;

    virtual bool _loadFromXml(const TiXmlElement* compElem, String& error) override;

protected:
    u2int32   m_nDeltaHp;
    u2int32   m_nDeltaMaxHp;
    u2int32   m_nDeltaMinHp;
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif /* defined(__U2PredefinedComponents__) */
