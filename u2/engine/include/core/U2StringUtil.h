#ifndef __U2StringUtil_H__
#define __U2StringUtil_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "cocos2d.h"
#include "U2HeaderPrefix.h"

// If we're using the GCC 3.1 C++ Std lib
#if U2_COMPILER == U2_COMPILER_GNUC && U2_COMP_VER >= 310 && !defined(STLPORT)

// For gcc 4.3 see http://gcc.gnu.org/gcc-4.3/changes.html
#   if U2_COMP_VER >= 430
#       include <tr1/unordered_map>
#   else
#       include <ext/hash_map>
namespace __gnu_cxx
{
    template <> struct hash< u2::_StringBase >
    {
        size_t operator()( const Ogre::_StringBase _stringBase ) const
        {
            /* This is the PRO-STL way, but it seems to cause problems with VC7.1
               and in some other cases (although I can't recreate it)
               hash<const char*> H;
               return H(_stringBase.c_str());
            */
            /** This is our custom way */
            register size_t ret = 0;
            for( u2::_StringBase::const_iterator it = _stringBase.begin(); it != _stringBase.end(); ++it )
                ret = 5 * ret + *it;

            return ret;
        }
    };
}
#   endif

#endif


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/

/** Utility class for manipulating Strings.  */
class _U2Export StringUtil
{
public:

    /** Removes any whitespace characters, be it standard space or
        TABs and so on.
        @remarks
        The user may specify whether they want to trim only the
        beginning or the end of the String ( the default action is
        to trim both).
    */
    static void trim( String& str, bool left = true, bool right = true );

    /** Returns a StringVector that contains all the substrings delimited
        by the characters in the passed <code>delims</code> argument.
        @param
        delims A list of delimiter characters to split by
        @param
        maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
        parameters is > 0, the splitting process will stop after this many splits, left to right.
        @param
        preserveDelims Flag to determine if delimiters should be saved as substrings
    */
    static vector<String>::type split( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0, bool preserveDelims = false);

    /** Returns a StringVector that contains all the substrings delimited
        by the characters in the passed <code>delims</code> argument,
        or in the <code>doubleDelims</code> argument, which is used to include (normal)
        delimeters in the tokenised string. For example, "strings like this".
        @param
            delims A list of delimiter characters to split by
        @param
            doubleDelims A list of double delimeters characters to tokenise by
        @param
        maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
        parameters is > 0, the splitting process will stop after this many splits, left to right.
    */
    static vector<String>::type tokenise( const String& str, const String& delims = "\t\n ", const String& doubleDelims = "\"", unsigned int maxSplits = 0);

    /** Lower-cases all the characters in the string.
    */
    static void toLowerCase( String& str );

    /** Upper-cases all the characters in the string.
    */
    static void toUpperCase( String& str );

    /** Upper-cases the first letter of each word.
    */
    static void toTitleCase( String& str );


    /** Returns whether the string begins with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the start of the string will be lower cased before
        comparison, pattern should also be in lower case.
    */
    static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

    /** Returns whether the string ends with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the end of the string will be lower cased before
        comparison, pattern should also be in lower case.
    */
    static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

    /** Method for standardising paths - use forward slashes only, end with slash.
    */
    static String standardisePath( const String &init);

    /** Returns a normalized version of a file path
        This method can be used to make file path strings which point to the same directory
        but have different texts to be normalized to the same text. The function:
        - Transforms all backward slashes to forward slashes.
        - Removes repeating slashes.
        - Removes initial slashes from the beginning of the path.
        - Removes ".\" and "..\" meta directories.
        - Sets all characters to lowercase (if requested)
        @param init The file path to normalize.
        @param makeLowerCase If true, transforms all characters in the string to lowercase.
    */
    static String normalizeFilePath(const String& init, bool makeLowerCase = true);


    /** Method for splitting a fully qualified filename into the base name
        and path.
        @remarks
        Path is standardised as in standardisePath
    */
    static void splitFilename(const String& qualifiedName,
                                String& outBasename, String& outPath);

    /** Method for splitting a fully qualified filename into the base name,
        extension and path.
        @remarks
        Path is standardised as in standardisePath
    */
    static void splitFullFilename(const String& qualifiedName,
                                    String& outBasename, String& outExtention,
                                    String& outPath);

    /** Method for splitting a filename into the base name
        and extension.
    */
    static void splitBaseFilename(const String& fullName,
                                    String& outBasename, String& outExtention);


    /** Simple pattern-matching routine allowing a wildcard pattern.
        @param str String to test
        @param pattern Pattern to match against; can include simple '*' wildcards
        @param caseSensitive Whether the match is case sensitive or not
    */
    static bool match(const String& str, const String& pattern, bool caseSensitive = true);


    /** Replace all instances of a sub-string with a another sub-string.
        @param source Source string
        @param replaceWhat Sub-string to find and replace
        @param replaceWithWhat Sub-string to replace with (the new sub-string)
        @return An updated string with the sub-string replaced
    */
    static const String replaceAll(const String& source, const String& replaceWhat, const String& replaceWithWhat);

	/** Converts a u2real to a String. */
	static String toString(u2real val, unsigned short precision = 6,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));

	// This counter-intuitive guard is correct. In case of enabled double precision
	// the toString() version above using Ogre::u2real already provides a double precision
	// version and hence we need to explicitly declare a float version as well.
#if U2_DOUBLE_PRECISION == 1
	/** Converts a float to a String. */
	static String toString(float val, unsigned short precision = 6,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));
#else
	/** Converts a double to a String. */
	static String toString(double val, unsigned short precision = 6,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif

	/** Converts an int to a String. */
	static String toString(int val, unsigned short width = 0,
		char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));

#if U2_ARCH_TYPE == U2_ARCHITECTURE_64 || U2_PLATFORM == U2_PLATFORM_APPLE || U2_PLATFORM == U2_PLATFORM_APPLE_IOS
	/** Converts an unsigned int to a String. */
	static String toString(unsigned int val,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));

	/** Converts a size_t to a String. */
	static String toString(size_t val,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));
#	if U2_COMPILER == U2_COMPILER_MSVC
	/** Converts an unsigned long to a String. */
	static String toString(unsigned long val,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));
#	endif

    /** Converts an unsigned long to a String. */
    static String toString(u2uint64 val,
        unsigned short width = 0, char fill = ' ',
        std::ios::fmtflags flags = std::ios::fmtflags(0));
#else
	/** Converts a size_t to a String. */
	static String toString(size_t val,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));

	/** Converts an unsigned long to a String. */
	static String toString(unsigned long val,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));

    /** Converts an unsigned int64 to a String. */
    static String toString(u2uint64 val,
        unsigned short width = 0, char fill = ' ',
        std::ios::fmtflags flags = std::ios::fmtflags(0));
#endif

	/** Converts a long to a String. */
	static String toString(long val,
		unsigned short width = 0, char fill = ' ',
		std::ios::fmtflags flags = std::ios::fmtflags(0));

	/** Converts a boolean to a String.
	@param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
	*/
	static String toString(bool val, bool yesNo = false);

	/** Converts a String to a u2real.
	@return
	0.0 if the value could not be parsed, otherwise the u2real version of the String.
	*/
	static u2real parseReal(const String& val, u2real defaultValue = 0);
	
	/** Converts a String to a whole number.
	@return
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static int parseInt(const String& val, int defaultValue = 0);

	/** Converts a String to a whole number.
	@return
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static unsigned int parseUnsignedInt(const String& val, unsigned int defaultValue = 0);

	/** Converts a String to a whole number.
	@return
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static long parseLong(const String& val, long defaultValue = 0);

    /** Converts a String to a whole number.
    @return
    0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static u2int64 parseInt64(const String& val, u2int64 defaultValue = 0);

	/** Converts a String to a whole number.
	@return
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static unsigned long parseUnsignedLong(const String& val, unsigned long defaultValue = 0);

    /** Converts a String to a whole number.
    @return
    0.0 if the value could not be parsed, otherwise the numeric version of the String.
    */
    static u2uint64 parseUnsignedInt64(const String& val, u2uint64 defaultValue = 0);

	/** Converts a String to size_t.
	@return
	defaultValue if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static size_t parseSizeT(const String& val, size_t defaultValue = 0);

	/** Converts a String to a boolean.
	@remarks
	Returns true if case-insensitive match of the start of the string
	matches "true", "yes", "1", or "on", false if "false", "no", "0"
	or "off".
	*/
	static bool parseBool(const String& val, bool defaultValue = 0);

    /** Parses a Vector2 out of a String.
    @remarks
    Format is "x y" ie. 2 Real components, space delimited. Failure to parse returns
    Vector2::ZERO.
    */
    static cocos2d::Vec2 parseVector2(const String& val, const cocos2d::Vec2& defaultValue = cocos2d::Vec2::ZERO);

	/** Checks the String is a valid number value. */
	static bool isNumber(const String& val);

	//-----------------------------------------------------------------------
	static String getDefaultStringLocale(void) { return m_sDefaultStringLocale; }
	//-----------------------------------------------------------------------
	static void setUseLocale(bool useLocale) { m_sUseLocale = useLocale; }
	//-----------------------------------------------------------------------
	static bool isUseLocale() { return m_sUseLocale; }

protected:
	static String		m_sDefaultStringLocale;
	static std::locale	m_sLocale;
	static bool			m_sUseLocale;
};


#if U2_COMPILER == U2_COMPILER_GNUC && U2_COMP_VER >= 310 && !defined(STLPORT)
#   if U2_COMP_VER < 430
typedef ::__gnu_cxx::hash< _StringBase > _StringHash;
#   else
typedef ::std::tr1::hash< _StringBase > _StringHash;
#   endif
#elif U2_COMPILER == U2_COMPILER_CLANG
#   if defined(_LIBCPP_VERSION)
typedef ::std::hash< _StringBase > _StringHash;
#   else
typedef ::std::tr1::hash< _StringBase > _StringHash;
#   endif
#elif U2_COMPILER == U2_COMPILER_MSVC && U2_COMP_VER >= 1600 && !defined(STLPORT) // VC++ 10.0
typedef ::std::tr1::hash< _StringBase > _StringHash;
#elif !defined( _STLP_HASH_FUN_H )
typedef stdext::hash_compare< _StringBase, std::less< _StringBase > > _StringHash;
#else
typedef std::hash< _StringBase > _StringHash;
#endif


/** @} */
/** @} */


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2StringUtil_H__
