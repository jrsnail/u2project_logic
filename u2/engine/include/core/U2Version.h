#ifndef __U2Version_H__
#define __U2Version_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class Version
{
public:
    Version(unsigned int version
        , const std::string& versionName = U2_VERSION_NAME, const std::string& suffix = U2_VERSION_SUFFIX);
    Version(unsigned int magor, unsigned int minor, unsigned int patch
        , const std::string& versionName = U2_VERSION_NAME, const std::string& suffix = U2_VERSION_SUFFIX);
    Version(const std::string& version
        , const std::string& versionName = U2_VERSION_NAME, const std::string& suffix = U2_VERSION_SUFFIX);

    virtual ~Version();
 
    bool operator< (const Version& other);

    inline unsigned int getMajor();
    inline unsigned int getMinor();
    inline unsigned int getPatch();
    inline std::string getVersionNumAsStr();
    inline const std::string& getVersionName();
    inline const std::string& getVersionSuffix();

private:
    inline unsigned int _combine(unsigned int magor, unsigned int minor, unsigned int patch);

private:
    unsigned int    m_uVersion;
    std::string     m_szSuffix;
    std::string     m_szVersionName;
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Task_H__
