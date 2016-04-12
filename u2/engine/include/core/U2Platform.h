#ifndef __U2Platform_H__
#define __U2Platform_H__

#include "U2Config.h"




// ////////////////////////// platform //////////////////////////////
#define U2_PLATFORM_WIN32           1
#define U2_PLATFORM_LINUX           2
#define U2_PLATFORM_APPLE           3
#define U2_PLATFORM_APPLE_IOS       4
#define U2_PLATFORM_ANDROID         5
#define U2_PLATFORM_WINRT           6


#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(__ANDROID__)
#   include <sdkddkver.h>
#   if defined(WINAPI_FAMILY)
#       include <winapifamily.h>
#       if WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#           define U2_PLATFORM      U2_PLATFORM_WINRT
#       else
#           define U2_PLATFORM      U2_PLATFORM_WIN32
#       endif
#   else
#       define U2_PLATFORM          U2_PLATFORM_WIN32
#   endif
#   define __U2_WINRT_STORE     (U2_PLATFORM == U2_PLATFORM_WINRT && WINAPI_FAMILY == WINAPI_FAMILY_APP)        // WindowsStore 8.0 and 8.1
#   define __U2_WINRT_PHONE     (U2_PLATFORM == U2_PLATFORM_WINRT && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)  // WindowsPhone 8.0 and 8.1
#   define __U2_WINRT_PHONE_80  (U2_PLATFORM == U2_PLATFORM_WINRT && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP && _WIN32_WINNT <= _WIN32_WINNT_WIN8) // Windows Phone 8.0 often need special handling, while 8.1 is OK
#elif defined(__APPLE_CC__)
// Device                                                     Simulator
// Both requiring OS version 6.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
#       define U2_PLATFORM          U2_PLATFORM_APPLE_IOS
#   else
#       define U2_PLATFORM          U2_PLATFORM_APPLE
#   endif
#elif defined(__ANDROID__)
#    define U2_PLATFORM             U2_PLATFORM_ANDROID
#else
#   define U2_PLATFORM              U2_PLATFORM_LINUX
#endif



// ////////////////////////// arch type //////////////////////////////

#define U2_ARCHITECTURE_32          1
#define U2_ARCHITECTURE_64          2

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) \
    || defined(__ia64__) || defined(__s390__) || defined(__s390x__) || defined(__arm64__) \
    || defined(__aarch64__) || defined(__mips64) || defined(__mips64_)
#   define U2_ARCH_TYPE             U2_ARCHITECTURE_64
#else
#   define U2_ARCH_TYPE             U2_ARCHITECTURE_32
#endif


// ////////////////////////// compiler //////////////////////////////


#define U2_COMPILER_MSVC            1
#define U2_COMPILER_GNUC            2
#define U2_COMPILER_WINSCW          4           // symbian
#define U2_COMPILER_GCCE            5
#define U2_COMPILER_CLANG           6


/* Finds the compiler type and version.
*/
#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined(__ANDROID__) // We are using NVTegra
#   define U2_COMPILER              U2_COMPILER_GNUC
#   define U2_COMP_VER              470
#elif defined( __GCCE__ )
#   define U2_COMPILER              U2_COMPILER_GCCE
#   define U2_COMP_VER              _MSC_VER
//#    include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE
#elif defined( __WINSCW__ )
#   define U2_COMPILER              U2_COMPILER_WINSCW
#   define U2_COMP_VER              _MSC_VER
#elif defined( _MSC_VER )
#   define U2_COMPILER              U2_COMPILER_MSVC
#   define U2_COMP_VER              _MSC_VER
#elif defined( __clang__ )
#   define U2_COMPILER              U2_COMPILER_CLANG
#   define U2_COMP_VER              (((__clang_major__) * 100) + \
                                    (__clang_minor__ * 10) + \
                                    __clang_patchlevel__)
#elif defined( __GNUC__ )
#   define U2_COMPILER              U2_COMPILER_GNUC
#   define U2_COMP_VER              (((__GNUC__) * 100) + \
                                    (__GNUC_MINOR__ * 10) + \
                                    __GNUC_PATCHLEVEL__)
#else
#   pragma error "No known compiler. Abort! Abort!"
#endif



// ////////////////////////// warnings //////////////////////////////
// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define U2_QUOTE_INPLACE(x)         # x
#define U2_QUOTE(x)                 U2_QUOTE_INPLACE(x)
#define U2_WARN( x )                message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )




// ////////////////////////// Platform Name //////////////////////////////
#if U2_PLATFORM == U2_PLATFORM_WIN32
#    define U2_PLATFORM_NAME            "Win"
#elif U2_PLATFORM == U2_PLATFORM_LINUX
#    define U2_PLATFORM_NAME            "Linux"
#elif U2_PLATFORM == U2_PLATFORM_APPLE
#    define U2_PLATFORM_NAME            "Apple"
#elif U2_PLATFORM == U2_PLATFORM_APPLE_IOS
#    define U2_PLATFORM_NAME            "iOS"
#elif U2_PLATFORM == U2_PLATFORM_ANDROID
#    define U2_PLATFORM_NAME            "Android"
#endif


// ////////////////////////// DLL //////////////////////////////
#if U2_PLATFORM == U2_PLATFORM_WIN32
#    ifdef U2_EXPORTS
#            define _U2Export               __declspec(dllexport)
#            define _U2Private
#    else
#       if defined( __MINGW32__ )
#            define _U2Export
#       else
#            define _U2Export               __declspec(dllimport)
#       endif
#            define _U2Private
#    endif
#elif U2_PLATFORM == U2_PLATFORM_LINUX || U2_PLATFORM == U2_PLATFORM_APPLE || U2_PLATFORM == U2_PLATFORM_APPLE_IOS || \
    U2_PLATFORM == U2_PLATFORM_ANDROID
#    ifdef U2_EXPORTS
#        define _U2Export                   __attribute__ ((visibility("default")))
#        define _U2Private                  __attribute__ ((visibility("hidden")))
#        define U2_GCC_VISIBILITY
#    else
#        define _U2Export
#        define _U2Private
#    endif
#endif


// ////////////////////////// FORCEINLINE //////////////////////////////
#if U2_COMPILER == U2_COMPILER_MSVC
#   if U2_COMP_VER >= 1200
#       define FORCEINLINE          __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE          __inline
#   endif
#else
#   define FORCEINLINE              __inline
#endif


// ////////////////////////// no retrun //////////////////////////////
#if U2_COMPILER == U2_COMPILER_MSVC
#	define U2_NORETURN              __declspec(noreturn)
#elif U2_COMPILER == U2_COMPILER_GNUC || U2_COMPILER == U2_COMPILER_CLANG
#	define U2_NORETURN              __attribute__((noreturn))
#else
#	define U2_NORETURN
#endif


// ////////////////////////// deprecated //////////////////////////////
#if U2_COMPILER == U2_COMPILER_MSVC
#   define U2_DEPRECATED            __declspec(deprecated)
#elif U2_COMPILER == U2_COMPILER_GNUC || U2_COMPILER == U2_COMPILER_CLANG
#   define U2_DEPRECATED            __attribute__ ((deprecated))
#else
#   pragma message("WARNING: You need to implement U2_DEPRECATED for this compiler")
#   define U2_DEPRECATED
#endif


// ////////////////////////// Debug //////////////////////////////
#if U2_PLATFORM == U2_PLATFORM_WIN32
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define U2_DEBUG_MODE             1
#   else
#       define U2_DEBUG_MODE             0
#   endif
#else
#   ifdef DEBUG
#       define U2_DEBUG_MODE             1
#   else
#       define U2_DEBUG_MODE             0
#   endif
#endif


// ////////////////////////// Static Lib //////////////////////////////
#if U2_PLATFORM == U2_PLATFORM_ANDROID || U2_PLATFORM == U2_PLATFORM_APPLE_IOS
#    define U2_STATIC_LIB
#endif


// ////////////////////////// Library suffixes //////////////////////////////
// "_d" for debug builds, nothing otherwise
#if U2_DEBUG_MODE
#   define U2_BUILD_SUFFIX               "_d"
#else
#   define U2_BUILD_SUFFIX               ""
#endif


// ////////////////////////// Android Settings //////////////////////////////
#if U2_PLATFORM == U2_PLATFORM_ANDROID
// A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifndef CLOCKS_PER_SEC
#       define CLOCKS_PER_SEC  1000
#   endif
#endif


// ////////////////////////// locale //////////////////////////////
// Set the default locale for strings
#if U2_PLATFORM == U2_PLATFORM_ANDROID
//  Locales are not supported by the C lib you have to go through JNI.
#   define U2_DEFAULT_LOCALE				""
#else
#   define U2_DEFAULT_LOCALE				"C"
#endif


// Disable these warnings (too much noise)
#if U2_COMPILER == U2_COMPILER_MSVC
#   ifndef _CRT_SECURE_NO_WARNINGS
#       define _CRT_SECURE_NO_WARNINGS
#   endif
#   ifndef _SCL_SECURE_NO_WARNINGS
#       define _SCL_SECURE_NO_WARNINGS
#   endif
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4786)
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4503)
// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)
// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275)
// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception
// specifications in the ANSI C++ draft.
#   pragma warning (disable : 4290)
// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning (disable: 4661)
// disable: deprecation warnings when using CRT calls in VC8
// These show up on all C runtime lib code in VC8, disable since they clutter
// the warnings with things we may not be able to do anything about (e.g.
// generated code from nvparse etc). I doubt very much that these calls
// will ever be actually removed from VC anyway, it would break too much code.
#   pragma warning (disable: 4996)
// disable: "conditional expression constant", always occurs on 
// OGRE_MUTEX_CONDITIONAL when no threading enabled
#   pragma warning (disable : 201)
// disable: "unreferenced formal parameter"
// Many versions of VC have bugs which generate this error in cases where they shouldn't
#   pragma warning (disable : 4100)
// disable: "behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized"
// We have this issue in OgreMemorySTLAlloc.h - so we see it over and over
#   pragma warning (disable : 4345)
#endif


#endif
