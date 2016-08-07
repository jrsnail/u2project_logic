#include "U2StringUtil.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
String StringUtil::m_sDefaultStringLocale	= U2_DEFAULT_LOCALE;
std::locale StringUtil::m_sLocale			= std::locale(m_sDefaultStringLocale.c_str());
bool StringUtil::m_sUseLocale				= false;
//-----------------------------------------------------------------------
void StringUtil::trim(String& str, bool left, bool right)
{
    /*
    size_t lspaces, rspaces, len = length(), i;

    lspaces = rspaces = 0;

    if( left )
    {
        // Find spaces / tabs on the left
        for( i = 0;
            i < len && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
            ++lspaces, ++i );
    }
        
    if( right && lspaces < len )
    {
        // Find spaces / tabs on the right
        for( i = len - 1;
            i >= 0 && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
            rspaces++, i-- );
    }

    *this = substr(lspaces, len-lspaces-rspaces);
    */
    static const String delims = " \t\r";
    if(right)
        str.erase(str.find_last_not_of(delims)+1); // trim right
    if(left)
        str.erase(0, str.find_first_not_of(delims)); // trim left
}
//-----------------------------------------------------------------------
StringVector StringUtil::split( const String& str, const String& delims, unsigned int maxSplits, bool preserveDelims)
{
    StringVector ret;
    // Pre-allocate some space for performance
    ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

    unsigned int numSplits = 0;

    // Use STL methods 
    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find_first_of(delims, start);
        if (pos == start)
        {
            // Do nothing
            start = pos + 1;
        }
        else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
        {
            // Copy the rest of the string
            ret.push_back( str.substr(start) );
            break;
        }
        else
        {
            // Copy up to delimiter
            ret.push_back( str.substr(start, pos - start) );

            if(preserveDelims)
            {
                // Sometimes there could be more than one delimiter in a row.
                // Loop until we don't find any more delims
                size_t delimStart = pos, delimPos;
                delimPos = str.find_first_not_of(delims, delimStart);
                if (delimPos == String::npos)
                {
                    // Copy the rest of the string
                    ret.push_back( str.substr(delimStart) );
                }
                else
                {
                    ret.push_back( str.substr(delimStart, delimPos - delimStart) );
                }
            }

            start = pos + 1;
        }
        // parse up to next real data
        start = str.find_first_not_of(delims, start);
        ++numSplits;

    } while (pos != String::npos);

    return ret;
}
//-----------------------------------------------------------------------
StringVector StringUtil::tokenise( const String& str, const String& singleDelims, const String& doubleDelims, unsigned int maxSplits)
{
    StringVector ret;
    // Pre-allocate some space for performance
    ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

    unsigned int numSplits = 0;
    String delims = singleDelims + doubleDelims;

    // Use STL methods 
    size_t start, pos;
    char curDoubleDelim = 0;
    start = 0;
    do 
    {
        if (curDoubleDelim != 0)
        {
            pos = str.find(curDoubleDelim, start);
        }
        else
        {
            pos = str.find_first_of(delims, start);
        }

        if (pos == start)
        {
            char curDelim = str.at(pos);
            if (doubleDelims.find_first_of(curDelim) != String::npos)
            {
                curDoubleDelim = curDelim;
            }
            // Do nothing
            start = pos + 1;
        }
        else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
        {
            if (curDoubleDelim != 0)
            {
                //Missing closer. Warn or throw exception?
            }
            // Copy the rest of the string
            ret.push_back( str.substr(start) );
            break;
        }
        else
        {
            if (curDoubleDelim != 0)
            {
                curDoubleDelim = 0;
            }

            // Copy up to delimiter
            ret.push_back( str.substr(start, pos - start) );
            start = pos + 1;
        }
        if (curDoubleDelim == 0)
        {
            // parse up to next real data
            start = str.find_first_not_of(singleDelims, start);
        }
            
        ++numSplits;

    } while (start != String::npos);

    return ret;
}
//-----------------------------------------------------------------------
void StringUtil::toLowerCase(String& str)
{
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        tolower);
}
//-----------------------------------------------------------------------
void StringUtil::toUpperCase(String& str) 
{
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        toupper);
}
//-----------------------------------------------------------------------
void StringUtil::toTitleCase(String& str) 
{
    String::iterator it = str.begin();
    *it = toupper(*it);
    for (; it != str.end() - 1; it++)
    {
        if (*it == ' ') 
        {
            *(it + 1) = toupper(*(it + 1));
        }
    }
}
//-----------------------------------------------------------------------
bool StringUtil::startsWith(const String& str, const String& pattern, bool lowerCase)
{
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0)
        return false;

    String startOfThis = str.substr(0, patternLen);
    if (lowerCase)
    {
        String lowerCasePattern = pattern;
        StringUtil::toLowerCase(lowerCasePattern);
        StringUtil::toLowerCase(startOfThis);
        return (startOfThis == lowerCasePattern);
    }

    return (startOfThis == pattern);
}
//-----------------------------------------------------------------------
bool StringUtil::endsWith(const String& str, const String& pattern, bool lowerCase)
{
    size_t thisLen = str.length();
    size_t patternLen = pattern.length();
    if (thisLen < patternLen || patternLen == 0)
        return false;

    String endOfThis = str.substr(thisLen - patternLen, patternLen);
    if (lowerCase)
    {
        String lowerCasePattern = pattern;
        StringUtil::toLowerCase(lowerCasePattern);
        StringUtil::toLowerCase(endOfThis);
        return (endOfThis == lowerCasePattern);
    }

    return (endOfThis == pattern);
}
//-----------------------------------------------------------------------
String StringUtil::standardisePath(const String& init)
{
    String path = init;

    std::replace( path.begin(), path.end(), '\\', '/' );
    if( path[path.length() - 1] != '/' )
        path += '/';

    return path;
}
//-----------------------------------------------------------------------
String StringUtil::normalizeFilePath(const String& init, bool makeLowerCase)
{
    const char* bufferSrc = init.c_str();
    int pathLen = (int)init.size();
    int indexSrc = 0;
    int indexDst = 0;
    int metaPathArea = 0;

    char reservedBuf[1024];
    char* bufferDst = reservedBuf;
    bool isDestAllocated = false;
    if (pathLen > 1023)
    {
        //if source path is to long ensure we don't do a buffer overrun by allocating some
        //new memory
        isDestAllocated = true;
        bufferDst = new char[pathLen + 1];
    }

    //The outer loop loops over directories
    while (indexSrc < pathLen)
    {       
        if ((bufferSrc[indexSrc] == '\\') || (bufferSrc[indexSrc] == '/'))
        {
            //check if we have a directory delimiter if so skip it (we should already
            //have written such a delimiter by this point
            ++indexSrc;
            continue;
        }
        else
        {
            //check if there is a directory to skip of type ".\"
            if ((bufferSrc[indexSrc] == '.') && 
                ((bufferSrc[indexSrc + 1] == '\\') || (bufferSrc[indexSrc + 1] == '/')))
            {
                indexSrc += 2;
                continue;           
            }

            //check if there is a directory to skip of type "..\"
            else if ((bufferSrc[indexSrc] == '.') && (bufferSrc[indexSrc + 1] == '.') &&
                ((bufferSrc[indexSrc + 2] == '\\') || (bufferSrc[indexSrc + 2] == '/')))
            {
                if (indexDst > metaPathArea)
                {
                    //skip a directory backward in the destination path
                    do {
                        --indexDst;
                    }
                    while ((indexDst > metaPathArea) && (bufferDst[indexDst - 1] != '/'));
                    indexSrc += 3;
                    continue;
                }
                else
                {
                    //we are about to write "..\" to the destination buffer
                    //ensure we will not remove this in future "skip directories"
                    metaPathArea += 3;
                }
            }
        }

        //transfer the current directory name from the source to the destination
        while (indexSrc < pathLen)
        {
            char curChar = bufferSrc[indexSrc];
            if (makeLowerCase) curChar = tolower(curChar);
            if ((curChar == '\\') || (curChar == '/')) curChar = '/';
            bufferDst[indexDst] = curChar;
            ++indexDst;
            ++indexSrc;
            if (curChar == '/') break;
        }
    }
    bufferDst[indexDst] = 0;

    String normalized(bufferDst); 
    if (isDestAllocated)
    {
        delete[] bufferDst;
    }

    return normalized;      
}
//-----------------------------------------------------------------------
void StringUtil::splitFilename(const String& qualifiedName, 
    String& outBasename, String& outPath)
{
    String path = qualifiedName;
    // Replace \ with / first
    std::replace( path.begin(), path.end(), '\\', '/' );
    // split based on final /
    size_t i = path.find_last_of('/');

    if (i == String::npos)
    {
        outPath.clear();
        outBasename = qualifiedName;
    }
    else
    {
        outBasename = path.substr(i+1, path.size() - i - 1);
        outPath = path.substr(0, i+1);
    }
}
//-----------------------------------------------------------------------
void StringUtil::splitBaseFilename(const String& fullName, 
    String& outBasename, String& outExtention)
{
    size_t i = fullName.find_last_of(".");
    if (i == String::npos)
    {
        outExtention.clear();
        outBasename = fullName;
    }
    else
    {
        outExtention = fullName.substr(i+1);
        outBasename = fullName.substr(0, i);
    }
}
// ----------------------------------------------------------------------------------------------------------------------------------------------
void StringUtil::splitFullFilename( const String& qualifiedName, 
    String& outBasename, String& outExtention, String& outPath )
{
    String fullName;
    splitFilename( qualifiedName, fullName, outPath );
    splitBaseFilename( fullName, outBasename, outExtention );
}
//-----------------------------------------------------------------------
bool StringUtil::match(const String& str, const String& pattern, bool caseSensitive)
{
    String tmpStr = str;
    String tmpPattern = pattern;
    if (!caseSensitive)
    {
        StringUtil::toLowerCase(tmpStr);
        StringUtil::toLowerCase(tmpPattern);
    }

    String::const_iterator strIt = tmpStr.begin();
    String::const_iterator patIt = tmpPattern.begin();
    String::const_iterator lastWildCardIt = tmpPattern.end();
    while (strIt != tmpStr.end() && patIt != tmpPattern.end())
    {
        if (*patIt == '*')
        {
            lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;
            if (patIt == tmpPattern.end())
            {
                // Skip right to the end since * matches the entire rest of the string
                strIt = tmpStr.end();
            }
            else
            {
                // scan until we find next pattern character
                while(strIt != tmpStr.end() && *strIt != *patIt)
                    ++strIt;
            }
        }
        else
        {
            if (*patIt != *strIt)
            {
                if (lastWildCardIt != tmpPattern.end())
                {
                    // The last wildcard can match this incorrect sequence
                    // rewind pattern to wildcard and keep searching
                    patIt = lastWildCardIt;
                    lastWildCardIt = tmpPattern.end();
                }
                else
                {
                    // no wildwards left
                    return false;
                }
            }
            else
            {
                ++patIt;
                ++strIt;
            }
        }

    }
    // If we reached the end of both the pattern and the string, we succeeded
    if (patIt == tmpPattern.end() && strIt == tmpStr.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}
//-----------------------------------------------------------------------
const String StringUtil::replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat)
{
    String result = source;
    String::size_type pos = 0;
    while(1)
    {
        pos = result.find(replaceWhat,pos);
        if (pos == String::npos) break;
        result.replace(pos,replaceWhat.size(),replaceWithWhat);
        pos += replaceWithWhat.size();
    }
    return result;
}
//-----------------------------------------------------------------------
String StringUtil::toString(u2real val, unsigned short precision,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;

	return stream.str();
}
#if U2_DOUBLE_PRECISION == 1
//-----------------------------------------------------------------------
String StringUtil::toString(float val, unsigned short precision,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
#else
//-----------------------------------------------------------------------
String StringUtil::toString(double val, unsigned short precision,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.precision(precision);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
#endif
//-----------------------------------------------------------------------
String StringUtil::toString(int val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
#if U2_ARCH_TYPE == U2_ARCHITECTURE_64 || U2_PLATFORM == U2_PLATFORM_APPLE || U2_PLATFORM == U2_PLATFORM_APPLE_IOS
String StringUtil::toString(unsigned int val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
String StringUtil::toString(size_t val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
#	if U2_COMPILER == U2_COMPILER_MSVC
//-----------------------------------------------------------------------
String StringUtil::toString(unsigned long val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
#	endif
//-----------------------------------------------------------------------
String StringUtil::toString(u2uint64 val,
    unsigned short width, char fill, std::ios::fmtflags flags)
{
    StringStream stream;
    if (m_sUseLocale)
        stream.imbue(m_sLocale);
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
#else
String StringUtil::toString(size_t val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
String StringUtil::toString(unsigned long val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
String StringUtil::toString(u2uint64 val,
    unsigned short width, char fill, std::ios::fmtflags flags)
{
    StringStream stream;
    if (m_sUseLocale)
        stream.imbue(m_sLocale);
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}
//-----------------------------------------------------------------------
#endif
String StringUtil::toString(long val,
	unsigned short width, char fill, std::ios::fmtflags flags)
{
	StringStream stream;
	if (m_sUseLocale)
		stream.imbue(m_sLocale);
	stream.width(width);
	stream.fill(fill);
	if (flags)
		stream.setf(flags);
	stream << val;
	return stream.str();
}
//-----------------------------------------------------------------------
u2real StringUtil::parseReal(const String& val, u2real defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	u2real ret = defaultValue;
	if (!(str >> ret))
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
int StringUtil::parseInt(const String& val, int defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	int ret = defaultValue;
	if (!(str >> ret))
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
unsigned int StringUtil::parseUnsignedInt(const String& val, unsigned int defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	unsigned int ret = defaultValue;
	if (!(str >> ret))
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
long StringUtil::parseLong(const String& val, long defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	long ret = defaultValue;
	if (!(str >> ret))
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
u2int64 StringUtil::parseInt64(const String& val, u2int64 defaultValue)
{
    // Use iStringStream for direct correspondence with toString
    StringStream str(val);
    if (m_sUseLocale)
        str.imbue(m_sLocale);
    u2int64 ret = defaultValue;
    if (!(str >> ret))
        return defaultValue;

    return ret;
}
//-----------------------------------------------------------------------
unsigned long StringUtil::parseUnsignedLong(const String& val, unsigned long defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	unsigned long ret = defaultValue;
	if (!(str >> ret))
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
u2uint64 StringUtil::parseUnsignedInt64(const String& val, u2uint64 defaultValue)
{
    // Use iStringStream for direct correspondence with toString
    StringStream str(val);
    if (m_sUseLocale)
        str.imbue(m_sLocale);
    u2uint64 ret = defaultValue;
    if (!(str >> ret))
        return defaultValue;

    return ret;
}
//-----------------------------------------------------------------------
size_t StringUtil::parseSizeT(const String& val, size_t defaultValue)
{
	// Use iStringStream for direct correspondence with toString
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	size_t ret = defaultValue;
	if (!(str >> ret))
		return defaultValue;

	return ret;
}
//-----------------------------------------------------------------------
bool StringUtil::parseBool(const String& val, bool defaultValue)
{
	//FIXME Returns both parsed value and error in same value - ambiguous.
	// Suggested alternatives: implement exception handling or make either
	// error or parsed value a parameter.
	if ((StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
		|| StringUtil::startsWith(val, "1") || StringUtil::startsWith(val, "on")))
		return true;
	else if ((StringUtil::startsWith(val, "false") || StringUtil::startsWith(val, "no")
		|| StringUtil::startsWith(val, "0") || StringUtil::startsWith(val, "off")))
		return false;
	else
		return defaultValue;
}
//-----------------------------------------------------------------------
cocos2d::Vec2 StringUtil::parseVector2(const String& val, const cocos2d::Vec2& defaultValue)
{
    // Split on space
    vector<String>::type vec = StringUtil::split(val);

    if (vec.size() != 2)
    {
        return defaultValue;
    }
    else
    {
        return cocos2d::Vec2(parseReal(vec[0], defaultValue.x), parseReal(vec[1], defaultValue.y));
    }
}
//-----------------------------------------------------------------------
bool StringUtil::isNumber(const String& val)
{
	StringStream str(val);
	if (m_sUseLocale)
		str.imbue(m_sLocale);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}