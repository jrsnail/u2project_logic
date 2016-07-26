#ifndef __U2XmlSerialize__
#define __U2XmlSerialize__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Serialize.h"
#include "U2Version.h"


U2EG_NAMESPACE_BEGIN



class XmlSerialize : public Serialize
{
public:
    XmlSerialize(const String& type, const String& name);
    virtual ~XmlSerialize();

    virtual void load(const void* data) override;
    virtual void save() override;

    void addTypeVersionPair(const String& xmlRoot, const Version& version, const String& objType);

protected:
    void _initialize();

protected:
    // <Type_Version, Serialize type>
    typedef map<String, String>::type       TypeVersionMap;
    TypeVersionMap  m_TypeVersionMap;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2XmlSerialize__) */
