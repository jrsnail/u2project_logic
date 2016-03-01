﻿//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Prerequisites__
#define __U2Prerequisites__


#include "U2Platform.h"



// ////////////////////// namespace begin ///////////////////////
#define U2EG_NAMESPACE_NAME         u2
#define U2EG_NAMESPACE_BEGIN        namespace U2EG_NAMESPACE_NAME {
#define U2EG_NAMESPACE_END          };
#define U2EG_NAMESPACE_USING        using namespace U2EG_NAMESPACE_NAME;
// ////////////////////// namespace end ///////////////////////


// ////////////////////// version begin ///////////////////////
#define U2_VERSION_MAJOR            1
#define U2_VERSION_MINOR            10
#define U2_VERSION_PATCH            0
#define U2_VERSION_SUFFIX           "unstable"
#define U2_VERSION_NAME             "TimeMachine"

#define U2_VERSION    ((U2_VERSION_MAJOR << 16) | (U2_VERSION_MINOR << 8) | U2_VERSION_PATCH)
// ////////////////////// version end ///////////////////////


// ////////////////////// data type begin ///////////////////////
namespace U2EG_NAMESPACE_NAME 
{
#if U2_DOUBLE_PRECISION == 1
/** Software floating point type.
@note Not valid as a pointer to GPU buffers / parameters
*/
typedef double                      u2real;
#else
/** Software floating point type.
@note Not valid as a pointer to GPU buffers / parameters
*/
typedef float                       u2real;
#endif

typedef signed char                 u2char;
#if U2_COMPILER == U2_COMPILER_MSVC
typedef wchar_t                     u2wchar;
#else
typedef wchar_t                     u2wchar;
#endif
typedef signed char                 u2byte;
typedef signed char                 u2int8;
typedef signed short                u2int16;
typedef signed int                  u2int32;
typedef unsigned char               u2uchar;
typedef unsigned char               u2ubyte;
typedef unsigned char               u2uint8;
typedef unsigned short              u2uint16;
typedef unsigned int                u2uint32;
#if U2_COMPILER == U2_COMPILER_MSVC
typedef unsigned __int64            u2uint64;
typedef __int64                     u2int64;
#else
typedef unsigned long long          u2uint64;
typedef long long                   u2int64;
#endif
}
// ////////////////////// data type end ///////////////////////




#include "U2StdHeader.h"


#endif /* defined(__U2Prerequisites__) */
