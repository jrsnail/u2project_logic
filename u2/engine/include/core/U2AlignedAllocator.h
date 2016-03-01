#ifndef __U2AlignedAllocator_H__
#define __U2AlignedAllocator_H__


#include "U2Prerequisites.h"


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/

/** \addtogroup Memory
*  @{
*/

/** Class to provide aligned memory allocate functionality.
@remarks
    All SIMD processing are friendly with aligned memory, and some SIMD routines
    are designed for working with aligned memory only. If the data are intended to
    use SIMD processing, it's need to be aligned for better performance boost.
    In additional, most time cache boundary aligned data also lead to better
    performance even if didn't used SIMD processing. So this class provides a couple
    of functions for allocate aligned memory.
@par
    Anyways, in general, you don't need to use this class directly, Ogre internally
    will take care with most SIMD and cache friendly optimisation if possible.
@par
    This isn't a "one-step" optimisation, there are a lot of underlying work to
    achieve performance boost. If you didn't know what are you doing or what there
    are going, just ignore this class.
@note
    This class intended to use by advanced user only.
*/
class _U2Export AlignedMemory
{
public:
    /** Allocate memory with given alignment.
        @param
            size The size of memory need to allocate.
        @param
            alignment The alignment of result pointer, must be power of two
            and in range [1, 128].
        @return
            The allocated memory pointer.
        @par
            On failure, exception will be throw.
    */
    static void* allocate(size_t size, size_t alignment);

    /** Allocate memory with default platform dependent alignment.
        @remarks
            The default alignment depend on target machine, this function
            guarantee aligned memory according with SIMD processing and
            cache boundary friendly.
        @param
            size The size of memory need to allocate.
        @return
            The allocated memory pointer.
        @par
            On failure, exception will be throw.
    */
    static void* allocate(size_t size);

    /** Deallocate memory that allocated by this class.
        @param
            p Pointer to the memory allocated by this class or <b>NULL</b> pointer.
        @par
            On <b>NULL</b> pointer, nothing happen.
    */
    static void deallocate(void* p);
};

/** @} */
/** @} */

U2EG_NAMESPACE_END

#endif  // __U2AlignedAllocator_H__
