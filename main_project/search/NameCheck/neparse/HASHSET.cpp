/**************************************************************
*Filename:HASHSET.c
*Author: bfzhang
*Date:   2007-02-12
*Description: define the implementationn of simple hashmap
***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "HASHSET.h"


#ifndef FREE
#define FREE(p) if(p){ free(p); p = NULL;}
#endif

typedef unsigned int UINT32;
HashSetPtr createHashSet(const UINT32 dwBucketSize)
{
	HashSetPtr pHashSet = (HashSetPtr)calloc(1,sizeof(HashSet));
	if(pHashSet == NULL) return NULL;

	pHashSet->pBucketArray = (HashSetNodePtr)calloc(dwBucketSize,sizeof(HashSetNode));
	if(pHashSet->pBucketArray == NULL) return NULL;

	pHashSet->dwBucketSize = dwBucketSize;
	pHashSet->dwNumberHashSetNode = 0;
	return pHashSet;
}
int insertHashSetNode(HashSet * pHashSet,const char *key)
{
	UINT32 dwHashSign = getHashSetSign(key,strlen(key),pHashSet->dwBucketSize);
	HashSetNodePtr currHashSetNodePtr = NULL;
	HashSetNodePtr preHashSetNodePtr = NULL;
	UINT32 dwTmpLen = 0;
	if( ( pHashSet->pBucketArray + dwHashSign)->key == NULL)
	{

		currHashSetNodePtr = pHashSet->pBucketArray + dwHashSign;
		
		dwTmpLen = strlen(key);
		currHashSetNodePtr->key = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(currHashSetNodePtr->key == NULL) return -1;
		strncpy(currHashSetNodePtr->key,key,dwTmpLen);

		currHashSetNodePtr->next = NULL;

		pHashSet->dwNumberHashSetNode += 1;
		return 0;
	}
	else
	{
		currHashSetNodePtr = pHashSet->pBucketArray + dwHashSign;

		while(currHashSetNodePtr != NULL && currHashSetNodePtr->key != NULL)
		{
			//exist in hashmap
			if( strcmp(currHashSetNodePtr->key,key) == 0)
			{
				return -1;
			}
			preHashSetNodePtr = currHashSetNodePtr;
			currHashSetNodePtr = currHashSetNodePtr->next;
		}
		//insert at the last point
		if(currHashSetNodePtr != pHashSet->pBucketArray + dwHashSign)
		{
			currHashSetNodePtr = (HashSetNodePtr)calloc(1,sizeof(HashSetNode));
			if(currHashSetNodePtr == NULL) return -1;
		}
       
		dwTmpLen = strlen(key);
		currHashSetNodePtr->key = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(currHashSetNodePtr->key == NULL) return -1;
		strncpy(currHashSetNodePtr->key,key,dwTmpLen);
		
		currHashSetNodePtr->next = NULL;

		preHashSetNodePtr->next = currHashSetNodePtr;

		pHashSet->dwNumberHashSetNode += 1;
		
		return 0;
	}
}
int searchHashSetNode(const HashSet * pHashSet,const char *key)
{
	UINT32 dwHashSign = getHashSetSign(key,strlen(key),pHashSet->dwBucketSize);
	HashSetNodePtr currHashSetNodePtr = NULL;

   //assert(pHashSet != NULL && key != NULL);
    if(NULL == pHashSet || NULL == key)
        return -1;
	if( ( pHashSet->pBucketArray + dwHashSign)->key == NULL)
	{
		return -1;
	}
	else
	{
		currHashSetNodePtr = pHashSet->pBucketArray + dwHashSign;
		while(currHashSetNodePtr != NULL  && currHashSetNodePtr->key != NULL)
		{
			if(strcmp(currHashSetNodePtr->key,key) == 0 )
			{
				
				return 0;
			}
			currHashSetNodePtr = currHashSetNodePtr->next;
		}
		return -1;
	}
}
int removeHashSetNode(HashSet *pHashSet,const char *key)
{
	UINT32 dwHashSign = getHashSetSign(key,strlen(key),pHashSet->dwBucketSize);
	HashSetNodePtr currHashSetNodePtr = NULL;
	HashSetNodePtr preHashSetNodePtr = NULL;
	HashSetNodePtr nextHashSetNodePtr = NULL;
	UINT32 dwTmpLen = 0;

	currHashSetNodePtr = pHashSet->pBucketArray + dwHashSign;
	if(currHashSetNodePtr->key == NULL)
	{
		return -1;
	}
	else
	{
		while(currHashSetNodePtr != NULL && currHashSetNodePtr->key != NULL)
		{
			// <key,value> pair exist in hashmap
			if(strcmp(currHashSetNodePtr->key,key) == 0 )	
			{
				break;
			}
			preHashSetNodePtr = currHashSetNodePtr;
			currHashSetNodePtr = currHashSetNodePtr->next;
		}
		if(currHashSetNodePtr == NULL)
		{
			return -1;//<key,value> doesn't exist in hashmap
		}
		
		if(currHashSetNodePtr != pHashSet->pBucketArray + dwHashSign)
		{
			//break the linklist
			preHashSetNodePtr->next = currHashSetNodePtr->next;
			FREE(currHashSetNodePtr->key);
			FREE(currHashSetNodePtr);
			pHashSet->dwNumberHashSetNode -= 1;
			return 0;
		}
		else//currHashSetNodePtr == pBucketArray
		{

			if(currHashSetNodePtr->next == NULL)
			{
				FREE(currHashSetNodePtr->key);
				pHashSet->dwNumberHashSetNode -= 1;
				return 0;
			}

			nextHashSetNodePtr = currHashSetNodePtr ->next;

			//copy second 
			FREE(currHashSetNodePtr->key);
			
			dwTmpLen = strlen(nextHashSetNodePtr->key);
			currHashSetNodePtr->key = (char *)calloc(dwTmpLen + 1,sizeof(char));
			if(currHashSetNodePtr->key == NULL) return -1;
			strncpy(currHashSetNodePtr->key,nextHashSetNodePtr->key,dwTmpLen);
			
			currHashSetNodePtr->next = nextHashSetNodePtr ->next;	

			//remove second in linklist
			FREE(nextHashSetNodePtr->key);
			FREE(nextHashSetNodePtr);

			pHashSet->dwNumberHashSetNode -= 1;
			return 0;
		}
	}
}	
void printHashSet(const HashSet * pHashSet)
{
	HashSetNodePtr currHashSetNodePtr = NULL;
	UINT32 i = 0;
	printf("-----begin:%d <key,value> pair in hashset\n",pHashSet->dwNumberHashSetNode);
	for(; i < pHashSet->dwBucketSize;i++)
	{
		if( (pHashSet->pBucketArray + i)->key == NULL)
		{
			continue;
		}
		else
		{
			currHashSetNodePtr = pHashSet->pBucketArray + i;
			while(currHashSetNodePtr != NULL && currHashSetNodePtr->key != NULL)
			{
				printf("<%s>\n",currHashSetNodePtr->key);
				currHashSetNodePtr = currHashSetNodePtr ->next;
			}
		}
	}
	printf("---------the end----------\n");
}

void printHashSetToFile(const HashSet * pHashSet,const char *outFile,const char *openMode)
{
	FILE *fp = fopen(outFile,openMode);
	HashSetNodePtr currHashSetNodePtr = NULL;
	UINT32 i = 0;
	
	if(fp == NULL) return ;
	fprintf(fp,"-------begin----%d--keys\n",pHashSet->dwNumberHashSetNode);
	for(; i < pHashSet->dwBucketSize;i++)
	{
		if( (pHashSet->pBucketArray + i)->key == NULL)
		{
			continue;
		}
		else
		{
			currHashSetNodePtr = pHashSet->pBucketArray + i;
			while(currHashSetNodePtr)
			{
				fputs("<",fp);
				fputs(currHashSetNodePtr->key,fp);
				fputs(">\n",fp);
				fflush(fp);
				currHashSetNodePtr = currHashSetNodePtr ->next;
			}
		}
	}
	fprintf(fp,"--------end------\n");
	fclose(fp);

}
int destroyHashSet(HashSetPtr pHashSet)
{
	UINT32 i = 0;
	HashSetNodePtr currHashSetNodePtr = NULL;
	HashSetNodePtr nextHashSetNodePtr = NULL;

	//step 1;remove hashnode in linklist
	for( ; i < pHashSet->dwBucketSize ; i++)
	{
		currHashSetNodePtr = pHashSet->pBucketArray + i;
		//in pBucketArray
		FREE(currHashSetNodePtr->key);
		currHashSetNodePtr = currHashSetNodePtr->next;

		while(currHashSetNodePtr != NULL)
		{
			nextHashSetNodePtr = currHashSetNodePtr->next;
			FREE(currHashSetNodePtr->key);
			FREE(currHashSetNodePtr);
			pHashSet->dwNumberHashSetNode -= 1;
			currHashSetNodePtr = nextHashSetNodePtr;
		}
	}

	//step2: remove pBucketArray
	FREE(pHashSet->pBucketArray);

	//step3: remove hashmap
	FREE(pHashSet);

	return 0;
}

/**
*@name:
*@desc:simple hash function
*@return: hash sigh
*/
UINT32 getHashSetSign(const char *key,const UINT32 dwKeyLen,const UINT32 dwBucketSize)
{
	UINT32 dwSum =  0;
	UINT32 i = 0;

	for(; i < dwKeyLen ; i++)
	{
		dwSum += 13 * key[i];
	}
	return dwSum % dwBucketSize;
}
