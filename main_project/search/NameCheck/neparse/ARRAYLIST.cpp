/****************************************************
*Filename:ARRAYLIST.c
*Author: bfzhang
*Date: 2007-03-08
*Description: The list is  constructed by array
*ChangeLog:
*(1) create this file at 2007-03-08
*Time complexity:
*(1) get,set,add o(1)
*(2) remove,insert o(n)
*****************************************************/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ARRAYLIST.h"

typedef unsigned int UINT32;

#ifndef FREE
#define FREE(p) if(p){free(p);p = NULL;}
#endif

ArrayList *ArrayList_create(unsigned int dwInitSize)
{
	ArrayList *pCurrArrayList = NULL;
	
	if(dwInitSize == 0) return NULL;

	pCurrArrayList = (ArrayList *)calloc(1,sizeof(ArrayList));
	if(pCurrArrayList == NULL) return NULL;

	pCurrArrayList->pArrayListNodeArray = (ArrayListNode *)calloc(dwInitSize,sizeof(ArrayListNode));
	if(pCurrArrayList->pArrayListNodeArray == NULL) return NULL;

	pCurrArrayList->dwSizeofNodeArray = dwInitSize;

	return pCurrArrayList;
}

/**
*time complexity:o(1)
*/
int ArrayList_isEmpty(const ArrayList *pArrayList)
{
	//assert(pArrayList != NULL);
    if(NULL == pArrayList)
        return -1;
	return (pArrayList->dwLength == 0)?0:-1;
}


/**
*time complexity:o(1)
*/
UINT32 ArrayList_getLength(const ArrayList *pArrayList)
{
	//assert(pArrayList != NULL);
    if(NULL == pArrayList)
        return 0;
	return pArrayList->dwLength;
}

/**
 * calloc memory
 * */
char *ArrayList_getNode(const ArrayList *pArrayList,unsigned int dwPosition)
{
    //assert(pArrayList != NULL);
    if(NULL == pArrayList)
        return NULL;
    
    if(dwPosition >= pArrayList->dwLength) return NULL;
    
    UINT32 dwTmpLen = strlen( (pArrayList->pArrayListNodeArray + dwPosition)->szValue);
    char *szBuff = (char *)calloc(1,dwTmpLen + 1);
    strncpy(szBuff,(pArrayList->pArrayListNodeArray + dwPosition)->szValue,dwTmpLen);

    return szBuff;    
}

/**
*time:complexity:o(1)
*Note:dwPosition base 0
*may be strlen(szValue) == 0
*/
int ArrayList_setNode(ArrayList *pArrayList,unsigned int dwPosition,char *szValue)
{
	//assert(pArrayList != NULL && szValue != NULL);
    if(NULL == pArrayList||NULL == szValue)
        return -1;
	ArrayListNode *pTmpListNode = NULL;
	UINT32 dwTmpLen = 0;
	if(dwPosition >= pArrayList->dwLength) return -1;

	pTmpListNode = pArrayList->pArrayListNodeArray + dwPosition;
	FREE(pTmpListNode->szValue);

	dwTmpLen = strlen(szValue);
	pTmpListNode->szValue = (char *)calloc(dwTmpLen + 1,sizeof(char));
	if(pTmpListNode->szValue == NULL) return -1;

	strncpy(pTmpListNode->szValue,szValue,dwTmpLen);

	return 0;
}

/**
*time complexity:o(n)
*/
int ArrayList_existNode(const ArrayList *pArrayList,char *szValue)
{
	//assert(pArrayList != NULL && szValue != NULL);
    if(NULL == pArrayList ||NULL == szValue)
        return -1;

	UINT32 i = 0;
	ArrayListNode *pCurrNode = NULL;
	for( i = 0; i < pArrayList->dwLength;i++)
	{
		pCurrNode = pArrayList->pArrayListNodeArray + i;
		if(strcmp(pCurrNode->szValue,szValue) == 0) return 0;
	}
	return -1;
}

/**
*time complexity:o(n)
* may strlen(szValue) == 0
*/
int ArrayList_addNode(ArrayList *pArrayList,char *szValue)
{
	//assert(pArrayList != NULL && szValue != NULL);
    if(NULL == pArrayList ||NULL == szValue)
        return -1;

	ArrayListNode *pCurrListNode = NULL;
	UINT32 dwTmpLen = 0;

	if( (pArrayList->dwLength + 1) > pArrayList->dwSizeofNodeArray)
	{
		//realloc memory
		pArrayList->pArrayListNodeArray = (ArrayListNode *)realloc(pArrayList->pArrayListNodeArray,
			2 * pArrayList->dwSizeofNodeArray * sizeof(ArrayListNode));
		
		if(pArrayList->pArrayListNodeArray == NULL) return -1;

		pArrayList->dwSizeofNodeArray = 2 * pArrayList->dwSizeofNodeArray;

	}

	pCurrListNode = pArrayList->pArrayListNodeArray + pArrayList->dwLength;

	dwTmpLen = strlen(szValue);

	/*step1:add szValue*/
	pCurrListNode->szValue = (char *)calloc(dwTmpLen + 1,sizeof(char));
	if(pCurrListNode->szValue == NULL) return -1;
	strncpy(pCurrListNode->szValue,szValue,dwTmpLen);

	/*pArrayList->dwLength++*/
	pArrayList->dwLength += 1;


	return 0;
}

/**
*time complexity:o(n)
*dwPosition:base 0
* @param: maybe strlen(szValue) == 0 
*/
int ArrayList_insertNode(ArrayList *pArrayList,unsigned int dwPosition,char *szValue)
{
	//assert(pArrayList != NULL && szValue  != NULL);
    if(NULL == pArrayList ||NULL == szValue)
        return -1;
	
	UINT32 dwTmpLen = 0;
	UINT32 i = 0;
	ArrayListNode * pCurrNode = NULL;
	ArrayListNode * pLeftNode = NULL;
	ArrayListNode * pRightNode = NULL;
	if(dwPosition >= pArrayList->dwLength) return -1;
	if( (pArrayList->dwLength + 1) > (pArrayList->dwSizeofNodeArray) )
	{
		//realloc memory
		pArrayList->pArrayListNodeArray = (ArrayListNode *)realloc(pArrayList->pArrayListNodeArray,
			2 * pArrayList->dwSizeofNodeArray * sizeof(ArrayListNode) );

		if(pArrayList->pArrayListNodeArray == NULL) return -1;
		 
		pArrayList->dwSizeofNodeArray = 2 * pArrayList->dwSizeofNodeArray;
	}
	
	dwTmpLen = strlen(szValue);

	//step1:move node
	for(i = pArrayList->dwLength - 1; i != (dwPosition - 1); i--)
	{
		pLeftNode = pArrayList->pArrayListNodeArray + i;
		pRightNode = pArrayList->pArrayListNodeArray + i + 1;
		
		
		dwTmpLen = strlen(pLeftNode->szValue);
		FREE(pRightNode->szValue);
		pRightNode->szValue = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(pRightNode->szValue == NULL) return -1;
		strncpy(pRightNode->szValue,pLeftNode->szValue,dwTmpLen);
	}
	
	//step2:set node
	pCurrNode = pArrayList->pArrayListNodeArray + dwPosition;
	FREE(pCurrNode->szValue);
	dwTmpLen = strlen(szValue);
	pCurrNode->szValue = (char *)calloc(dwTmpLen + 1,sizeof(char));
	if(pCurrNode->szValue == NULL) return -1;
	strncpy(pCurrNode->szValue,szValue,dwTmpLen);

	//step3:set pArrayList->dwLength;

	pArrayList->dwLength += 1;

	return 0;
}

int ArrayList_destroy(ArrayList *pArrayList)
{
	//assert(pArrayList != NULL);
    if(NULL == pArrayList)
        return -1;
	
	UINT32 i = 0;
	ArrayListNode * pTmpListNode = NULL;
	
	/*step1:free every ArrayListNode->szValue*/
	for( i = 0; i < pArrayList->dwLength ; i++)
	{
		pTmpListNode = pArrayList->pArrayListNodeArray + i;
		FREE( pTmpListNode->szValue);
	}

	/**step2:free pArrayList->pArrayListNodeAray*/
	FREE(pArrayList->pArrayListNodeArray);

	/**step3:free pArrayList**/
	FREE(pArrayList);

	return 0;
}

void ArrayList_print(ArrayList *pArrayList,FILE *fp)
{
	//assert(pArrayList != NULL);
    if(NULL == pArrayList)
        return;
	UINT32 i = 0;
	ArrayListNode *pCurrListNode = NULL;
	printf("[number of nodes:%d]---[sizeof nodearray:%d]\n",pArrayList->dwLength,pArrayList->dwSizeofNodeArray);
	for( i = 0; i < pArrayList->dwLength; i++)
	{
		pCurrListNode = pArrayList->pArrayListNodeArray + i;
		printf("%s\n",pCurrListNode->szValue);
	}
}

int arraylist_node_compare(const void *arg1,const void *arg2)
{
	//assert(arg1 != NULL && arg2 != NULL);
    if(NULL == arg1 || NULL == arg2)
        return -1;
	ArrayListNode *stpNodeLeft = (ArrayListNode *)arg1;
	ArrayListNode *stpNodeRight = (ArrayListNode *)arg2;
	
	return strcmp(stpNodeLeft->szValue,stpNodeRight->szValue);
}
