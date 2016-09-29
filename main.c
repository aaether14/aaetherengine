#include "aae_utils/aae_types.h"
#include "aae_utils/aae_defines.h"
#include "aae_memory_allocator.h"
aae_allocator aae_mallocator;




#define AAE_LINKED_LIST_TYPE float
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"


/**
*Wrapper for linux's sys_write
*/
extern "C" void aae_fwrite(int32_t fd, const char * buffer, aae_size_t length);



extern "C" int aae_main()
{


	aae_fwrite(stderr, "Hello World!\n", 12);



	aae_LinkedList(float) * test = AAE_NEW(aae_LinkedList(float), aae_mallocator);
	test->push_back(2.2);
	AAE_DELETE(test, aae_mallocator);



	return 0;

}
