#ifndef __U2ComponentSerialize__
#define __U2ComponentSerialize__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2XmlSerialize.h"
#include "U2HeaderPrefix.h"


class TiXmlElement;


U2EG_NAMESPACE_BEGIN


class ComponentSerialize_0_0_1 : public XmlSerialize
{
public:
    ComponentSerialize_0_0_1(const String& type, const String& name);
    virtual ~ComponentSerialize_0_0_1();

    virtual bool load(const TiXmlElement* elem, const String& groupName) override;
    virtual bool save() override;

protected:
    bool _loadComponent(const TiXmlElement* compElem, String& error, const String& groupName);
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif /* defined(__U2ComponentSerialize__) */
