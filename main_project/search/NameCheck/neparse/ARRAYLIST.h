/****************************************************
*Filename:ARRAYLIST.h
*
*Date: 2007-03-08
*Description: 
* (1)The list is  constructed by array
* (2) just for char type
*(3) deep copy not just pointer
*ChangeLog:
*(1) create this file at 2007-03-08
*Time complexity:
*(1) get,set,add o(1)
*(2) remove,insert,search o(n)
*****************************************************/

#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H
#include <stdio.h>


typedef struct _ArrayListNode
{
	char *szValue;
}ArrayListNode;

typedef struct _ArrayList
{
	//public field
	ArrayListNode * pArrayListNodeArray;
	unsigned int dwLength;
	
	//private field
	unsigned int dwSizeofNodeArray;

}ArrayList;

ArrayList *ArrayList_create(unsigned int dwInitSize);
/**
*time complexity:o(1)
*/
int ArrayList_isEmpty(const ArrayList *pArrayList);

/**
*time complexity:o(1)
*/
unsigned int ArrayList_getLength(const ArrayList *pArrayList);

/**
*time complexity:o(1)
*note:calloc memory
*/
char *ArrayList_getNode(const ArrayList *pArrayList,unsigned int dwPosition);

/**
*time:complexity:o(1)
*/
int ArrayList_setNode(ArrayList *pArrayList,unsigned int dwPosition,char *szValue);

/**
*time complexity:o(n)
*/
int ArrayList_existNode(const ArrayList *pArrayList,char *szValue);

/**
*time complexity:o(n)
*/
int ArrayList_addNode(ArrayList *pArrayList,char *szValue);

/**
*time complexity:o(n)
*/
int ArrayList_insertNode(ArrayList *pArrayList,unsigned int dwPosition,char *szValue);

/**
*time comlexity:o(n)
*/
int ArrayList_removeNode(ArrayList *pArrayList,unsigned int dwPosition);


int ArrayList_destroy(ArrayList *pArrayList);

void ArrayList_print(ArrayList *pArrayList,FILE *fp);


/*
 * this interface is for quick_sort
 */ 
int arraylist_node_compare(const void *arg1, const void *arg2);


#endif
