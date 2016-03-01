#ifndef __U2StdHeaders_H__
#define __U2StdHeaders_H__


#if defined (U2_GCC_VISIBILITY) && ((U2_PLATFORM == U2_PLATFORM_APPLE && !__LP64__) && U2_PLATFORM != U2_PLATFORM_APPLE_IOS)
/* Until libstdc++ for gcc 4.2 is released, we have to declare all
 * symbols in libstdc++.so externally visible, otherwise we end up
 * with them marked as hidden by -fvisible=hidden.
 *
 * See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=20218
 *
 * Due to a more strict linker included with Xcode 4, this is disabled on Mac OS X and iOS.
 * The reason? It changes the visibility of Boost functions.  The mismatch between visibility Boost when used in Ogre (default)
 * and Boost when compiled (hidden) results in mysterious link errors such as "Bad codegen, pointer diff".
 */
#   pragma GCC visibility push(default)
#endif


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <memory>


// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <iterator>
#include <utility>


// Note - not in the original STL, but exists in SGI STL and STLport
// For gcc 4.3 see http://gcc.gnu.org/gcc-4.3/changes.html
#if (U2_COMPILER == U2_COMPILER_GNUC) && !defined(STLPORT)
#   if U2_COMP_VER >= 430
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#   else
#       include <ext/hash_map>
#       include <ext/hash_set>
#   endif
#elif (U2_COMPILER == U2_COMPILER_CLANG)
#   if defined(_LIBCPP_VERSION)
#       include <unordered_map>
#       include <unordered_set>
#   else
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#   endif
#elif !defined(STLPORT)
#   if (U2_COMPILER == U2_COMPILER_MSVC) && _MSC_FULL_VER >= 150030729 // VC++ 9.0 SP1+
#    	include <unordered_map>
#    	include <unordered_set>
#   else
#   	error "Your compiler doesn't support unordered_set and unordered_map. Try to compile U2 with Boost or STLPort."
#	endif
#endif


#if U2_COMPILER == U2_COMPILER_GNUC && U2_COMP_VER >= 310 && !defined(STLPORT)
#   if U2_COMP_VER >= 430
#       define U2_HashMap           ::std::tr1::unordered_map
#       define U2_HashMultiMap      ::std::tr1::unordered_multimap
#       define U2_HashSet           ::std::tr1::unordered_set
#       define U2_HashMultiSet      ::std::tr1::unordered_multiset
#    else
#       define U2_HashMap           ::__gnu_cxx::hash_map
#       define U2_HashMultiMap      ::__gnu_cxx::hash_multimap
#       define U2_HashSet           ::__gnu_cxx::hash_set
#       define U2_HashMultiSet      ::__gnu_cxx::hash_multiset
#    endif
#elif U2_COMPILER == U2_COMPILER_CLANG
#    if defined(_LIBCPP_VERSION)
#       define U2_HashMap           ::std::unordered_map
#       define U2_HashMultiMap      ::std::unordered_multimap
#       define U2_HashSet           ::std::unordered_set
#       define U2_HashMultiSet      ::std::unordered_multiset
#    else
#       define U2_HashMap           ::std::tr1::unordered_map
#       define U2_HashMultiMap      ::std::tr1::unordered_multimap
#       define U2_HashSet           ::std::tr1::unordered_set
#       define U2_HashMultiSet      ::std::tr1::unordered_multiset
#    endif
#else
#   if U2_COMPILER == U2_COMPILER_MSVC && !defined(_STLP_MSVC)
#       if _MSC_FULL_VER >= 150030729 // VC++ 9.0 SP1+
#           define U2_HashMap       ::std::tr1::unordered_map
#           define U2_HashMultiMap  ::std::tr1::unordered_multimap
#           define U2_HashSet       ::std::tr1::unordered_set
#           define U2_HashMultiSet  ::std::tr1::unordered_multiset
#       else
#           define U2_HashMap       ::std::unordered_map
#           define U2_HashMultiMap  ::std::unordered_multimap
#           define U2_HashSet       ::std::unordered_set
#           define U2_HashMultiSet  ::std::unordered_multiset
#       endif
#   else
#       define U2_HashMap           ::std::unordered_map
#       define U2_HashMultiMap      ::std::unordered_multimap
#       define U2_HashSet           ::std::unordered_set
#       define U2_HashMultiSet      ::std::unordered_multiset
#   endif
#endif


// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>


// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


extern "C" {
#   include <sys/types.h>
#   include <sys/stat.h>
}

#if U2_PLATFORM == U2_PLATFORM_WIN32
#   undef min
#   undef max
#   if defined( __MINGW32__ )
#       include <unistd.h>
#   endif
#endif

#if U2_PLATFORM == U2_PLATFORM_LINUX || U2_PLATFORM == U2_PLATFORM_ANDROID
extern "C" {
#   include <unistd.h>
#   include <dlfcn.h>
}
#endif

#if U2_PLATFORM == U2_PLATFORM_APPLE || U2_PLATFORM == U2_PLATFORM_APPLE_IOS
extern "C" {
#   include <unistd.h>
#   include <sys/param.h>
#   include <CoreFoundation/CoreFoundation.h>
}
#endif



#include <stdlib.h>
#include <cctype>
#include <typeinfo>


#if defined (U2_GCC_VISIBILITY) && ((U2_PLATFORM == U2_PLATFORM_APPLE && !__LP64__) && U2_PLATFORM != U2_PLATFORM_APPLE_IOS)
#   pragma GCC visibility pop
#endif


#endif
