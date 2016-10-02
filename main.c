#include "aae_memory_allocator.h"
#include "aae_allocators.h"



extern aae_allocator aae_mallocator;



#define AAE_LINKED_LIST_TYPE float
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"




AAE_EXTERN_TOOL void aae_write(i32 fd, const char * buffer, aae_size_t length);
/**
*Wrapper for linux's sys_write
*/




AAE_EXTERN_TOOL int32_t aae_main()
{
	
	aae_LinkedList(float) * list = AAE_NEW(aae_LinkedList(float), aae_mallocator);
	list->push_back(1.23);
	list->push_front(2.54);
	aae_write(stdout, "Hello World!\n", 13);
	AAE_DELETE(list, aae_mallocator);
	return 0;

}
