#include <stdio.h>
#include "aae_utils/aae_linked_list_type_generation.h"



extern "C" {
void * aae_malloc(long numbytes);
void aae_free(void * firstbyte);
}



//extern variable from aae_memory_allocator.h
aae_allocator mallocator;



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{



 


	int * test = (int*)aae_malloc(sizeof(int));
	int * test2 = (int*)aae_malloc(sizeof(int));
	printf("%lu\n", (unsigned long int)test);
	printf("%lu\n", (unsigned long int)test2);
	aae_free(test);
	aae_free(test2);





	return 0;


}
