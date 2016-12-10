#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "assert/aae_assert.h"
#include "os/linux/aae_os_functions.h" 



extern aae_allocator aae_mallocator;



#define AAE_LINKED_LIST_TYPE r32
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"



#define AAE_RB_TREE_KEY_TYPE r32
#define AAE_RB_TREE_VALUE_TYPE const_byte_ptr
#define AAE_RB_TREE_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_rb_tree.def"




AAE_EXTERN_TOOL i32 aae_main()
{


	aae_RBTree(r32, const_byte_ptr) *test = AAE_NEW(aae_RBTree(r32, const_byte_ptr), aae_mallocator);
	test->insert(5, "This is a message!");
	test->insert(7.45, "This is the best message!");
	test->insert(9.13, "lol9");
	test->insert(3.14, "Fucking panzers");
	test->insert(2.113, "Lolipop!");
	test->insert(1.71, "Hello World!");


	const_byte_ptr t2 = test->get(5.0);
	AAE_ASSERT(t2 != AAE_NULL);
	aae_write(stdout, t2, aae_strlen(t2));
	AAE_DELETE(test, aae_mallocator);


	return 0;

}
