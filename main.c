#include <stdio.h>



#define AAE_LINKED_LIST_TYPE float
#include "aae_utils/aae_linked_list.def"



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


	aae_LinkedListForEach(pList)
	{
		printf("%2.2f\n", pList->m_it->m_data);
	}


	free(pList);
	pList = NULL;


 
	return 0;


}
