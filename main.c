#include <stdio.h>



#define AAE_LINKED_LIST_TYPE float
#include "aae_utils/aae_linked_list.h"



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{

	
	aae_LinkedList(float) * pList = aae_LinkedListNewList(float);
	aae_LinkedListPushBack(float, pList, 2.25);
	aae_LinkedListPushBack(float, pList, 3.14);
	aae_LinkedListPushBack(float, pList, 1241.32);


	for (pList->m_it = pList->m_first; pList->m_it != NULL; pList->m_it = pList->m_it->m_next)
	printf("%2.2f\n", pList->m_it->m_data);


	free(pList);
	pList = NULL;


 
	return 0;


}
