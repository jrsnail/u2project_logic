#ifndef __U2XmlSerialize_H__
#define __U2XmlSerialize_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"
#include "U2Stream.h"
#include "U2Version.h"
#include "U2HeaderPrefix.h"


class TiXmlElement;


U2EG_NAMESPACE_BEGIN


#define GET_ERROR_LINE(errstr)       \
    StringStream ss;                 \
    ss << "parse error: " << __FUNCTION__ << ", line = " << __LINE__;   \
    errstr = ss.str();

#define GET_ERROR_LINE_AND_BREAK(elem, errstr)      \
    if (elem == nullptr)                            \
    {                                               \
        GET_ERROR_LINE(errstr);                     \
        assert(0);                                  \
        break;                                      \
    }


class XmlSerialize : public Object
{
public:
    XmlSerialize(const String& type, const String& name, const String& guid = BLANK);
    virtual ~XmlSerialize();

    virtual bool load(const TiXmlElement* elem, const String& groupName) = 0;
    virtual bool save() = 0;

protected:
};


class XmlSerializeManager : public Singleton<XmlSerializeManager>
{
public:
    XmlSerializeManager();
    virtual ~XmlSerializeManager();

    virtual void load(InStreamPtr& stream, const String& groupName);
    virtual void save();

    void addTypeVersionPair(const String& xmlRoot, const Version& version, const String& objType);

protected:
    void _initialize();

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static XmlSerializeManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static XmlSerializeManager* getSingletonPtr(void);

protected:
    // <Type_Version, Serialize type>
    typedef map<String, String>::type       TypeVersionMap;
    TypeVersionMap  m_TypeVersionMap;
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif /* defined(__U2XmlSerialize__) */
