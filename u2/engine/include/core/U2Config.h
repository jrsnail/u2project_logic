#ifndef __U2Config_H__
#define __U2Config_H__


// ////////////////////////// const ////////////////////////////////
#define U2_OFF                          0
#define U2_ON                           1




// ////////////////////////// memory //////////////////////////////
#define U2_MEMORY_ALLOCATOR_STD         1
#define U2_MEMORY_ALLOCATOR_NED         2

#ifndef U2_MEMORY_ALLOCATOR
#   define U2_MEMORY_ALLOCATOR          U2_MEMORY_ALLOCATOR_STD
#endif

// Whether to use the custom memory allocator in STL containers
#ifndef U2_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
#   define U2_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR    U2_OFF
#endif

//if you want to make Ogre::String use the custom memory allocator then set:
//#define OGRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR 1
// Doing this will mean Ogre's strings will not be compatible with std::string however
#ifndef U2_STRING_USE_CUSTOM_MEMORY_ALLOCATOR
#   define U2_STRING_USE_CUSTOM_MEMORY_ALLOCATOR        U2_OFF
#endif

#ifndef U2_MEMORY_TRACKER
#    define U2_MEMORY_TRACKER           U2_ON
#endif

// ////////////////////////// real //////////////////////////////
#ifndef U2_DOUBLE_PRECISION
#    define U2_DOUBLE_PRECISION         U2_OFF
#endif

// ////////////////////////// Exception //////////////////////////////
#ifndef U2_NO_EXCEPTIONS
#    define U2_NO_EXCEPTIONS            U2_OFF
#endif

// ////////////////////////// wchar //////////////////////////////
#ifndef U2_WCHAR_T_STRINGS
#    define U2_WCHAR_T_STRINGS          U2_OFF
#endif

// ////////////////////////// endian //////////////////////////////
#define U2_ENDIAN_LITTLE                1
#define U2_ENDIAN_BIG                   2

#ifndef U2_ENDIAN
#   define U2_ENDIAN                    U2_ENDIAN_LITTLE
#endif


// ////////////////////////// archive //////////////////////////////
#ifndef U2_NO_ZIP_ARCHIVE
#   define U2_NO_ZIP_ARCHIVE            U2_ON
#endif

// ////////////////////////// thread //////////////////////////////
/** Provider for threading functionality, there are 4 options.
U2_THREAD_PROVIDER = 0
No support for threading.
U2_THREAD_PROVIDER = 1
Boost libraries provide threading functionality.
U2_THREAD_PROVIDER = 2
C++11 libraries provide threading functionality.
U2_THREAD_PROVIDER = 3
TBB library provides threading functionality.
*/
#ifndef U2_THREAD_PROVIDER
#   define U2_THREAD_PROVIDER           2
#endif



#endif
