#ifndef AAE_MEMORY_H
#define AAE_MEMORY_H
#include "aae_new.h"
namespace aae
{
	class AAEAllocatorInterface
	{
		public:
		virtual ~AAEAllocatorInterface() {}
		virtual void* Allocate(aae_size_t size) noexcept = 0; /** should it fail, this MUST return nullptr, should size be 0, this MUST return nullptr **/
		virtual void Deallocate(void* start) noexcept = 0; /** should start be nullptr this MUST do nothing **/
	};
	#ifndef AAE_PROVIDE_ALLOCATOR_DEFINITIONS
	extern AAEAllocatorInterface& mallocator;
	extern AAEAllocatorInterface& mallocator_release;
	#else 
	#undef AAE_PROVIDE_ALLOCATOR_DEFINITIONS
	#endif
}
#endif