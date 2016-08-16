#include <stdio.h>
#include "aae_utils/aae_linked_list_type_generation.h"



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{

	
	aae_LinkedList(float) * pList = aae_LinkedListNewList(float);
	for (int i = 0; i < 1000000; i++)
	aae_LinkedListPushFront(float, pList, float(i));



	aae_LinkedListFreeNodes(float, pList);	
	free(pList);
	pList = NULL;


 
	return 0;


}
