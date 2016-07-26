#include "U2Version.h"

#include "U2StringUtil.h"


U2EG_NAMESPACE_USING



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Version::Version(u2uint32 version
    , const String& versionName, const String& suffix)
: m_uVersion(version)
, m_szVersionName(versionName)
, m_szSuffix(suffix)
{
}
//-----------------------------------------------------------------------
Version::Version(u2uint32 magor, u2uint32 minor, u2uint32 patch
    , const String& versionName, const String& suffix)
    : Version(_combine(magor, minor, patch), versionName, suffix)
{

}
//-----------------------------------------------------------------------
Version::Version(const String& version
    , const String& versionName, const String& suffix)
    : m_uVersion(0)
    , m_szVersionName(versionName)
    , m_szSuffix(suffix)
{
    std::vector<String> v = StringUtil::split(version, ".");
    if (v.size() != 3)
    {
        assert(0);
    }
    else
    {
        u2uint32 uMajor = StringUtil::parseUnsignedInt(v[0]);
        u2uint32 uMinor = StringUtil::parseUnsignedInt(v[1]);
        u2uint32 uPatch = StringUtil::parseUnsignedInt(v[2]);
        m_uVersion = _combine(uMajor, uMinor, uPatch);
    }
}
//-----------------------------------------------------------------------
Version::~Version()
{
}
//-----------------------------------------------------------------------
bool Version::operator< (const Version& other)
{
    return m_uVersion < other.m_uVersion;
}
//-----------------------------------------------------------------------
u2uint32 Version::_combine(u2uint32 magor, u2uint32 minor, u2uint32 patch)
{
    return (magor << 16) | (minor << 8) | patch;
}
//-----------------------------------------------------------------------
u2uint32 Version::getMajor() const
{
    return m_uVersion >> 16;
}
//-----------------------------------------------------------------------
u2uint32 Version::getMinor() const
{
    return (m_uVersion >> 8) & 0xFF;
}
//-----------------------------------------------------------------------
u2uint32 Version::getPatch() const
{
    return m_uVersion & 0xFF;
}
//-----------------------------------------------------------------------
String Version::getVersionNumAsStr() const
{
    StringStream ss;
    ss << getMajor();
    ss << ".";
    ss << getMinor();
    ss << ".";
    ss << getPatch();
    return ss.str();
}
//-----------------------------------------------------------------------
const String& Version::getVersionName() const
{
    return m_szVersionName;
}
//-----------------------------------------------------------------------
const String& Version::getVersionSuffix() const
{
    return m_szSuffix;
}