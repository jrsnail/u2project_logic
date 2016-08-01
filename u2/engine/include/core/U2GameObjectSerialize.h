#ifndef __U2GameObjectSerialize__
#define __U2GameObjectSerialize__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2XmlSerialize.h"
#include "U2HeaderPrefix.h"


class TiXmlElement;


U2EG_NAMESPACE_BEGIN


class GameObjectSerialize_0_0_1 : public XmlSerialize
{
public:
    GameObjectSerialize_0_0_1(const String& type, const String& name);
    virtual ~GameObjectSerialize_0_0_1();

    virtual bool load(const TiXmlElement* elem, const String& groupName) override;
    virtual bool save() override;

protected:
    bool _loadGameObject(const TiXmlElement* compElem, String& error, const String& groupName);
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif /* defined(__U2GameObjectSerialize__) */
