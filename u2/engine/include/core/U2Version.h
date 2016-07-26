#ifndef __U2Version_H__
#define __U2Version_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class Version
{
public:
    Version(u2uint32 version
        , const String& versionName = U2_VERSION_NAME, const String& suffix = U2_VERSION_SUFFIX);
    Version(u2uint32 magor, u2uint32 minor, u2uint32 patch
        , const String& versionName = U2_VERSION_NAME, const String& suffix = U2_VERSION_SUFFIX);
    Version(const String& version
        , const String& versionName = U2_VERSION_NAME, const String& suffix = U2_VERSION_SUFFIX);

    virtual ~Version();
 
    bool operator< (const Version& other);

    u2uint32 getMajor() const;
    u2uint32 getMinor() const;
    u2uint32 getPatch() const;
    String getVersionNumAsStr() const;
    const String& getVersionName() const;
    const String& getVersionSuffix() const;

private:
    u2uint32 _combine(u2uint32 magor, u2uint32 minor, u2uint32 patch);

private:
    u2uint32    m_uVersion;
    String     m_szSuffix;
    String     m_szVersionName;
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Task_H__
