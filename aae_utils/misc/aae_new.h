#ifndef AAE_MEMORY_ALLOCATOR_H
#define AAE_MEMORY_ALLOCATOR_H
#include "aae_types.h"
#ifdef AAE_LINUX_PLATFORM
extern "C++" inline void* operator new(aae_size_t, void* __p) { return __p; }
#endif


template <typename T, class Arena>
static inline void aae_delete(T* object, Arena& arena)
{
	object->~T();
	arena->Deallocate(object);
}


template <typename T, class Arena>
static inline void aae_delete_array(T* array, Arena& arena)
{
	void* address = static_cast<void*>(reinterpret_cast<byte_ptr>(array) - sizeof(aae_size_t));
	aae_size_t array_size = *(static_cast<aae_size_t*>(address));
	for (i32 it = array_size - 1; it >= 0; --it)
		array[it].~T();
	arena->Deallocate(address);
}


template <typename T, class Arena>
static inline T* aae_new_array(Arena& arena, aae_size_t n_instances)
{
	void* address = arena->Allocate(sizeof(T) * n_instances + sizeof(aae_size_t));
	if (!address) return nullptr;
	*(static_cast<aae_size_t*>(address)) = n_instances;
	for (i32 it = 0; it < n_instances; ++it)
		new(static_cast<T*>(address) + it) T;
	return reinterpret_cast<T*>(static_cast<byte_ptr>(address) + sizeof(aae_size_t));
}


template <class Arena>
static inline void* aae_aligned_new(aae_size_t size, aae_size_t alignment, Arena& arena) /** aligned to a power of 2 **/
{
	aae_size_t offset = alignment - 1 + sizeof(void*);
	void* address = arena->Allocate(size + offset);
	if (!address) return nullptr;
	void* aligned_address = reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(address) + offset & ~(alignment - 1));
	*(reinterpret_cast<void**>(static_cast<byte_ptr>(aligned_address) - sizeof(void*))) = address;
	return aligned_address;
}


template <class Arena>
static inline void aae_aligned_delete(void* aligned_address, Arena& arena)
{
	arena->Deallocate(*(reinterpret_cast<void**>(static_cast<byte_ptr>(aligned_address) - sizeof(void*))));
}


/** allocate and deallocate using provided memory arena **/
#ifdef AAE_NEW
#undef AAE_NEW
#endif
#ifdef AAE_DELETE
#undef AAE_DELETE
#endif
#ifdef AAE_NEW_ARRAY
#undef AAE_NEW_ARRAY
#endif
#ifdef AAE_DELETE_ARRAY
#undef AAE_DELETE_ARRAY
#endif
#ifdef AAE_ALIGNED_NEW
#undef AAE_ALIGNED_NEW
#endif
#ifdef AAE_ALIGNED_DELETE
#undef AAE_ALIGNED_DELETE
#endif
#define AAE_NEW(type, arena) new(arena->Allocate(sizeof(type)))type
#define AAE_DELETE(object, arena) aae_delete(object, arena)
#define AAE_NEW_ARRAY(type, n, arena) aae_new_array<type>(arena, n)
#define AAE_DELETE_ARRAY(array, arena) aae_delete_array(array, arena)
#define AAE_ALIGNED_NEW(type, alignment, arena) new(aae_aligned_new(sizeof(type), alignment, arena))type
#define AAE_ALIGNED_DELETE(object, arena) aae_aligned_delete(object, arena)
#endif