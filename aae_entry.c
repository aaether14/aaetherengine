#define AAE_PROVIDE_MEMORY_ARENA_DEFINITION
#include "aae_allocators.h"
#ifdef AAE_LINUX_PLATFORM
extern "C++" inline void* operator new(aae_size_t, void* __p) { return __p; }
#endif
AAEAllocatorInterface* aae_mallocator = nullptr;


extern i32 aae_main(void);
extern "C" void exit(i32);
extern "C" void _start() 
{
        #if defined(AAE_LINUX_PLATFORM) && defined(AAE_64BIT_BUILD)
        __asm__ __volatile__("subq $8, %rsp");
        #endif
       DefaultMemoryArenaType* DefaultAllocator = new(aae_malloc(sizeof(DefaultMemoryArenaType)))DefaultMemoryArenaType;
       aae_mallocator = static_cast<AAEAllocatorInterface*>(DefaultAllocator);
       exit(aae_main());
}