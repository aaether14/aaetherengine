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

	const char * str = "Hello World!\n";
	aae_write(stdout, str, aae_strlen(str));
	return 0;

}
