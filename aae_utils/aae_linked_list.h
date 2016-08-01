/**
*aae_linked_list.h - Dinescu Stefan-Cristian aka Aaether - 31.07.2016
*/


#ifndef AAE_LINKED_LIST_H
#define AAE_LINKED_LIST_H



#include <malloc.h>
#include "aae_types.h"



/**
Code generation for linked list holding TYPE data 
-td stands for typedef
-don't ever use variables, functions etc. starting with "__". Only the framework has this right.
Features:
-push a new node to the front of the list
-push a new node to the back of the list
Safe types:
-aae_LinkedList(TYPE)
-aae_LinkedListElement(TYPE)
*/



/**
*Posible return values for the linked list
*/
typedef enum
	{	AAE_LINKED_LIST_ALLOCATION_ERROR = -2, //malloc error
		AAE_LINKED_LIST_INVALID_LIST = -1, //invalid list pointer 
		AAE_LINKED_LIST_FALSE = 0,
		AAE_LINKED_LIST_TRUE = 1,
		AAE_LINKED_LIST_SUCCESS = 2
	} AAE_LINKED_LIST_RETURN_VALUES;



#define AAE_DEFINE_LINKED_LIST(TYPE)														\
																		\
																		\
																		\
																		\
	typedef struct __linked_list_elem_##TYPE												\
	{																	\
		TYPE m_data;															\
		__linked_list_elem_##TYPE * m_next;												\
	} __td_linked_list_elem_##TYPE;														\
																		\
																		\
																		\
	typedef struct __linked_list_##TYPE													\
	{																	\
		aae_size_t m_size;														\
		__td_linked_list_elem_##TYPE * m_first;												\
		__td_linked_list_elem_##TYPE * m_last;												\
		__td_linked_list_elem_##TYPE * m_it;												\
	} __td_linked_list_##TYPE;														\
																		\
																		\
																		\
																		\
	inline static AAE_LINKED_LIST_RETURN_VALUES __linked_list_push_back_##TYPE(__td_linked_list_##TYPE * pList, TYPE tValue)		\
	{																	\
		if (!pList) return AAE_LINKED_LIST_INVALID_LIST;										\
		__td_linked_list_elem_##TYPE * pNewElem = (__td_linked_list_elem_##TYPE*)malloc(sizeof(__td_linked_list_elem_##TYPE));		\
		if (!pNewElem) return AAE_LINKED_LIST_SUCCESS;											\
		pNewElem->m_next = NULL; pNewElem->m_data = tValue;										\
		if (!pList->m_first) pList->m_first = pList->m_last = pNewElem;									\
		else { pList->m_last->m_next = pNewElem; pList->m_last = pNewElem; }								\
		pList->m_size++;														\
		return AAE_LINKED_LIST_SUCCESS;													\
	}																	\
																		\
																		\
																		\
	inline static AAE_LINKED_LIST_RETURN_VALUES __linked_list_push_front_##TYPE(__td_linked_list_##TYPE * pList, TYPE tValue)		\
	{																	\
		if (!pList) return AAE_LINKED_LIST_INVALID_LIST;										\
		__td_linked_list_elem_##TYPE * pNewElem = (__td_linked_list_elem_##TYPE*)malloc(sizeof(__td_linked_list_elem_##TYPE));		\
		if (!pNewElem) return AAE_LINKED_LIST_ALLOCATION_ERROR;										\
		pNewElem->m_next = pList->m_first; pNewElem->m_data = tValue;									\
		if (!pList->m_first) pList->m_first = pList->m_last = pNewElem;									\
		else { pList->m_first = pNewElem; }												\
		pList->m_size++;														\
		return AAE_LINKED_LIST_SUCCESS;													\
	}																	\
																		\
																		\
																		\
	inline static __td_linked_list_##TYPE * __linked_list_new_list_##TYPE()									\
	{																	\
		__td_linked_list_##TYPE * pNewList = (__td_linked_list_##TYPE*)malloc(sizeof(__td_linked_list_##TYPE));				\
		pNewList->m_first = pNewList->m_last = pNewList->m_it = NULL;									\
		pNewList->m_size = 0;														\
		return pNewList;														\
	}																	\
																		\
																		\
																		\
	inline static AAE_LINKED_LIST_RETURN_VALUES __linked_list_free_nodes_##TYPE(__td_linked_list_##TYPE * pList)				\
	{																	\
		if (!pList) return AAE_LINKED_LIST_INVALID_LIST;										\
		pList->m_it = pList->m_first;													\
		while (pList->m_it != NULL)													\
		{																\
			__td_linked_list_elem_##TYPE * pTempElem = pList->m_it;									\
			pList->m_it = pList->m_it->m_next;											\
			free(pTempElem);													\
		}																\
		pList->m_first = pList->m_last = pList->m_it = NULL;										\
		pList->m_size = 0;														\
		return AAE_LINKED_LIST_SUCCESS;													\
	}
		
		
		


/** 
*Types
*/
#define aae_LinkedList(TYPE)					__td_linked_list_##TYPE //Defines a linked list of a certain TYPE
#define aae_LinkedListElement(TYPE)				__td_linked_list_elem_##TYPE //Defines a node holding data of TYPE
/**
List members (better don't modify this using other than defined functions):
m_first - the first node in the list
m_last - the last node in the list
m_size - the size of the list
*/



/**
*Functions
*/
#define aae_NewLinkedList(TYPE)					__linked_list_new_list_##TYPE() //will return NULL if memory allocation failed
#define aae_LinkedListPushBack(TYPE, list, elem)		__linked_list_push_back_##TYPE(list, elem)
#define aae_LinkedListPushFront(TYPE, list, elem)		__linked_list_push_front_##TYPE(list, elem)
#define aae_LinkedListFreeNodes(TYPE, list)			__linked_list_free_nodes_##TYPE(list)	




#endif
