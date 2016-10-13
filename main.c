#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "os/linux/aae_os_functions.h"


extern aae_allocator aae_mallocator;



#define AAE_LINKED_LIST_TYPE r32
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"




AAE_EXTERN_TOOL i32 aae_main()
{


	aae_LinkedList(r32) * list = AAE_NEW(aae_LinkedList(r32), aae_mallocator);
	list->push_back(1.23);
	list->push_front(2.54);
	aae_write(stdout, "Hello World!\n", 13);
	aae_write(stdout, "Lol Dude\n", 9);
	AAE_DELETE(list, aae_mallocator);


	return 0;

}
