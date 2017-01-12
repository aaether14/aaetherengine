#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "assert/aae_assert.h"
#include "error/aae_error.h"
#include "os/linux/aae_os_functions.h" 
#include <pthread.h>


extern aae_allocator aae_mallocator;


#define AAE_LINKED_LIST_NAME test_list
#define AAE_LINKED_LIST_DATA_TYPE const_byte_ptr
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"



#define AAE_RB_TREE_NAME test_tree
#define AAE_RB_TREE_KEY_TYPE i32
#define AAE_RB_TREE_VALUE_TYPE const_byte_ptr
#define AAE_RB_TREE_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_rb_tree.def"



void* function(void* key)
{
	return AAE_NULL;
}



AAE_EXTERN_TOOL i32 aae_main()
{
	pthread_t thread_id;
	pthread_create(&thread_id, AAE_NULL, &function, NULL);
	char* str = (char*)aae_malloc(7);
	str[0] = 'h';
	str[1] = 'e';
	str[2] = 'l';
	str[3] = 'l';
	str[4] = 'o';
	str[5] = '\n';
	str[6] = '\0';
	aae_write(stderr, str, aae_strlen(str));
	aae_free(str);
	char* str2 = (char*)aae_malloc(7);
	str2[0] = 'h';
	str2[1] = 'e';
	str2[2] = 'l';
	str2[3] = 'l';
	str2[4] = '2';
	str2[5] = '\n';
	str2[6] = '\0';
	aae_write(stderr, str2, aae_strlen(str2));
	aae_free(str2);
	pthread_join(thread_id, NULL);
	return 0;
}
