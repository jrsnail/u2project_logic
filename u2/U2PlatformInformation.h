#ifndef __U2PlatformInformation_H__
#define __U2PlatformInformation_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_BEGIN


//
// TODO: Puts following macros into OgrePlatform.h?
//

/* Initial CPU stuff to set.
*/
#define U2_CPU_UNKNOWN    0
#define U2_CPU_X86        1
#define U2_CPU_PPC        2
#define U2_CPU_ARM        3
#define U2_CPU_MIPS       4

/* Find CPU type
*/
#if (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) || \
    (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
#   define U2_CPU U2_CPU_X86

#elif U2_PLATFORM == U2_PLATFORM_APPLE && defined(__BIG_ENDIAN__)
#   define U2_CPU U2_CPU_PPC
#elif U2_PLATFORM == U2_PLATFORM_APPLE
#   define U2_CPU U2_CPU_X86
#elif U2_PLATFORM == U2_PLATFORM_APPLE_IOS && (defined(__i386__) || defined(__x86_64__))
#   define U2_CPU U2_CPU_X86
#elif defined(__arm__) || defined(_M_ARM) || defined(__arm64__) || defined(__aarch64__)
#   define U2_CPU U2_CPU_ARM
#elif defined(__mips64) || defined(__mips64_)
#   define U2_CPU U2_CPU_MIPS
#else
#   define U2_CPU U2_CPU_UNKNOWN
#endif

/* Find how to declare aligned variable.
*/
#if U2_COMPILER == U2_COMPILER_MSVC
#   define U2_ALIGNED_DECL(type, var, alignment)  __declspec(align(alignment)) type var

#elif (U2_COMPILER == U2_COMPILER_GNUC) || (U2_COMPILER == U2_COMPILER_CLANG)
#   define U2_ALIGNED_DECL(type, var, alignment)  type var __attribute__((__aligned__(alignment)))

#else
#   define U2_ALIGNED_DECL(type, var, alignment)  type var
#endif

/** Find perfect alignment (should supports SIMD alignment if SIMD available)
*/
#if U2_CPU == U2_CPU_X86
#   define U2_SIMD_ALIGNMENT  16

#else
#   define U2_SIMD_ALIGNMENT  16
#endif

/* Declare variable aligned to SIMD alignment.
*/
#define U2_SIMD_ALIGNED_DECL(type, var)   U2_ALIGNED_DECL(type, var, U2_SIMD_ALIGNMENT)

/* Define whether or not Ogre compiled with SSE supports.
*/
#if   U2_DOUBLE_PRECISION == 0 && U2_CPU == U2_CPU_X86 && U2_COMPILER == U2_COMPILER_MSVC && \
    U2_PLATFORM != U2_PLATFORM_NACL
#   define __U2_HAVE_SSE  1
#elif U2_DOUBLE_PRECISION == 0 && U2_CPU == U2_CPU_X86 && (U2_COMPILER == U2_COMPILER_GNUC || U2_COMPILER == U2_COMPILER_CLANG) && \
      U2_PLATFORM != U2_PLATFORM_APPLE_IOS && U2_PLATFORM != U2_PLATFORM_NACL
#   define __U2_HAVE_SSE  1
#endif

/* Define whether or not Ogre compiled with VFP support.
 */
#if U2_DOUBLE_PRECISION == 0 && U2_CPU == U2_CPU_ARM && (U2_COMPILER == U2_COMPILER_GNUC || U2_COMPILER == U2_COMPILER_CLANG) && defined(__ARM_ARCH_6K__) && defined(__VFP_FP__)
#   define __U2_HAVE_VFP  1
#endif

/* Define whether or not Ogre compiled with NEON support.
 */
#if U2_DOUBLE_PRECISION == 0 && U2_CPU == U2_CPU_ARM && (U2_COMPILER == U2_COMPILER_GNUC || U2_COMPILER == U2_COMPILER_CLANG) && defined(__ARM_ARCH_7A__) && defined(__ARM_NEON__)
#   define __U2_HAVE_NEON  1
#endif

/* Define whether or not Ogre compiled with MSA support.
 */
#if U2_DOUBLE_PRECISION == 0 && U2_CPU == U2_CPU_MIPS && (U2_COMPILER == U2_COMPILER_GNUC || U2_COMPILER == U2_COMPILER_CLANG) && defined(__mips_msa)
#   define __U2_HAVE_MSA  1
#endif

#ifndef __U2_HAVE_SSE
#   define __U2_HAVE_SSE  0
#endif

#ifndef __U2_HAVE_VFP
#   define __U2_HAVE_VFP  0
#endif

#ifndef __U2_HAVE_NEON
#   define __U2_HAVE_NEON  0
#endif

#ifndef __U2_HAVE_MSA
#   define __U2_HAVE_MSA  0
#endif



class Log;


/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/


/** Class which provides the run-time platform information Ogre runs on.
    @remarks
        Ogre is designed to be platform-independent, but some platform
        and run-time environment specific optimised functions are built-in
        to maximise performance, and those special optimised routines are
        need to determine run-time environment for select variant executing
        path.
    @par
        This class manages that provides a couple of functions to determine
        platform information of the run-time environment.
    @note
        This class is supposed to use by advanced user only.
*/
class _U2Export PlatformInformation
{
public:

    /// Enum describing the different CPU features we want to check for, platform-dependent
    enum CpuFeatures
    {
#if U2_CPU == U2_CPU_X86
        CPU_FEATURE_SSE         = 1 << 0,
        CPU_FEATURE_SSE2        = 1 << 1,
        CPU_FEATURE_SSE3        = 1 << 2,
        CPU_FEATURE_MMX         = 1 << 3,
        CPU_FEATURE_MMXEXT      = 1 << 4,
        CPU_FEATURE_3DNOW       = 1 << 5,
        CPU_FEATURE_3DNOWEXT    = 1 << 6,
        CPU_FEATURE_CMOV        = 1 << 7,
        CPU_FEATURE_TSC         = 1 << 8,
        CPU_FEATURE_FPU         = 1 << 9,
        CPU_FEATURE_PRO         = 1 << 10,
        CPU_FEATURE_HTT         = 1 << 11,
#elif U2_CPU == U2_CPU_ARM
        CPU_FEATURE_VFP         = 1 << 12,
        CPU_FEATURE_NEON        = 1 << 13,
#elif U2_CPU == U2_CPU_MIPS
        CPU_FEATURE_MSA         = 1 << 14,
#endif

        CPU_FEATURE_NONE        = 0
    };

    /** Gets a string of the CPU identifier.
    @note
        Actual detecting are performs in the first time call to this function,
        and then all future calls with return internal cached value.
    */
    static const u2::String& getCpuIdentifier(void);

    /** Gets a or-masked of enum CpuFeatures that are supported by the CPU.
    @note
        Actual detecting are performs in the first time call to this function,
        and then all future calls with return internal cached value.
    */
    static u2uint32 getCpuFeatures(void);

    /** Gets whether a specific feature is supported by the CPU.
    @note
        Actual detecting are performs in the first time call to this function,
        and then all future calls with return internal cached value.
    */
    static bool hasCpuFeature(CpuFeatures feature);


    /** Write the CPU information to the passed in Log */
    static void log(Log* pLog);

};
/** @} */
/** @} */


U2EG_NAMESPACE_END

#endif  // __PlatformInformation_H__
