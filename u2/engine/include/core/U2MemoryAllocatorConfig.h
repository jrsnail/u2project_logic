#ifndef __U2MemoryAllocatorConfig_H__
#define __U2MemoryAllocatorConfig_H__

#include "U2MemoryAllocatedObject.h"
#include "U2HeaderPrefix.h"


/** @file

	This file configures Ogre's memory allocators. You can modify this
	file to alter the allocation routines used for Ogre's main objects.

	When customising memory allocation, all you need to do is provide one or
	more custom allocation policy classes. These classes need to implement:

	@code
	// Allocate bytes - file/line/func information should be optional, 
	// will be provided when available but not everywhere (e.g. release mode, STL allocations)
	static inline void* allocateBytes(size_t count, const char* file = 0, int line = 0, const char* func = 0);
	// Free bytes
	static inline void deallocateBytes(void* ptr);
	// Return the max number of bytes available to be allocated in a single allocation
	static inline size_t getMaxAllocationSize();
	@endcode

	Policies are then used as implementations for the wrapper classes and macros 
	which call them. AllocatedObject for example provides the hooks to override
	the new and delete operators for a class and redirect the functionality to the
	policy. STLAllocator is a class which is provided to STL containers in order
	to hook up allocation of the containers members to the allocation policy.
	@par
	In addition to linking allocations to policies, this class also defines
	a number of macros to allow debugging information to be passed along with
	allocations, such as the file and line number they originate from. It's
	important to realise that we do not redefine the 'new' and 'delete' symbols 
	with macros, because that's very difficult to consistently do when other
	libraries are also trying to do the same thing; instead we use dedicated
	'U2_' prefixed macros. See U2_NEW and related items.
	@par
	The base macros you can use are listed below, in order of preference and 
	with their conditions stated:
	<ul>
	<li>U2_NEW - use to allocate an object which have custom new/delete operators
		to handle custom allocations, usually this means it's derived from U2EG_NAMESPACE_NAME::AllocatedObject.
		Free the memory using U2_DELETE. You can in fact use the regular new/delete
		for these classes but you won't get any line number debugging if you do.
		The memory category is automatically derived for these classes; for all other
		allocations you have to specify it.
	</li>
	<li>U2_NEW_T - use to allocate a single class / struct that does not have custom
		new/delete operators, either because it is non-virtual (Vector3, Quaternion),
		or because it is from an external library (e.g. STL). You must
		deallocate with U2_DELETE_T if you expect the destructor to be called.
		You may free the memory using U2_FREE if you are absolutely sure there
		is no destructor to be called. 
		These macros ensure that constructors and destructors are called correctly 
		even though the memory originates externally (via placement new). Also note 
		that you have to specify the type and memory category so that the correct 
		allocator can be derived, when both allocating
		and freeing.
	</li>
	<li>U2_NEW_ARRAY_T - as U2_NEW_T except with an extra parameter to construct
		multiple instances in contiguous memory. Again constructors and destructors
		are called. Free with U2_DELETE_ARRAY_T.
	</li>
	<li>U2_ALLOC_T - use to allocate a set of primitive types conveniently with type safety.
	This <i>can</i> also be used for classes and structs but it is <b>imperative</b> that
	you understand that neither the constructor nor the destructor will be called. 
	Sometimes you want this because it's more efficient just to grab/free a chunk of
	memory without having to iterate over each element constructing / destructing.
	Free the memory with U2_FREE. </li>
	<li>U2_MALLOC - the most raw form of allocation, just a set of bytes.
		Use U2_FREE to release.</li>
	<li>_SIMD and _ALIGN variants - all of the above have variations which allow
		aligned memory allocations. The _SIMD versions align automatically to the
		SIMD requirements of your platform, the _ALIGN variants allow user-defined
		alignment to be specified. </li>
	</ul>
	Here are some examples:
	@code
	/// AllocatedObject subclass, with custom operator new / delete
	AllocatedClass* obj = U2_NEW AllocatedClass();
	U2_DELETE obj;
	AllocatedClass* array = U2_NEW AllocatedClass[10];
	U2_DELETE [] obj;
	/// Non-virtual or external class, constructors / destructors called
	ExternalClass* obj = U2_NEW_T(ExternalClass, MEMCATEGORY_GENERAL)(constructorArgs);
	U2_DELETE_T(obj, ExternalClass, MEMCATEGORY_GENERAL);
	ExternalClass* obj = U2_NEW_ARRAY_T(ExternalClass, 10, MEMCATEGORY_GENERAL);
	U2_DELETE_ARRAY_T(obj, NonVirtualClass, 10, MEMCATEGORY_GENERAL);
	/// Primitive types
	long* pLong = U2_ALLOC_T(long, 10, MEMCATEGORY_GENERAL);
	U2_FREE(pLong, MEMCATEGORY_GENERAL);
	/// Primitive type with constructor (you can mismatch U2_NEW_T and U2_FREE because no destructor)
	long* pLong = U2_NEW_T(long, MEMCATEGORY_GENERAL)(0);
	U2_FREE(pLong, MEMCATEGORY_GENERAL);
	/// Raw memory
	void* pVoid = U2_MALLOC(1024, MEMCATEGORY_GENERAL);
	U2_FREE(pVoid, MEMCATEGORY_GENERAL);
	@endcode
	U2_ALLOC_T is also the route to go for allocating real primitive types like
	int & float. You free the memory using U2_FREE, and both variants have SIMD
	and custom alignment variants.
*/


namespace U2EG_NAMESPACE_NAME
{
	/** A set of categories that indicate the purpose of a chunk of memory
		being allocated.
		These categories will be provided at allocation time in order to allow
		the allocation policy to vary its behaviour if it wishes. This allows you
		to use a single policy but still have variant behaviour. The level of
		control it gives you is at a higher level than assigning different
		policies to different classes, but is the only control you have over
		general allocations that are primitive types.
	*/
	enum MemoryCategory
	{
		/// General purpose
		MEMCATEGORY_GENERAL = 0,
		/// Scripting
		MEMCATEGORY_SCRIPTING = 1,

		// sentinel value, do not use 
		MEMCATEGORY_COUNT = 2
	};
}




#if U2_MEMORY_ALLOCATOR == U2_MEMORY_ALLOCATOR_STD
#   include "U2MemoryStdAlloc.h"
	namespace U2EG_NAMESPACE_NAME
	{
		// configure default allocators based on the options above
		// notice how we're not using the memory categories here but still roughing them out
		// in your allocators you might choose to create different policies per category

		// configurable category, for general malloc
		// notice how we ignore the category here
		template <MemoryCategory Cat> class CategorisedAllocPolicy : public StdAllocPolicy{};
		template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public StdAlignedAllocPolicy<align>{};

		// if you wanted to specialise the allocation per category, here's how it might work:
		// template <> class CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> : public YourSceneObjectAllocPolicy{};
		// template <size_t align> class CategorisedAlignAllocPolicy<MEMCATEGORY_SCENE_OBJECTS, align> : public YourSceneObjectAllocPolicy<align>{};

	}
#else
	
// your allocators here?

#endif


namespace U2EG_NAMESPACE_NAME
{
	// Useful shortcuts
	typedef CategorisedAllocPolicy<U2EG_NAMESPACE_NAME::MEMCATEGORY_GENERAL>       GeneralAllocPolicy;

	// Now define all the base classes for each allocation
	typedef AllocatedObject<GeneralAllocPolicy>                                    GeneralAllocatedObject;


	// Per-class allocators defined here
	// NOTE: small, non-virtual classes should not subclass an allocator
	// the virtual function table could double their size and make them less efficient
	// use primitive or STL allocators / deallocators for those
	

	// Containers (by-value only)
	// Will  be of the form:
	// typedef STLAllocator<T, DefaultAllocPolicy, Category> TAlloc;
	// for use in vector<T, TAlloc>::type 
}


// Util functions
namespace U2EG_NAMESPACE_NAME
{
	/** Utility function for constructing an array of objects with placement new,
		without using new[] (which allocates an undocumented amount of extra memory
		and so isn't appropriate for custom allocators).
	*/
	template<typename T>
	T* constructN(T* basePtr, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			new ((void*)(basePtr + i)) T();
		}
		return basePtr;
	}
}


// define macros
#if U2_DEBUG_MODE

/// Allocate a block of raw memory, and indicate the category of usage
#	define U2_MALLOC(bytes, category)           ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// Allocate a block of memory for a primitive type, and indicate the category of usage
#	define U2_ALLOC_T(T, count, category)       static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// Free the memory allocated with U2_MALLOC or U2_ALLOC_T. Category is required to be restated to ensure the matching policy is used
#	define U2_FREE(ptr, category)               ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)
/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#	define U2_NEW_T(T, category)                new (::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define U2_NEW_ARRAY_T(T, count, category)   ::U2EG_NAMESPACE_NAME::constructN(static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count)
/// Free the memory allocated with U2_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define U2_DELETE_T(ptr, T, category)        if(ptr) { (ptr)->~T(); ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr); }
/// Free the memory allocated with U2_NEW_ARRAY_T. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define U2_DELETE_ARRAY_T(ptr, T, count, category)      if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

// SIMD allocation
/// Allocate a block of raw memory aligned to SIMD boundaries, and indicate the category of usage
#	define U2_MALLOC_SIMD(bytes, category)              ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// Allocate a block of memory for a primitive type aligned to SIMD boundaries, and indicate the category of usage
#	define U2_ALLOC_T_SIMD(T, count, category)          static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// Free the memory allocated with either U2_MALLOC_SIMD or U2_ALLOC_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define U2_FREE_SIMD(ptr, category)                  ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr)
/// Allocate space for one primitive type, external type or non-virtual type aligned to SIMD boundaries
#	define U2_NEW_T_SIMD(T, category)                   new (::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// Allocate a block of memory for 'count' primitive types aligned to SIMD boundaries - do not use for classes that inherit from AllocatedObject
#	define U2_NEW_ARRAY_T_SIMD(T, count, category)      ::U2EG_NAMESPACE_NAME::constructN(static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count)
/// Free the memory allocated with U2_NEW_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define U2_DELETE_T_SIMD(ptr, T, category)           if(ptr){(ptr)->~T(); ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr);}
/// Free the memory allocated with U2_NEW_ARRAY_T_SIMD. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define U2_DELETE_ARRAY_T_SIMD(ptr, T, count, category)  if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr);}

// aligned allocation
/// Allocate a block of raw memory aligned to user defined boundaries, and indicate the category of usage
#	define U2_MALLOC_ALIGN(bytes, category, align)          ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// Allocate a block of memory for a primitive type aligned to user defined boundaries, and indicate the category of usage
#	define U2_ALLOC_T_ALIGN(T, count, category, align)      static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// Free the memory allocated with either U2_MALLOC_ALIGN or U2_ALLOC_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define U2_FREE_ALIGN(ptr, category, align)              ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr)
/// Allocate space for one primitive type, external type or non-virtual type aligned to user defined boundaries
#	define U2_NEW_T_ALIGN(T, category, align)               new (::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// Allocate a block of memory for 'count' primitive types aligned to user defined boundaries - do not use for classes that inherit from AllocatedObject
#	define U2_NEW_ARRAY_T_ALIGN(T, count, category, align)  ::U2EG_NAMESPACE_NAME::constructN(static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count)
/// Free the memory allocated with U2_NEW_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define U2_DELETE_T_ALIGN(ptr, T, category, align)       if(ptr){(ptr)->~T(); ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr);}
/// Free the memory allocated with U2_NEW_ARRAY_T_ALIGN. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define U2_DELETE_ARRAY_T_ALIGN(ptr, T, count, category, align) if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr);}

// new / delete for classes deriving from AllocatedObject (alignment determined by per-class policy)
// Also hooks up the file/line/function params
// Can only be used with classes that derive from AllocatedObject since customised new/delete needed
#	define U2_NEW               new (__FILE__, __LINE__, __FUNCTION__)
#	define U2_DELETE            delete


#else // !U2_DEBUG_MODE

/// Allocate a block of raw memory, and indicate the category of usage
#	define U2_MALLOC(bytes, category)           ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(bytes)
/// Allocate a block of memory for a primitive type, and indicate the category of usage
#	define U2_ALLOC_T(T, count, category)       static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))
/// Free the memory allocated with U2_MALLOC or U2_ALLOC_T. Category is required to be restated to ensure the matching policy is used
#	define U2_FREE(ptr, category)               ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)
/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#	define U2_NEW_T(T, category)                new (::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define U2_NEW_ARRAY_T(T, count, category)   ::U2EG_NAMESPACE_NAME::constructN(static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count)
/// Free the memory allocated with U2_NEW_T. Category is required to be restated to ensure the matching policy is used
#	define U2_DELETE_T(ptr, T, category)        if(ptr){(ptr)->~T(); ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}
/// Free the memory allocated with U2_NEW_ARRAY_T. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define U2_DELETE_ARRAY_T(ptr, T, count, category)   if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::U2EG_NAMESPACE_NAME::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

// SIMD allocation
/// Allocate a block of raw memory aligned to SIMD boundaries, and indicate the category of usage
#	define U2_MALLOC_SIMD(bytes, category)      ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(bytes)
/// Allocate a block of memory for a primitive type aligned to SIMD boundaries, and indicate the category of usage
#	define U2_ALLOC_T_SIMD(T, count, category)  static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))
/// Free the memory allocated with either U2_MALLOC_SIMD or U2_ALLOC_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define U2_FREE_SIMD(ptr, category)          ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr)
/// Allocate space for one primitive type, external type or non-virtual type aligned to SIMD boundaries
#	define U2_NEW_T_SIMD(T, category)           new (::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types aligned to SIMD boundaries - do not use for classes that inherit from AllocatedObject
#	define U2_NEW_ARRAY_T_SIMD(T, count, category)      ::U2EG_NAMESPACE_NAME::constructN(static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count)
/// Free the memory allocated with U2_NEW_T_SIMD. Category is required to be restated to ensure the matching policy is used
#	define U2_DELETE_T_SIMD(ptr, T, category)           if(ptr){(ptr)->~T(); ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr);}
/// Free the memory allocated with U2_NEW_ARRAY_T_SIMD. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define U2_DELETE_ARRAY_T_SIMD(ptr, T, count, category)  if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr);}

// aligned allocation
/// Allocate a block of raw memory aligned to user defined boundaries, and indicate the category of usage
#	define U2_MALLOC_ALIGN(bytes, category, align)      ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(bytes)
/// Allocate a block of memory for a primitive type aligned to user defined boundaries, and indicate the category of usage
#	define U2_ALLOC_T_ALIGN(T, count, category, align)  static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count)))
/// Free the memory allocated with either U2_MALLOC_ALIGN or U2_ALLOC_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define U2_FREE_ALIGN(ptr, category, align)          ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr)
/// Allocate space for one primitive type, external type or non-virtual type aligned to user defined boundaries
#	define U2_NEW_T_ALIGN(T, category, align)           new (::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types aligned to user defined boundaries - do not use for classes that inherit from AllocatedObject
#	define U2_NEW_ARRAY_T_ALIGN(T, count, category, align)      ::U2EG_NAMESPACE_NAME::constructN(static_cast<T*>(::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count))), count)
/// Free the memory allocated with U2_NEW_T_ALIGN. Category is required to be restated to ensure the matching policy is used
#	define U2_DELETE_T_ALIGN(ptr, T, category, align)           if(ptr){(ptr)->~T(); ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr);}
/// Free the memory allocated with U2_NEW_ARRAY_T_ALIGN. Category is required to be restated to ensure the matching policy is used, count and type to call destructor
#	define U2_DELETE_ARRAY_T_ALIGN(ptr, T, count, category, align)  if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::U2EG_NAMESPACE_NAME::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr);}

// new / delete for classes deriving from AllocatedObject (alignment determined by per-class policy)
#	define U2_NEW               new
#	define U2_DELETE            delete

#endif // U2_DEBUG_MODE


namespace U2EG_NAMESPACE_NAME
{
	/** Function which invokes U2_DELETE on a given pointer.
	@remarks
		Useful to pass custom deletion policies to external libraries (e. g. boost).
	*/
	template<typename T>
	void deletePtr(T* ptr)
	{
		U2_DELETE ptr;
	}

	template<typename T>
	void deleteTPtr(T* ptr)
	{
		U2_DELETE_T(ptr, T, MEMCATEGORY_GENERAL);
	}

	template<typename T>
	void freePtr(T* ptr)
	{
		U2_FREE(ptr, MEMCATEGORY_GENERAL);
	}
}



#include "U2HeaderSuffix.h"

#endif
