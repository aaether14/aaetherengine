#include "aae_memory_allocator.h"
aae_allocator aae_mallocator;




#define AAE_LINKED_LIST_TYPE float
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"





extern "C" int aae_main()
{


	aae_LinkedList(float) * test = AAE_NEW(aae_LinkedList(float), aae_mallocator);
	test->push_back(2.2);
	AAE_DELETE(test, aae_mallocator);


	return 0;

}
