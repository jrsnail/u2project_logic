#include "U2Version.h"

#include "U2StringUtil.h"


U2EG_NAMESPACE_USING



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Version::Version(unsigned int version
    , const std::string& versionName, const std::string& suffix)
: m_uVersion(version)
, m_szVersionName(versionName)
, m_szSuffix(suffix)
{
}
//-----------------------------------------------------------------------
Version::Version(unsigned int magor, unsigned int minor, unsigned int patch
    , const std::string& versionName, const std::string& suffix)
    : Version(_combine(magor, minor, patch), versionName, suffix)
{

}
//-----------------------------------------------------------------------
Version::Version(const std::string& version
    , const std::string& versionName, const std::string& suffix)
    : m_uVersion(0)
    , m_szVersionName(versionName)
    , m_szSuffix(suffix)
{
    std::vector<std::string> v = StringUtil::split(version, ".");
    if (v.size() != 3)
    {
        assert(0);
    }
    else
    {
        unsigned int uMajor = StringUtil::parseUnsignedInt(v[0]);
        unsigned int uMinor = StringUtil::parseUnsignedInt(v[1]);
        unsigned int uPatch = StringUtil::parseUnsignedInt(v[2]);
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
unsigned int Version::_combine(unsigned int magor, unsigned int minor, unsigned int patch)
{
    return (magor << 16) | (minor << 8) | patch;
}
//-----------------------------------------------------------------------
unsigned int Version::getMajor()
{
    return m_uVersion >> 16;
}
//-----------------------------------------------------------------------
unsigned int Version::getMinor()
{
    return (m_uVersion >> 8) & 0xFF;
}
//-----------------------------------------------------------------------
unsigned int Version::getPatch()
{
    return m_uVersion & 0xFF;
}
//-----------------------------------------------------------------------
std::string Version::getVersionNumAsStr()
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
const std::string& Version::getVersionName()
{
    return m_szVersionName;
}
//-----------------------------------------------------------------------
const std::string& Version::getVersionSuffix()
{
    return m_szSuffix;
}