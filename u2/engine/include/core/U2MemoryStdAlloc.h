#ifndef __U2MemoryStdAlloc_H__
#define __U2MemoryStdAlloc_H__


#include "U2Prerequisites.h"
#include "U2AlignedAllocator.h"
#include "U2MemoryTracker.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


#if U2_MEMORY_ALLOCATOR == U2_MEMORY_ALLOCATOR_STD

/** \addtogroup Core
*  @{
*/
/** \addtogroup Memory
*  @{
*/

/**	A "standard" allocation policy for use with AllocatedObject and 
	STLAllocator. This is the class that actually does the allocation
	and deallocation of physical memory, and is what you will want to 
	provide a custom version of if you wish to change how memory is allocated.
	@par
	This class just delegates to the global malloc/free.
*/
class _U2Export StdAllocPolicy
{
public:
	static inline void* allocateBytes(size_t count, 
#if U2_MEMORY_TRACKER
		const char* file = 0, int line = 0, const char* func = 0
#else
		const char*  = 0, int  = 0, const char* = 0
#endif
        )
	{
		void* ptr = new unsigned char[count];
#if U2_MEMORY_TRACKER
		// this alloc policy doesn't do pools
		MemoryTracker::get()._recordAlloc(ptr, count, 0, file, line, func);
#endif
		return ptr;
	}

	static inline void deallocateBytes(void* ptr)
	{
#if U2_MEMORY_TRACKER
		MemoryTracker::get()._recordDealloc(ptr);
#endif
		delete[]((unsigned char*)ptr);
	}

	/// Get the maximum size of a single allocation
	static inline size_t getMaxAllocationSize()
	{
		return std::numeric_limits<size_t>::max();
	}

private:
	// no instantiation
	StdAllocPolicy()
	{ }
};




/**	A "standard" allocation policy for use with AllocatedObject and 
	STLAllocator, which aligns memory at a given boundary (which should be
	a power of 2). This is the class that actually does the allocation
	and deallocation of physical memory, and is what you will want to 
	provide a custom version of if you wish to change how memory is allocated.
	@par
	This class just delegates to the global malloc/free, via AlignedMemory.
	@note
	template parameter Alignment equal to zero means use default
	platform dependent alignment.

*/
template <size_t Alignment = 0>
class StdAlignedAllocPolicy
{
public:
	// compile-time check alignment is available.
	typedef int IsValidAlignment
		[Alignment <= 128 && ((Alignment & (Alignment-1)) == 0) ? +1 : -1];

	static inline void* allocateBytes(size_t count, 
#if U2_MEMORY_TRACKER
		const char* file = 0, int line = 0, const char* func = 0
#else
		const char*  = 0, int  = 0, const char* = 0
#endif
        )
	{
		void* ptr = Alignment ? AlignedMemory::allocate(count, Alignment)
			: AlignedMemory::allocate(count);
#if U2_MEMORY_TRACKER
		// this alloc policy doesn't do pools
		MemoryTracker::get()._recordAlloc(ptr, count, 0, file, line, func);
#endif
		return ptr;
	}

	static inline void deallocateBytes(void* ptr)
	{
#if U2_MEMORY_TRACKER
		MemoryTracker::get()._recordDealloc(ptr);
#endif
		AlignedMemory::deallocate(ptr);
	}

	/// Get the maximum size of a single allocation
	static inline size_t getMaxAllocationSize()
	{
		return std::numeric_limits<size_t>::max();
	}

private:
	// No instantiation
	StdAlignedAllocPolicy()
	{ }
};

#endif
/** @} */
/** @} */


U2EG_NAMESPACE_END


#include "U2HeaderSuffix.h"

#endif // __MemoryStdAlloc_H__
