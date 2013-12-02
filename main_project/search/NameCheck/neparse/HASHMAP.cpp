/**************************************************************
*Filename:HASHMAP.c
*Author: bfzhang
*Date:   2007-02-12
*Description: define the implementationn of simple hashmap
***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "HASHMAP.h"
#ifndef FREE
#define FREE(p) if(p){ free(p); p = NULL;}
#endif
typedef unsigned int   UINT32;

HashMapPtr createHashMap(const UINT32 dwBucketSize)
{
	//assert(dwBucketSize > 0);
    if(dwBucketSize <= 0)
        return NULL;
	HashMapPtr pHashMap = (HashMapPtr)calloc(1,sizeof(HashMap));
	if(pHashMap == NULL) return NULL;

	pHashMap->pBucketArray = (HashMapNodePtr)calloc(dwBucketSize,sizeof(HashMapNode));
	if(pHashMap->pBucketArray == NULL)
	{
		 FREE(pHashMap);
		 return NULL;
	}

	pHashMap->dwBucketSize = dwBucketSize;
	pHashMap->dwNumberHashMapNode = 0;
	return pHashMap;
}
int insertHashMapNode(HashMap * pHashMap,const char *key,const char *value)
{
	//assert(pHashMap != NULL && key != NULL && value != NULL);
    if (NULL == pHashMap || NULL == key || NULL == value)
        return -1;

	UINT32 dwHashSign = getHashMapSign(key,strlen(key),pHashMap->dwBucketSize);
	HashMapNodePtr currHashMapNodePtr = NULL;
	HashMapNodePtr preHashMapNodePtr = NULL;
	UINT32 dwTmpLen = 0;
	if( ( pHashMap->pBucketArray + dwHashSign)->key == NULL)
	{

		currHashMapNodePtr = pHashMap->pBucketArray + dwHashSign;
		
		dwTmpLen = strlen(key);
		currHashMapNodePtr->key = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(currHashMapNodePtr->key == NULL) return -1;
		strncpy(currHashMapNodePtr->key,key,dwTmpLen);

		dwTmpLen = strlen(value);
		currHashMapNodePtr->value = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(currHashMapNodePtr->value == NULL) return -1;
		strncpy(currHashMapNodePtr->value,value,dwTmpLen);

		currHashMapNodePtr->next = NULL;

		pHashMap->dwNumberHashMapNode += 1;
		return 0;
	}
	else
	{
		currHashMapNodePtr = pHashMap->pBucketArray + dwHashSign;

		while(currHashMapNodePtr != NULL && currHashMapNodePtr->key != NULL)
		{
			//exist in hashmap
			if( strcmp(currHashMapNodePtr->key,key) == 0 &&
				strcmp(currHashMapNodePtr->value,value) == 0)
			{
				return -1;
			}
			preHashMapNodePtr = currHashMapNodePtr;
			currHashMapNodePtr = currHashMapNodePtr->next;
		}
		//insert at the last point
		if(currHashMapNodePtr != pHashMap->pBucketArray + dwHashSign)
		{
			currHashMapNodePtr = (HashMapNodePtr)calloc(1,sizeof(HashMapNode));
			if(currHashMapNodePtr == NULL) return -1;
		}
       
		dwTmpLen = strlen(key);
		currHashMapNodePtr->key = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(currHashMapNodePtr->key == NULL) return -1;
		strncpy(currHashMapNodePtr->key,key,dwTmpLen);
		
		dwTmpLen = strlen(value);
		currHashMapNodePtr->value = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(currHashMapNodePtr->value == NULL) return -1;
		strncpy(currHashMapNodePtr->value,value,dwTmpLen);

		currHashMapNodePtr->next = NULL;

		preHashMapNodePtr->next = currHashMapNodePtr;

		pHashMap->dwNumberHashMapNode += 1;
		
		return 0;
	}
}
int searchHashMapNode(const HashMap * pHashMap,const char *key,const char *value)
{
	//assert(pHashMap != NULL && key != NULL && value != NULL);
    if (NULL == pHashMap || NULL == key || NULL == value)
        return -1;

	UINT32 dwHashSign = getHashMapSign(key,strlen(key),pHashMap->dwBucketSize);
	HashMapNodePtr currHashMapNodePtr = NULL;

	if( ( pHashMap->pBucketArray + dwHashSign)->key == NULL)
	{
		return -1;
	}
	else
	{
		currHashMapNodePtr = pHashMap->pBucketArray + dwHashSign;
		while(currHashMapNodePtr != NULL  && currHashMapNodePtr->key != NULL)
		{
			if(strcmp(currHashMapNodePtr->key,key) == 0 &&
				strcmp(currHashMapNodePtr->value,value) == 0 )
			{
				return 0;
			}
			currHashMapNodePtr = currHashMapNodePtr->next;
		}
		return -1;
	}
}
int changeHashMapNode(HashMap *pHashMap,const char *szKey,const char *szNewValue)
{
	//assert(pHashMap != NULL && szKey != NULL && szNewValue != NULL);
    if(NULL == pHashMap || NULL == szKey || NULL == szNewValue)
        return -1;
	
	UINT32 dwHashSign = getHashMapSign(szKey,strlen(szKey),pHashMap->dwBucketSize);
	
	HashMapNodePtr currHashMapNodePtr = NULL;
	
	if( (pHashMap->pBucketArray + dwHashSign)->key == NULL)
	{
		return -1;
	}
	else
	{
		currHashMapNodePtr = pHashMap->pBucketArray + dwHashSign;
		while(currHashMapNodePtr != NULL && currHashMapNodePtr->key != NULL)
		{
			if(strcmp(currHashMapNodePtr->key,szKey) == 0)
			{
				FREE(currHashMapNodePtr->value);
				int newValueLen = strlen(szNewValue);
				currHashMapNodePtr->value = (char*)calloc(newValueLen + 1,sizeof(char));
				if(currHashMapNodePtr->value == NULL) return -1;
				strncpy(currHashMapNodePtr->value,szNewValue,newValueLen);			
			}
			currHashMapNodePtr = currHashMapNodePtr->next;
		}
		return -1;
	}
}
const char * getValueHashMap(const HashMap *pHashMap,const char *key)
{
	//assert(pHashMap != NULL && key != NULL);
    if(NULL == pHashMap || NULL == key)
        return NULL;


	UINT32 dwHashSign = getHashMapSign(key,strlen(key),pHashMap->dwBucketSize);
	HashMapNodePtr currHashMapNodePtr = NULL;

	if( ( pHashMap->pBucketArray + dwHashSign)->key == NULL)
	{
		return NULL;
	}
	else
	{
		currHashMapNodePtr = pHashMap->pBucketArray + dwHashSign;
		while(currHashMapNodePtr != NULL  && currHashMapNodePtr->key != NULL)
		{
			if(strcmp(currHashMapNodePtr->key,key) == 0 )
			{
				return currHashMapNodePtr->value;
			}
			currHashMapNodePtr = currHashMapNodePtr->next;
		}
		return NULL;
	}

}
int removeHashMapNode(HashMap *pHashMap,const char *key,const char*value)
{
	//assert(pHashMap != NULL && key != NULL && value != NULL);
    if(NULL == pHashMap || NULL == key || NULL == value)
        return -1;

	UINT32 dwHashSign = getHashMapSign(key,strlen(key),pHashMap->dwBucketSize);
	HashMapNodePtr currHashMapNodePtr = NULL;
	HashMapNodePtr preHashMapNodePtr = NULL;
	HashMapNodePtr nextHashMapNodePtr = NULL;
	UINT32 dwTmpLen = 0;

	currHashMapNodePtr = pHashMap->pBucketArray + dwHashSign;
	if(currHashMapNodePtr->key == NULL)
	{
		return -1;
	}
	else
	{
		while(currHashMapNodePtr != NULL && currHashMapNodePtr->key != NULL)
		{
			// <key,value> pair exist in hashmap
			if(strcmp(currHashMapNodePtr->key,key) == 0 &&
				strcmp(currHashMapNodePtr->value,value) == 0 )	
			{
				break;
			}
			preHashMapNodePtr = currHashMapNodePtr;
			currHashMapNodePtr = currHashMapNodePtr->next;
		}
		if(currHashMapNodePtr == NULL)
		{
			return -1;//<key,value> doesn't exist in hashmap
		}
		
		if(currHashMapNodePtr != pHashMap->pBucketArray + dwHashSign)
		{
			//break the linklist
			preHashMapNodePtr->next = currHashMapNodePtr->next;
			FREE(currHashMapNodePtr->key);
			FREE(currHashMapNodePtr->value);
			FREE(currHashMapNodePtr);
			pHashMap->dwNumberHashMapNode -= 1;
			return 0;
		}
		else//currHashMapNodePtr == pBucketArray
		{

			if(currHashMapNodePtr->next == NULL)
			{
				FREE(currHashMapNodePtr->key);
				FREE(currHashMapNodePtr->value);
				pHashMap->dwNumberHashMapNode -= 1;
				return 0;
			}

			nextHashMapNodePtr = currHashMapNodePtr ->next;

			//copy second 
			FREE(currHashMapNodePtr->key);
			FREE(currHashMapNodePtr->value);
			
			dwTmpLen = strlen(nextHashMapNodePtr->key);
			currHashMapNodePtr->key = (char *)calloc(dwTmpLen + 1,sizeof(char));
			if(currHashMapNodePtr->key == NULL) return -1;
			strncpy(currHashMapNodePtr->key,nextHashMapNodePtr->key,dwTmpLen);

			dwTmpLen = strlen(nextHashMapNodePtr->value);
			currHashMapNodePtr->value = (char *)calloc(dwTmpLen + 1,sizeof(char));
			if(currHashMapNodePtr->value == NULL) return -1;
			strncpy(currHashMapNodePtr->value,nextHashMapNodePtr->value,dwTmpLen);
			
			currHashMapNodePtr->next = nextHashMapNodePtr ->next;	

			//remove second in linklist
			FREE(nextHashMapNodePtr->key);
			FREE(nextHashMapNodePtr->value);
			FREE(nextHashMapNodePtr);

			pHashMap->dwNumberHashMapNode -= 1;
			return 0;
		}
	}
}	
void printHashMap(const HashMap * pHashMap)
{
	//assert(pHashMap != NULL);	
    if(NULL == pHashMap)
        return;

	HashMapNodePtr currHashMapNodePtr = NULL;
	UINT32 i = 0;
	printf("-----begin:%d <key,value> pair in hashmap\n",pHashMap->dwNumberHashMapNode);
	for(; i < pHashMap->dwBucketSize;i++)
	{
		if( (pHashMap->pBucketArray + i)->key == NULL)
		{
			continue;
		}
		else
		{
			currHashMapNodePtr = pHashMap->pBucketArray + i;
			while(currHashMapNodePtr != NULL && currHashMapNodePtr->key != NULL)
			{
				printf("<%s,%s>\n",currHashMapNodePtr->key,currHashMapNodePtr->value);
				currHashMapNodePtr = currHashMapNodePtr ->next;
			}
		}
	}
	printf("---------the end----------\n");
}

void printHashMapToFile(const HashMap * pHashMap,const char *outFile,const char *openMode)
{
	//assert(pHashMap != NULL && outFile != NULL && openMode != NULL);	
    if(NULL == pHashMap || NULL == outFile || NULL == openMode)
        return;

	FILE *fp = fopen(outFile,openMode);
	HashMapNodePtr currHashMapNodePtr = NULL;
	UINT32 i = 0;
	
	if(fp == NULL) return ;
	fprintf(fp,"-------begin---%d <key,value> pairs\n",pHashMap->dwNumberHashMapNode);
	
	for(; i < pHashMap->dwBucketSize;i++)
	{
		if( (pHashMap->pBucketArray + i)->key == NULL)
		{
			continue;
		}
		else
		{
			currHashMapNodePtr = pHashMap->pBucketArray + i;
			while(currHashMapNodePtr)
			{
				fputs("<",fp);
				fputs(currHashMapNodePtr->key,fp);
				fputs(",",fp);
				fputs(currHashMapNodePtr->value,fp);
				fputs(">",fp);
				fputs("\n",fp);
				fflush(fp);
				currHashMapNodePtr = currHashMapNodePtr ->next;
			}
		}
	}
	fprintf(fp,"-------end---------\n");
	fclose(fp);

}
int destroyHashMap(HashMapPtr pHashMap)
{
	//assert(pHashMap != NULL);	
    if(NULL == pHashMap)
        return -1;


	UINT32 i = 0;
	HashMapNodePtr currHashMapNodePtr = NULL;
	HashMapNodePtr nextHashMapNodePtr = NULL;

	//step 1;remove hashnode in linklist
	for( ; i < pHashMap->dwBucketSize ; i++)
	{
		currHashMapNodePtr = pHashMap->pBucketArray + i;
		//in pBucketArray
		FREE(currHashMapNodePtr->key);
		FREE(currHashMapNodePtr->value);
		currHashMapNodePtr = currHashMapNodePtr->next;

		while(currHashMapNodePtr != NULL)
		{
			nextHashMapNodePtr = currHashMapNodePtr->next;
			FREE(currHashMapNodePtr->key);
			FREE(currHashMapNodePtr->value);
			FREE(currHashMapNodePtr);
			pHashMap->dwNumberHashMapNode -= 1;
			currHashMapNodePtr = nextHashMapNodePtr;
		}
	}

	//step2: remove pBucketArray
	FREE(pHashMap->pBucketArray);

	//step3: remove hashmap
	FREE(pHashMap);

	return 0;
}

/**
*@name:
*@desc:simple hash function
*@return: hash sigh
*/
UINT32 getHashMapSign(const char *key,const UINT32 dwKeyLen,const UINT32 dwBucketSize)
{
	UINT32 dwSum =  0;
	UINT32 i = 0;

	for(; i < dwKeyLen ; i++)
	{
		dwSum += 13 * key[i];
	}
	return dwSum % dwBucketSize;
}
