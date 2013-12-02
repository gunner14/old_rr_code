/*****************************************************
*Filename:AB_UTILITY_WORDS_DICT.h
*Author:bfzhang
*Date: 2007-01-20
*Version:0.1
*Discription: define the public functions of how to build the dictionary for segmentation
********************************************************/
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_UTILITY.h"
#include "STRING_UTILITY.h"
#include <stdio.h>
#include <string.h>
/*#include <malloc.h>*/
#include <assert.h>
#include <stdlib.h>

/**private desclaretion********************************/

/********************************************************
*private interface declaration*************************
**********************************************************/

int ABU_WORDS_DICT_addWord(ABU_WORDS_DICT_S *pDict,const char *szWord,const char *szWordPosTag);

int ABU_WORDS_DICT_hasSingleTrieNode( TrieNodePtr pTrieNodePtr);

int ABU_WORDS_DICT_binarySearchWordInTrieNodeArray(const TrieNodePtr pTrieNodePtrArray,
												   const UINT32 dwNumberChild,const char * szWord,UINT32 dwHasCovered);
const char *ABU_WORDS_DICT_getPosTagBinarySearchInTrieNodeArray(const TrieNodePtr pTrieNodePtrArray,
													const UINT32 dwNumberChild,const char *szWord,UINT32 dwHasCovered);
int ABU_WORDS_DICT_copySingleTrieNode(TrieNodePtr pDestTrieNodePtr,const TrieNodePtr pSrcTrieNodePtr);

int ABU_WORDS_DICT_copyArrayTrieNode(TrieNodePtr pTrieNodeDestArray,const TrieNodePtr pTrieNodeSrcArray,
									 const UINT32 dwTrieNodeNumber);

void ABU_WORDS_DICT_destroyArrayTrieNodeContent(TrieNodePtr pTrieNodePtrArray,const UINT32 dwNumberTrieNode);

void ABU_WORDS_DICT_destroySingleTrieNodeContent(TrieNodePtr pTrieNodePtr);

void ABU_WORDS_DICT_destroySingleHashNodeContent(HashNodePtr pHashNodePtr);

HashNodePtr ABU_WORDS_DICT_addHashNode(ABU_WORDS_DICT_S *pDict,const char *szWord,const 
									   char *szWordPosTag, UINT32 dwHasCovered);

TrieNodePtr ABU_WORDS_DICT_addTrieNodeIntoHashNode(HashNodePtr pCurrHashNodePtr,const char *szWord,
												   const char *szWordPosTag,UINT32 dwHasCovered);

TrieNodePtr	ABU_WORDS_DICT_addTrieNodeIntoTrieNode( TrieNodePtr pPreTrieNodePtr,TrieNodePtr pCurrTrieNodePtr,
												   const char * szWord,const char *szWordPosTag,UINT32 dwHasCovered);
int ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(TrieNodePtr pCurrTrieNodePtr,const char *szWord,
												 const char *szWordPosTag,UINT32 dwHasCovered);
HashNodePtr ABU_WORDS_DICT_getHashNodePtr(const ABU_WORDS_DICT_S * pDict,const char *szWord,const UINT32 dwHasCovered);



void ABU_WORDSS_DICT_outputSingleTrieNode(const TrieNodePtr pTrieNodePtr,char *szBuff,FILE *fp);

void getWordAndPosTag(const char *szBuff,char *szWord,char *szWordPosTag);

int ABU_WORDS_DICT_outputSingleTrieNode(const TrieNodePtr pTrieNodePtr,char *szBuff,FILE *fp);


int quickSortWordItem(char **szWordItemArray,const int lowPos,const int highPos);
int setWordItem(char *szLeftWordItem,const char *szRightWordItem);
int sortStrcmp(const void *arg1,const void *arg2);
/**********************************************************************
*public interface implementation**************************************
***********************************************************************/

ABU_WORDS_DICT_S* ABU_WORDS_DICT_loadDictFromFile(const char **szDictFile,const UINT32 dwDictNumber)
{
	ABU_ASSERT(szDictFile != NULL);
	
	char szBuff[MAX_WORD_LENGTH + 1];
	char szWord[MAX_WORD_LENGTH + 1];
	char szWordPosTag[MAX_WORD_LENGTH + 1];

	UINT32 i = 0;
	UINT32 dwTmpLen = 0;
	
	/*step1:read word item into memory***/
	UINT32 dwWordItemsNumber = 0;
	UINT32 dwWordItemsSize = 100000;
	char **szWordItemArray= (char **)calloc(dwWordItemsSize,sizeof(char *));
	if(szWordItemArray == NULL) return NULL;


	for(i = 0; i < dwDictNumber;i++)
	{
		FILE *fp = fopen(szDictFile[i],"r");
		if(fp == NULL) 
		{
			fprintf(stderr,"can't open %s\n",szDictFile[i]);
            for(i = 0 ; i < dwWordItemsNumber;i++)
            {
                ABU_FREE(szWordItemArray[i]);
            }
            ABU_FREE(szWordItemArray);
            return NULL;
		}
		while( !feof(fp))
		{
			memset(szBuff,0,MAX_WORD_LENGTH + 1);

			fgets(szBuff,MAX_WORD_LENGTH + 1,fp);

			dwTmpLen = strlen(szBuff);
			
			if(dwTmpLen == 0) continue;			

			if(dwTmpLen >= 1 && szBuff[dwTmpLen - 1] == STRING_NEW_LINE)  szBuff[dwTmpLen - 1] = STRING_END;
	
			ABU_WORDS_PREPROCESS_all(szBuff);

			dwTmpLen = strlen(szBuff);
			if(dwTmpLen == 0) continue;

			if( (dwWordItemsNumber + 1) > dwWordItemsSize)
			{
				szWordItemArray = (char **)realloc(szWordItemArray,2 * dwWordItemsSize * sizeof(char *));
				if(szWordItemArray == NULL)
                {
                    for(i = 0 ; i < dwWordItemsNumber;i++)
                    {
                        ABU_FREE(szWordItemArray[i]);
                    }
                    ABU_FREE(szWordItemArray);
                    return NULL;
                } 
				dwWordItemsSize *= 2;
			}

			szWordItemArray[dwWordItemsNumber] = (char *)calloc(dwTmpLen + 1,sizeof(char));
			if(szWordItemArray[dwWordItemsNumber] == NULL)
            {
                for(i = 0 ; i < dwWordItemsNumber;i++)
                {
                    ABU_FREE(szWordItemArray[i]);
                }
                ABU_FREE(szWordItemArray);
                return NULL;
            }
			strncpy(szWordItemArray[dwWordItemsNumber],szBuff,dwTmpLen);

			dwWordItemsNumber += 1;
		}

		if(fclose(fp) != 0) fprintf(stderr,"fail to close %s\n",szDictFile[i]);
	}

	/*step2: sort szWordItems by QuickSort_Algorithm && unique***/

	qsort(szWordItemArray,dwWordItemsNumber,sizeof(char *),sortStrcmp);	
	
	/*step3: insert word ******************************/

	ABU_WORDS_DICT_S *pDict = (ABU_WORDS_DICT_S *)calloc(1,sizeof(ABU_WORDS_DICT_S));
	if(pDict == NULL) return NULL;

	for( i = 0; i < dwWordItemsNumber;i++)
	{
		memset(szWord,0,MAX_WORD_LENGTH + 1);
		memset(szWordPosTag,0,MAX_WORD_LENGTH + 1);
		getWordAndPosTag(szWordItemArray[i],szWord,szWordPosTag);
		if( ABU_WORDS_DICT_addWord(pDict,szWord,szWordPosTag) == -1)
        {
            for(i = 0 ; i < dwWordItemsNumber;i++)
            {
                ABU_FREE(szWordItemArray[i]);
            }
            ABU_FREE(szWordItemArray);
            ABU_WORDS_DICT_destroyDict(pDict);
            return NULL;
        } 

		#ifdef AB_DEBUG
			if(i % 10000 == 0 && i != 0) fprintf(stderr,".");
		#endif

	}
	#ifdef AB_DEBUG
		fprintf(stderr,"\n load  %d  dict_items\n",dwWordItemsNumber);
	#endif
	/*step4:free szWordItemArray***/
	for(i = 0 ; i < dwWordItemsNumber;i++)
	{
		ABU_FREE(szWordItemArray[i]);
	}
	ABU_FREE(szWordItemArray);

	return pDict;
}
ABU_WORDS_DICT_S *ABU_WORDS_DICT_loadDictFromArray(const char **szWordItemArray,const UINT32 dwWordItemNumber)
{
	ABU_ASSERT(szWordItemArray != 0 && dwWordItemNumber != 0);
	
	char szWord[MAX_WORD_LENGTH + 1];
	char szWordPosTag[MAX_WORD_LENGTH + 1];
		
	//step1: sort szWordItemArray by quick_sort
	qsort(szWordItemArray,dwWordItemNumber,sizeof(szWordItemArray[0]),sortStrcmp);
	
	//step2: create dict
	ABU_WORDS_DICT_S *pDict = (ABU_WORDS_DICT_S *)calloc(1,sizeof(ABU_WORDS_DICT_S));
	if(pDict == NULL) return NULL;
	
	int i = 0;
	for( i = 0; i < dwWordItemNumber;i++)
	{
		memset(szWord,0,MAX_WORD_LENGTH + 1);	
		memset(szWordPosTag,0,MAX_WORD_LENGTH + 1);	
		getWordAndPosTag(szWordItemArray[i],szWord,szWordPosTag);
		if(ABU_WORDS_DICT_addWord(pDict,szWord,szWordPosTag) == -1)
		{
            ABU_WORDS_DICT_destroyDict(pDict);
            return NULL;
		}
		#ifdef AB_DEBUG
			if(i % 10000 == 0 && i != 0) fprintf(stderr,".");
		#endif
	}
	#ifdef AB_DEBUG
		fprintf(stderr,"\n total load %u dict_items\n",dwWordItemNumber);
	#endif
	
	return pDict;
}
int ABU_WORDS_DICT_addWord(ABU_WORDS_DICT_S *pDict,const char *szWord,const char *szWordPosTag)
{
	 ABU_ASSERT( pDict != NULL && szWord != NULL);

	UINT32 dwSzWordlen = strlen(szWord);
	UINT32 dwHasCovered = 0;
	HashNodePtr currHashNodePtr;
	TrieNodePtr currTrieNodePtr;
  
	//get mutex_loc
	//pthread_mutex_lock(&pDict->dictMutexLock);
	
	/*alter index when Layer == 1 */
	if( (currHashNodePtr = ABU_WORDS_DICT_addHashNode(pDict,szWord,szWordPosTag,dwHasCovered)) == NULL)
				return -1;

	dwHasCovered += GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);

	/*alter index when Layer >= 2*/
	if( dwHasCovered < dwSzWordlen )
		currTrieNodePtr = ABU_WORDS_DICT_addTrieNodeIntoHashNode(currHashNodePtr,szWord,szWordPosTag,dwHasCovered);
		
	//pthread_mutex_unlock(&pDict->dictMutexLock);
	return 0;
}

int ABU_WORDS_DICT_searchWord(const ABU_WORDS_DICT_S *pDict,const char *szWord)
{
	ABU_ASSERT ( pDict != NULL && szWord != NULL);
	
	UINT32 dwHasCovered = 0;
	HashNodePtr currHashNodePtr = NULL;
	const UINT32 dwLenChineseCharacter = 2;
	UINT32 dwCharacterLength = 0;
	UINT32 dwHashPos = 0;
	UINT32 dwLenSzWord = strlen(szWord);

	/*find index where Layer==1*/
	dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	if(dwCharacterLength == dwLenChineseCharacter)
	{
		dwHashPos = getDoubleByteHashValue( (BYTE)szWord[dwHasCovered],(BYTE)szWord[dwHasCovered + 1],BUCKET_SIZE);
	}
	else
	{
		dwHashPos = getSingleByteHashValue( (BYTE)szWord[dwHasCovered],BUCKET_SIZE);
	}
	currHashNodePtr = pDict->pGeneralHashTable[dwHashPos];

	/*doesn't exist index where Layer==1*/
	if(currHashNodePtr == NULL)
		return -1;

	while(currHashNodePtr != NULL)
	{
		if(strncmp(currHashNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength) == 0)
		{
			dwHasCovered += dwCharacterLength;
			break;
		}
		currHashNodePtr = currHashNodePtr->pNext;
	}
	if(currHashNodePtr == NULL)
		return -1;

	/*find index where Layer==2*/
	if(dwHasCovered == dwLenSzWord)
	{
		if(currHashNodePtr->byIsWord == 1)
			return 0;
		else
			return -1;
	}
	/*find index where Layer >=3 */
	return ABU_WORDS_DICT_binarySearchWordInTrieNodeArray(currHashNodePtr->pFirstTrieNodeArray,
										currHashNodePtr->dwNumberEntry,szWord,dwHasCovered);
}
const char * ABU_WORDS_DICT_getPosTag(const ABU_WORDS_DICT_S *pDict,const char *szWord)
{
	ABU_ASSERT(pDict != NULL && szWord != NULL);

	UINT32 dwHasCovered = 0;
	HashNodePtr currHashNodePtr = NULL;
	UINT32 dwCharacterLength = 0;
	UINT32 	dwHashPos = 0;
	UINT32 dwLenSzWord = strlen(szWord);
	
	/*find index where layer == 1*/
	dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	if(dwCharacterLength == CHINESE_CHARACTER_LENGTH)
	{
        dwHashPos = getDoubleByteHashValue( (BYTE)szWord[dwHasCovered],(BYTE)szWord[dwHasCovered + 1],BUCKET_SIZE);
    }
    else
    {
        dwHashPos = getSingleByteHashValue( (BYTE)szWord[dwHasCovered],BUCKET_SIZE);
    }
	currHashNodePtr = pDict->pGeneralHashTable[dwHashPos];
	
	/*doesn't exist index where layer == 1*/
	if(currHashNodePtr == NULL) return NULL;

	while(currHashNodePtr != NULL)
	{
		if(strncmp(currHashNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength) == 0)
		{
			dwHasCovered += dwCharacterLength;
			break;
		}
		currHashNodePtr = currHashNodePtr->pNext;
	}
	if(currHashNodePtr == NULL) return NULL;
	
	/*find index where layer == 2*/
	if(dwHasCovered == dwLenSzWord)
	{
		if(currHashNodePtr->byIsWord == 1)
			return currHashNodePtr->szWordPosTag;
		else
			return NULL;
	}

	/*find index where layer >= 3*/
	return ABU_WORDS_DICT_getPosTagBinarySearchInTrieNodeArray(currHashNodePtr->pFirstTrieNodeArray,
										currHashNodePtr->dwNumberEntry,szWord,dwHasCovered);
}
/**
@desc:free the content fo pDict && free pDict
*/
void ABU_WORDS_DICT_destroyDict(ABU_WORDS_DICT_S  * pDict)
{
	if(pDict == NULL) return;
	
	HashNodePtr currHashNodePtr = NULL;
	HashNodePtr nextHashNodePtr = NULL;
	UINT32 dwHashPos = 0;
	
	
	for( dwHashPos = 0;dwHashPos < BUCKET_SIZE; dwHashPos ++)
	{
		/*remove index where Layer == 1*/
		if(pDict->pGeneralHashTable[dwHashPos] != NULL)
		{
			currHashNodePtr=pDict->pGeneralHashTable[dwHashPos];
			while(currHashNodePtr != NULL)
			{
				nextHashNodePtr = currHashNodePtr->pNext;
				
				//free the content of currHashNodePtr
				ABU_WORDS_DICT_destroySingleHashNodeContent(currHashNodePtr);
				
				ABU_FREE(currHashNodePtr);
				
				currHashNodePtr = nextHashNodePtr;
			}
		}		
	}
	
	ABU_FREE(pDict);
}

int ABU_WORDS_DICT_outputDict(ABU_WORDS_DICT_S *pDict, char *szOutputDictFile)
{
	ABU_ASSERT(pDict != NULL && szOutputDictFile != NULL);
	
	HashNodePtr tempHashNodePtr;
	TrieNodePtr tempTrieNodePtr;
	char szBuff[MAX_WORD_LENGTH];
	int i = 0,j=0;
	FILE *fp = fopen(szOutputDictFile,"w+");

	ABU_ASSERT( pDict != NULL && szOutputDictFile != NULL);

	/*open output file*/
	if( fp == NULL)
	{
		printf("can't open %s\n",szOutputDictFile);
		return -1;
	}
	memset(szBuff,0,MAX_WORD_LENGTH);

	/*output dict*/
	for(;i<BUCKET_SIZE;)
	{
		if(pDict->pGeneralHashTable[i] != NULL)
		{
			tempHashNodePtr = pDict->pGeneralHashTable[i];
				while( tempHashNodePtr != NULL)/*一级索引*/
				{
					if( tempHashNodePtr->byIsWord == 1)
					{
						strcat(szBuff,tempHashNodePtr->szSingleCharacter);
						/**/
						strcat(szBuff," ");
						strcat(szBuff,tempHashNodePtr->szWordPosTag);
						/**/
						fputs(strcat(szBuff,"\n"),fp);/*output*/
						memset(szBuff,0,MAX_WORD_LENGTH);
					}

					if(tempHashNodePtr->dwNumberEntry > 0)
					{
						
						for(j=0;j<tempHashNodePtr->dwNumberEntry;j++)
						{
							tempTrieNodePtr = tempHashNodePtr->pFirstTrieNodeArray +j;

							if(tempTrieNodePtr->byIsWord == 1)
							{
								strcat(szBuff,tempHashNodePtr->szSingleCharacter);
								strcat(szBuff,tempTrieNodePtr->szSingleCharacter);
								/**/
								strcat(szBuff," ");
								strcat(szBuff,tempTrieNodePtr->szWordPosTag);
								/**/
								fputs(strcat(szBuff,"\n"),fp);/*output*/
								//printf("%s",szBuff);
								memset(szBuff,0,MAX_WORD_LENGTH);
							}
							if(tempTrieNodePtr->dwNumberChild > 0)
							{
								strcat(szBuff,tempHashNodePtr->szSingleCharacter);
								if( ABU_WORDS_DICT_outputSingleTrieNode(tempTrieNodePtr,szBuff,fp) == -1)
									 return -1;
								memset(szBuff,0,MAX_WORD_LENGTH);
							}
						}
					}
					tempHashNodePtr = tempHashNodePtr->pNext;
				}
		}
		i++;
	}
	fclose(fp);
	return 0;	
}
/********************************************************************
***private interface implementation**********************************
*********************************************************************/

int ABU_WORDS_DICT_binarySearchWordInTrieNodeArray(const TrieNodePtr pTrieNodePtrArray,
												   const UINT32 dwNumberChild,const char * szWord,UINT32 dwHasCovered)
{
	ABU_ASSERT(pTrieNodePtrArray != NULL && szWord != NULL);
	
	UINT32 dwLenSzWord = strlen(szWord);
	TrieNodePtr currTrieNodePtr = NULL;
	UINT32 dwCharacterLength = 0;
	int lowPos = 0;
	int highPos = 0;
	int middPos = 0;
	int compareFlag = -1;

	dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);

	/*binary search*/
	
	lowPos = 0;
	highPos = dwNumberChild -1;
	while( lowPos <= highPos)
	{
		middPos = (lowPos + highPos)/2;
		currTrieNodePtr = pTrieNodePtrArray + middPos;
		compareFlag = strncmp(currTrieNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength);
		if( compareFlag == 0 )
		{
			dwHasCovered += dwCharacterLength;
			break;
		}
		else if (compareFlag < 0)
		{
			lowPos = middPos + 1;
		}
		else
		{
			highPos = middPos - 1;
		}	
	}	
	if(compareFlag != 0)/*二分查找未果*/
		return -1;
	currTrieNodePtr = pTrieNodePtrArray + middPos;
	if( dwHasCovered == dwLenSzWord)
	{
		if(currTrieNodePtr->byIsWord == 1 )
			return 0;
		else
			return -1;
	}
	else
	{
		if(currTrieNodePtr->dwNumberChild == 0)
			return -1;
		/*recursive call ABU_WORDS_DICT_binarySearchWordInTrieNodeArray*/
		return ABU_WORDS_DICT_binarySearchWordInTrieNodeArray(currTrieNodePtr->pFirstChildArray,currTrieNodePtr->dwNumberChild,szWord,dwHasCovered);
	}
}
const char *ABU_WORDS_DICT_getPosTagBinarySearchInTrieNodeArray(const TrieNodePtr pTrieNodePtrArray,
													const UINT32 dwNumberChild,const char *szWord,UINT32 dwHasCovered)
{
	ABU_ASSERT(pTrieNodePtrArray != NULL && szWord != NULL);
	
	UINT32 dwLenSzWord = strlen(szWord);
	TrieNodePtr currTrieNodePtr = NULL;
	UINT32 dwCharacterLength = 0;
	int lowPos = 0;
	int highPos = 0;
	int middPos = 0;
	int compareFlag = -1;

	dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);

	/*binary search*/
	
	lowPos = 0;
	highPos = dwNumberChild -1;
	while( lowPos <= highPos)
	{
		middPos = (lowPos + highPos)/2;
		currTrieNodePtr = pTrieNodePtrArray + middPos;
		compareFlag = strncmp(currTrieNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength);
		if( compareFlag == 0 )
		{
			dwHasCovered += dwCharacterLength;
			break;
		}
		else if (compareFlag < 0)
		{
			lowPos = middPos + 1;
		}
		else
		{
			highPos = middPos - 1;
		}	
	}	
	if(compareFlag != 0)/*二分查找未果*/
		return NULL;
	currTrieNodePtr = pTrieNodePtrArray + middPos;
	if( dwHasCovered == dwLenSzWord)
	{
		if(currTrieNodePtr->byIsWord == 1 )
			return currTrieNodePtr->szWordPosTag;
		else
			return NULL;
	}
	else
	{
		if(currTrieNodePtr->dwNumberChild == 0)
			return NULL;
		/*recursive call ABU_WORDS_DICT_binarySearchWordInTrieNodeArray*/
		return ABU_WORDS_DICT_getPosTagBinarySearchInTrieNodeArray
				(currTrieNodePtr->pFirstChildArray,currTrieNodePtr->dwNumberChild,szWord,dwHasCovered);
	}
}


void ABU_WORDS_DICT_destroySingleHashNodeContent(HashNodePtr pHashNodePtr)
{
	if(pHashNodePtr == NULL) return;
	
	UINT32 dwNumberEntry = 0;
	TrieNodePtr tempTrieNodePtr = NULL;
	
      /*step1:free szWordPosTag*/

	ABU_FREE(pHashNodePtr->szWordPosTag);
	

	/*step2:free pHashNodePtr->pFirstTrieNodeArray*/
	for(dwNumberEntry = 0;dwNumberEntry < pHashNodePtr->dwNumberEntry ;dwNumberEntry++)
	{
		tempTrieNodePtr = pHashNodePtr->pFirstTrieNodeArray + dwNumberEntry;
		ABU_WORDS_DICT_destroySingleTrieNodeContent(tempTrieNodePtr);
	}
	
	ABU_FREE(pHashNodePtr->pFirstTrieNodeArray);

}


/**
@name: ABU_WORDS_DICT_addHashNode
@desc: 负责一层索引
@param:
@return: HashNodePtr
*/
HashNodePtr ABU_WORDS_DICT_addHashNode(ABU_WORDS_DICT_S *pDict,const char * szWord,const char *szWordPosTag, UINT32 dwHasCovered)
{
	ABU_ASSERT(pDict != NULL && szWord != NULL);
	
	UINT32 hashPos = 0;
	UINT32 dwLenSzWord = strlen(szWord);
	HashNodePtr currHashNodePtr = NULL;
	HashNodePtr preHashNodePtr = NULL;
	UINT32 dwCharacterLength = 0;
	int tmpLen = 0;
	/*find index where Layer == 1*/
	if( szWord[dwHasCovered] < 0)
	{
		dwCharacterLength = CHINESE_CHARACTER_LENGTH;
		hashPos = getDoubleByteHashValue((BYTE)szWord[dwHasCovered],(BYTE)szWord[dwHasCovered+1],BUCKET_SIZE);
	}
	else
	{
		dwCharacterLength = OTHER_CHARACTER_LENGTH;
		hashPos = getSingleByteHashValue((BYTE)szWord[dwHasCovered],BUCKET_SIZE);
	}
	
	if( pDict->pGeneralHashTable[hashPos] ==NULL)
	{
	    /*malloc new hashnode*/
		currHashNodePtr = (HashNodePtr)calloc(1,sizeof(struct HashNode));
		if(currHashNodePtr == NULL) return NULL;

		strncpy(currHashNodePtr->szSingleCharacter,szWord+dwHasCovered,2);
		pDict->pGeneralHashTable[hashPos] = currHashNodePtr;
		pDict->pGeneralHashTable[hashPos]->dwNumberEntry  = 0;
		dwHasCovered += dwCharacterLength;
		
		if( dwHasCovered == dwLenSzWord)
		{
			currHashNodePtr->byIsWord =1;
			tmpLen = strlen(szWordPosTag);
			currHashNodePtr->szWordPosTag = (char *)calloc(tmpLen + 1,sizeof(char));
			if(currHashNodePtr->szWordPosTag == NULL)
            {
                ABU_WORDS_DICT_destroySingleHashNodeContent(currHashNodePtr);
                return NULL;
            }
			strncpy(currHashNodePtr->szWordPosTag,szWordPosTag,tmpLen);
		}
		else
			currHashNodePtr->byIsWord = 0;
		return currHashNodePtr;
	}
	else 
	{
		/*find in bucket*/
		currHashNodePtr = pDict->pGeneralHashTable[hashPos];
		while(currHashNodePtr != NULL)
		{
			if( strncmp(currHashNodePtr->szSingleCharacter,szWord+dwHasCovered,dwCharacterLength) == 0)
			{
				break;
			}
			else
			{
				preHashNodePtr = currHashNodePtr;
				currHashNodePtr = currHashNodePtr->pNext;
			}
		}
		/*exist in hash node link list*/
		if(currHashNodePtr != NULL)
		{
			return currHashNodePtr;
		}
		else /*constructor hash bucket linker list*/
		{				
			currHashNodePtr = (HashNodePtr)calloc(1,sizeof(struct HashNode));
			if(currHashNodePtr == NULL) return NULL;

			strncpy(currHashNodePtr->szSingleCharacter,szWord+dwHasCovered,dwCharacterLength);
			currHashNodePtr->dwNumberEntry = 0;//head of the link list
			dwHasCovered += dwCharacterLength;
			if( dwHasCovered == dwLenSzWord)
			{
				currHashNodePtr->byIsWord = 1;
				tmpLen = strlen(szWordPosTag);
				currHashNodePtr->szWordPosTag = (char *)calloc(tmpLen + 1,sizeof(char));
				if(currHashNodePtr->szWordPosTag == NULL)
                {
                    ABU_WORDS_DICT_destroySingleHashNodeContent(currHashNodePtr);
                    return NULL;
                }
				strncpy(currHashNodePtr->szWordPosTag,szWordPosTag,tmpLen);
			}
			else
			{
				currHashNodePtr->byIsWord = 0;
			}
			preHashNodePtr->pNext = currHashNodePtr;
			return currHashNodePtr;
		}
	}
	
ABU_WORDS_DICT_addHashNode_POST:
	ABU_WORDS_DICT_destroySingleHashNodeContent(currHashNodePtr);
	return NULL;

}

/**
@name: ABU_WORDS_DICT_addTrieNodeIntoHashNode
@desc: 负责二层索引
@param:
@return: TrieNodePtr
*/
TrieNodePtr ABU_WORDS_DICT_addTrieNodeIntoHashNode(HashNodePtr pCurrHashNodePtr,const char *szWord,
												   const char *szWordPosTag, UINT32 dwHasCovered)
{
	ABU_ASSERT(pCurrHashNodePtr != NULL && szWord != NULL);
	
	int i = 0;
	TrieNodePtr currTrieNodePtr = NULL;
	TrieNodePtr tempTrieNodePtr = NULL;
	TrieNodePtr preTrieNodePtr = NULL;
	TrieNodePtr tempTrieNodePtrArray = NULL;
	char szTempBuff[MAX_WORD_LENGTH];
	UINT32 dwCharacterLength=0;
	UINT32 dwNextStepLength = 0;
	
	dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	memset(szTempBuff,0,MAX_WORD_LENGTH);

	if(pCurrHashNodePtr->pFirstTrieNodeArray == NULL)/*layer=2*/
	{
		/*D(exist), (add) DAA*/
		
		pCurrHashNodePtr->pFirstTrieNodeArray = (TrieNodePtr)calloc(1,sizeof(struct TrieNode));
		if(pCurrHashNodePtr->pFirstTrieNodeArray == NULL) return NULL;
		pCurrHashNodePtr->dwNumberEntry = 1;

		/*recursive add singleTrieNode*/
		if( ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(pCurrHashNodePtr->pFirstTrieNodeArray,szWord,szWordPosTag,dwHasCovered) == -1)
		{
					return NULL;
		}
		return pCurrHashNodePtr->pFirstTrieNodeArray;		
	}
	else
	{
		/*DA(exist),(add)DAYA*/
		currTrieNodePtr=pCurrHashNodePtr->pFirstTrieNodeArray+pCurrHashNodePtr->dwNumberEntry-1;//the last
		if(strncmp(currTrieNodePtr->szSingleCharacter,szWord+dwHasCovered,dwCharacterLength) == 0)
		{
			/*layer >= 3 index logic*/
			if(currTrieNodePtr->dwNumberChild != 0)
			{
				tempTrieNodePtr = currTrieNodePtr->pFirstChildArray+currTrieNodePtr->dwNumberChild -1;
				dwNextStepLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered + dwCharacterLength]);
				while(strncmp(tempTrieNodePtr->szSingleCharacter,szWord+dwHasCovered+dwCharacterLength,dwNextStepLength) == 0)
				{
					dwHasCovered += dwCharacterLength;
					currTrieNodePtr = tempTrieNodePtr;
					if(currTrieNodePtr->dwNumberChild !=0)
					{
						tempTrieNodePtr = currTrieNodePtr->pFirstChildArray+currTrieNodePtr->dwNumberChild -1;
					}
					else
					{
						break;
					}
					dwCharacterLength = dwNextStepLength;
					dwNextStepLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered + dwCharacterLength]);
				}
			}	
			if(currTrieNodePtr->dwNumberChild == 0)
			{
				preTrieNodePtr = currTrieNodePtr;
				currTrieNodePtr = NULL;
				return ABU_WORDS_DICT_addTrieNodeIntoTrieNode(preTrieNodePtr,currTrieNodePtr,szWord,szWordPosTag,dwHasCovered);
			}
			else
			{
				preTrieNodePtr = currTrieNodePtr;
				currTrieNodePtr =  currTrieNodePtr->pFirstChildArray + currTrieNodePtr->dwNumberChild -1;
				dwHasCovered += dwCharacterLength;
				return ABU_WORDS_DICT_addTrieNodeIntoTrieNode(preTrieNodePtr,currTrieNodePtr,szWord,szWordPosTag,dwHasCovered);
			}			
		}
		else/*DA(exist),(add)DBC*/
		{
				/*copy the TrieNodeArray to a tempArray*/
			tempTrieNodePtrArray = (TrieNodePtr)calloc(1,pCurrHashNodePtr->dwNumberEntry*sizeof(struct TrieNode));
			if(tempTrieNodePtrArray == NULL) return NULL;
			
			if( ABU_WORDS_DICT_copyArrayTrieNode(tempTrieNodePtrArray,pCurrHashNodePtr->pFirstTrieNodeArray,pCurrHashNodePtr->dwNumberEntry) == -1)
			{
					return NULL;
			}
			/*free TrieNodeArray*/
			ABU_WORDS_DICT_destroyArrayTrieNodeContent(pCurrHashNodePtr->pFirstTrieNodeArray,pCurrHashNodePtr->dwNumberEntry);

			ABU_FREE(pCurrHashNodePtr->pFirstTrieNodeArray);
			
			/*copy the tempArray to TrieNodeArray*/
			pCurrHashNodePtr->pFirstTrieNodeArray = (TrieNodePtr )calloc(pCurrHashNodePtr->dwNumberEntry + 1,sizeof(struct TrieNode));
			for( i =0;i< pCurrHashNodePtr->dwNumberEntry;i++)
			{
				ABU_WORDS_DICT_copySingleTrieNode(pCurrHashNodePtr->pFirstTrieNodeArray+i,tempTrieNodePtrArray+i);
			}

			/*destroy the tempArray*/
			ABU_WORDS_DICT_destroyArrayTrieNodeContent(tempTrieNodePtrArray,pCurrHashNodePtr->dwNumberEntry);
			
			ABU_FREE(tempTrieNodePtrArray);

			/*alter the state of the HashNodePtr*/
			tempTrieNodePtr = pCurrHashNodePtr->pFirstTrieNodeArray + pCurrHashNodePtr->dwNumberEntry;
			if( ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(tempTrieNodePtr,szWord,szWordPosTag,dwHasCovered) == -1)
			{
					return NULL;
			}
			pCurrHashNodePtr->dwNumberEntry += 1;
			
			return tempTrieNodePtr;
			//pCurrHashNodePtr->pFirstTrieNodeArray = (TrieNodePtr)realloc(pCurrHashNodePtr,
			//				(pCurrHashNodePtr->dwNumberEntry + 1) * sizeof(struct TrieNode));
			//if(pCurrHashNodePtr->pFirstTrieNodeArray == NULL) return NULL;

			///*alter the state of the HashNodePtr*/
			//tempTrieNodePtr = pCurrHashNodePtr->pFirstTrieNodeArray + pCurrHashNodePtr->dwNumberEntry;
			//memset(tempTrieNodePtr,0,sizeof(struct TrieNode));
			//if( ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(tempTrieNodePtr,szWord,szWordPosTag,dwHasCovered) == -1)
			//	return NULL;
			//pCurrHashNodePtr->dwNumberEntry += 1;
			//
			//return tempTrieNodePtr;

		}	
	}
}
/**
@name ABU_DICT_addTrieNodeIntoTrieNode
@desc 负责三层或者三层以上的索引
@param
@return 
	succeed return 0
	fail    return -1
*/
TrieNodePtr	ABU_WORDS_DICT_addTrieNodeIntoTrieNode( TrieNodePtr pPreTrieNodePtr,TrieNodePtr pCurrTrieNodePtr,
												   const char *szWord,const char *szWordPosTag,UINT32 dwHasCovered)
{
	TrieNodePtr tempTrieNodePtr = NULL;
	TrieNodePtr tempTrieNodePtrArray = NULL;
	char szTempBuff[MAX_WORD_LENGTH];
	int i = 0;
	UINT32 dwCharacterLength=0;

	dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	memset(szTempBuff,0,MAX_WORD_LENGTH);
	if(strncmp(pPreTrieNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength) == 0 
		&& pCurrTrieNodePtr == NULL)
	{		
			/*create seconde TrieNode layer=3*/
		    pPreTrieNodePtr->pFirstChildArray = (TrieNodePtr)calloc(1,sizeof(struct TrieNode));
			if(pPreTrieNodePtr->pFirstChildArray == NULL) return NULL;

			dwHasCovered += dwCharacterLength;
			if ( ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(pPreTrieNodePtr->pFirstChildArray,szWord,szWordPosTag,dwHasCovered ) == -1)
				return NULL;			
			pPreTrieNodePtr->dwNumberChild +=1;
			return pPreTrieNodePtr->pFirstChildArray;//*the most right_down*/
	}
	else
	{
	  /*pPreTrieNodePtr->pFirstChildArray = (TrieNodePtr)realloc(pPreTrieNodePtr->pFirstChildArray,
			(pPreTrieNodePtr->dwNumberChild + 1) * sizeof(struct TrieNode));
		if(pPreTrieNodePtr == NULL) return NULL;

		tempTrieNodePtr = pPreTrieNodePtr->pFirstChildArray + pPreTrieNodePtr->dwNumberChild;
		memset(tempTrieNodePtr,0,sizeof(struct TrieNode));
		if( ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(tempTrieNodePtr,szWord,szWordPosTag,dwHasCovered) == -1)
			return NULL;
		pPreTrieNodePtr->dwNumberChild += 1;
	
		return tempTrieNodePtr;*/

		//copy TrieNodeArray to tempTrieNodeArray
		tempTrieNodePtrArray = (TrieNodePtr)calloc(pPreTrieNodePtr->dwNumberChild,sizeof(struct TrieNode));
		if( ABU_WORDS_DICT_copyArrayTrieNode(tempTrieNodePtrArray,pPreTrieNodePtr->pFirstChildArray,pPreTrieNodePtr->dwNumberChild) == -1)
		{
				return NULL;
		}
		
		//free TrieNodeArray
		ABU_WORDS_DICT_destroyArrayTrieNodeContent(pPreTrieNodePtr->pFirstChildArray,pPreTrieNodePtr->dwNumberChild);
		ABU_FREE(pPreTrieNodePtr->pFirstChildArray);
		
		//copy tempTrieNodeArray to tempTrieNodeArray
		pPreTrieNodePtr->pFirstChildArray = (TrieNodePtr)calloc(pPreTrieNodePtr->dwNumberChild + 1,sizeof(struct TrieNode));
		for( i=0;i<pPreTrieNodePtr->dwNumberChild;i++)
		{
			if(ABU_WORDS_DICT_copySingleTrieNode(pPreTrieNodePtr->pFirstChildArray+i,tempTrieNodePtrArray+i) == -1) return NULL;
		}

		//free tempTrieNodeArray
		ABU_WORDS_DICT_destroyArrayTrieNodeContent(tempTrieNodePtrArray,pPreTrieNodePtr->dwNumberChild);
		ABU_FREE(tempTrieNodePtrArray);

		tempTrieNodePtr = pPreTrieNodePtr->pFirstChildArray + pPreTrieNodePtr->dwNumberChild;
		if( ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(tempTrieNodePtr,szWord,szWordPosTag,dwHasCovered) == -1)
		{
			return NULL;
		}
		pPreTrieNodePtr->dwNumberChild += 1;
	
		return tempTrieNodePtr;

	}	
}
/**
@name:ABU_DICT_addSingleTrieNodeIntoTireNode
@desc:其逻辑与ABU_DICT_addTrieNodeIntoTireNode不同，只用在不冲突的情况下构造底层结点
*/
int ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(TrieNodePtr pCurrTrieNodePtr,const char *szWord,
												 const char *szWordPosTag,UINT32 dwHasCovered)
{
	ABU_ASSERT(pCurrTrieNodePtr != NULL && szWord != NULL);
	
       UINT32 dwLenSzWord = strlen(szWord);
	UINT32 dwCharacterLength = 0;
	int len =0 ;

	if( dwHasCovered < dwLenSzWord)
	{
		dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
		strncpy(pCurrTrieNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength);
		dwHasCovered += dwCharacterLength;
		if(dwHasCovered < dwLenSzWord)
		{	
			pCurrTrieNodePtr->dwNumberChild = 1;
			pCurrTrieNodePtr->pFirstChildArray = (TrieNodePtr)calloc(1,sizeof(struct TrieNode));
			if(pCurrTrieNodePtr->pFirstChildArray == NULL) return -1;
			pCurrTrieNodePtr->dwNumberChild =1;

			/*recursive call ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode*/
			return ABU_WORDS_DICT_addSingleTrieNodeIntoTrieNode(pCurrTrieNodePtr->pFirstChildArray,szWord,szWordPosTag,dwHasCovered);
		}
		else
		{
			pCurrTrieNodePtr->byIsWord = 1;
			len = strlen(szWordPosTag);
			pCurrTrieNodePtr->szWordPosTag = (char *)calloc(strlen(szWordPosTag) + 1,sizeof(char));
			if(pCurrTrieNodePtr->szWordPosTag == NULL) return -1; 
			strncpy(pCurrTrieNodePtr->szWordPosTag,szWordPosTag,strlen(szWordPosTag));
			return 0;

		}
	}
	return -1;
}

/**
@return: succeed 0;fail -1;
*/
int ABU_WORDS_DICT_hasSingleTrieNode( TrieNodePtr pTrieNodePtr)
{
	ABU_ASSERT( pTrieNodePtr != NULL); 
	
	while(pTrieNodePtr != NULL)
	{
		if(pTrieNodePtr->dwNumberChild >1)
		{	
			return -1;
		}
		else if(pTrieNodePtr->dwNumberChild == 1)
		{
			pTrieNodePtr = pTrieNodePtr->pFirstChildArray;
		}
		else//pTrieNodePtr->dwNumberChile == 0
		{
			return 0;
		}
	}
	return -1;
}
/**
@name: ABU_WORDS_DICT_destroySingleTrieNode
@desc: just free the content of pTrieNodePtr
*/
void ABU_WORDS_DICT_destroySingleTrieNodeContent(TrieNodePtr pTrieNodePtr)
{
	if(pTrieNodePtr == NULL) return;

	TrieNodePtr tempTrieNodePtr;
	UINT32 dwNumberChild = pTrieNodePtr->dwNumberChild;
	UINT32 i = 0;
   //step 1:destroy dynamic memory
	if(pTrieNodePtr->byIsWord == 1)
	{
		ABU_FREE(pTrieNodePtr->szWordPosTag);
	}

	//step 2:destroy children
	for (i=0;i < dwNumberChild ; i++)
	{
		tempTrieNodePtr = pTrieNodePtr->pFirstChildArray + i;

		/*recursive call ABU_WORDS_DICT_destroySingleTrieNode*/
		ABU_WORDS_DICT_destroySingleTrieNodeContent(tempTrieNodePtr);
	}
	//step 3:destroy this children array pointer
	ABU_FREE(pTrieNodePtr->pFirstChildArray);
	
} 
/**
@return: just free the content of pTrieNodePtrArray
*/
void ABU_WORDS_DICT_destroyArrayTrieNodeContent(TrieNodePtr pTrieNodePtrArray,const UINT32 dwNumberTrieNode)
{
	if(pTrieNodePtrArray == NULL) return;
	
	TrieNodePtr tempTrieNodePtr;
	UINT32 i = 0;
	for( i = 0;i<dwNumberTrieNode;i++)
	{
		tempTrieNodePtr = pTrieNodePtrArray + i;
		ABU_WORDS_DICT_destroySingleTrieNodeContent(tempTrieNodePtr); 
	}
}
/**
@name: ABU_WORDS_DICT_copySingleTrieNode
@return: succeed 0;fail -1
*/
int ABU_WORDS_DICT_copySingleTrieNode(TrieNodePtr pDestTrieNodePtr,const TrieNodePtr pSrcTrieNodePtr)
{
	ABU_ASSERT( pDestTrieNodePtr != NULL && pSrcTrieNodePtr != NULL);
	TrieNodePtr tempDestTrieNodePtr;
	TrieNodePtr tempSrcTrieNodePtr;
	int i=0;

	/*copy szSingleCharacter*/
	
    strncpy(pDestTrieNodePtr->szSingleCharacter,pSrcTrieNodePtr->szSingleCharacter,2);

	//copy byIsWord
	pDestTrieNodePtr->byIsWord = pSrcTrieNodePtr->byIsWord;

	//copy szWordPosTag
	if(pSrcTrieNodePtr->byIsWord == 1)
	{
		pDestTrieNodePtr->szWordPosTag = (char *)calloc(1,strlen(pSrcTrieNodePtr->szWordPosTag) + 1);
		if(pDestTrieNodePtr->szWordPosTag == NULL)
        {
            ABU_WORDS_DICT_destroySingleTrieNodeContent(pDestTrieNodePtr);
            return -1;
        } 
		strncpy(pDestTrieNodePtr->szWordPosTag,pSrcTrieNodePtr->szWordPosTag,strlen(pSrcTrieNodePtr->szWordPosTag));
	}
	/*copy child*/
	if(pSrcTrieNodePtr->dwNumberChild == 0)
	{
		pDestTrieNodePtr->pFirstChildArray = NULL;
	}
	else
	{
		pDestTrieNodePtr->pFirstChildArray = (TrieNodePtr)calloc(1,pSrcTrieNodePtr->dwNumberChild *sizeof(struct TrieNode));
		if(pDestTrieNodePtr->pFirstChildArray == NULL) 
        {
            ABU_WORDS_DICT_destroySingleTrieNodeContent(pDestTrieNodePtr);
            return -1;
        } 
	}
	for(i =0; i< pSrcTrieNodePtr->dwNumberChild;i++)
	{
		tempDestTrieNodePtr = pDestTrieNodePtr->pFirstChildArray + i;
		tempSrcTrieNodePtr = pSrcTrieNodePtr->pFirstChildArray + i;
		if( ABU_WORDS_DICT_copySingleTrieNode(tempDestTrieNodePtr,tempSrcTrieNodePtr) == -1)
			return -1;
		pDestTrieNodePtr->dwNumberChild += 1;
	}
	
	return 0;
}
/**
@name:ABU_WORDS_DICT_copyArrayTrieNode
@return: succeed 0;fail -1
*/
int ABU_WORDS_DICT_copyArrayTrieNode(TrieNodePtr pTrieNodeDestArray,const TrieNodePtr pTrieNodeSrcArray,const UINT32 dwTrieNodeNumber)
{
	ABU_ASSERT( pTrieNodeDestArray != NULL && pTrieNodeSrcArray != NULL);
	
	UINT32 i = 0;
	TrieNodePtr tempDestTrieNodePtr;
	TrieNodePtr tempSrcTrieNodePtr;
	for( i=0;i<dwTrieNodeNumber;i++)
	{
		tempDestTrieNodePtr = pTrieNodeDestArray + i;
		tempSrcTrieNodePtr = pTrieNodeSrcArray + i;
		if ( ABU_WORDS_DICT_copySingleTrieNode(tempDestTrieNodePtr,tempSrcTrieNodePtr) == -1)
			return -1;
	}
	return 0;
}

/**
@name:ABU_WORDS_DICT_getHashNodePtr
*/
HashNodePtr ABU_WORDS_DICT_getHashNodePtr(const ABU_WORDS_DICT_S * pDict,const char *szWord,const UINT32 dwHasCovered)
{
	ABU_ASSERT(pDict != NULL && szWord != NULL);
	
	HashNodePtr currHashNodePtr = NULL;
	UINT32 dwHashPos = getHashPos(szWord,dwHasCovered);
	UINT32 dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	currHashNodePtr = pDict->pGeneralHashTable[dwHashPos];
	
	if(currHashNodePtr == NULL)return NULL;
	
	while(currHashNodePtr != NULL)
	{
		if(strncmp(currHashNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength) == 0) break;
		
		currHashNodePtr = currHashNodePtr->pNext;
	}
	return currHashNodePtr;
}
/**
@name:ABU_WORDS_DICT_getNextTrieNodePtrByHashNodePtr
@return: fail return NULL;
*/
TrieNodePtr ABU_WORDS_DICT_getNextTrieNodePtrByHashNodePtr(const HashNodePtr pCurrHashNodePtr,const char * szWord,const UINT32 dwHasCovered)
{
	ABU_ASSERT(pCurrHashNodePtr != NULL && szWord != NULL);
	
	UINT32 dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	TrieNodePtr currTrieNodePtr = NULL;
	int lowPos = 0;
	int highPos = pCurrHashNodePtr->dwNumberEntry -1;
	int middPos = 0;
	int compareFlag = -1;
	if(pCurrHashNodePtr->pFirstTrieNodeArray == NULL)
		return NULL;

	/*binary search*/
	while( lowPos <= highPos)
	{
		middPos = (lowPos + highPos)/2;
		currTrieNodePtr = pCurrHashNodePtr->pFirstTrieNodeArray + middPos;
		compareFlag = strncmp(currTrieNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength);
		if(compareFlag == 0)
		{
			return currTrieNodePtr;
		}
		else if(compareFlag < 0)
		{
			lowPos = middPos + 1;
		}
		else
		{
			highPos = middPos - 1;
		}
	}
	return NULL;
}
/**
@name:
@desc:
@param:
@return:
*/
TrieNodePtr ABU_WORDS_DICT_getNextTrieNodePtrByTrieNodePtr(const TrieNodePtr pCurrTrieNodePtr,const char *szWord, const UINT32 dwHasCovered)
{
	ABU_ASSERT(pCurrTrieNodePtr != NULL && szWord != NULL);
	
	UINT32 dwCharacterLength = GET_NEXT_COMP_LENGTH(szWord[dwHasCovered]);
	TrieNodePtr currTrieNodePtr = NULL;
	int lowPos = 0;
	int highPos = highPos = pCurrTrieNodePtr->dwNumberChild - 1;
	int middPos = 0;
	int compareFlag = -1;
	if(pCurrTrieNodePtr->pFirstChildArray == NULL)
		return NULL;

	/*binary search*/
	while( lowPos <= highPos)
	{
		middPos = (lowPos + highPos)/2;
		currTrieNodePtr = pCurrTrieNodePtr->pFirstChildArray + middPos;
		compareFlag = strncmp(currTrieNodePtr->szSingleCharacter,szWord + dwHasCovered,dwCharacterLength);
		if(compareFlag == 0)
		{
			return currTrieNodePtr;
		}
		else if(compareFlag < 0)
		{
			lowPos = middPos + 1;
		}
		else
		{
			highPos = middPos - 1;
		}
	}
	return NULL;
	
}
/**
@desc:split szBuff into word and wordPosTag
@desc: szBuff Format: word wordPosTag;
@param: szBuff
		szword: has been memseted 0
		szWordPosTag:has beeen memseted 0
*/
void getWordAndPosTag(const char *szBuff,char *szWord,char *szWordPosTag)
{
	ABU_ASSERT(szBuff != NULL && szWord != NULL && szWordPosTag != NULL);
	
	UINT32 dwLenSzBuff = strlen(szBuff);
	char * p = strchr(szBuff,SPACE);
	int position = p - szBuff;
	strncpy(szWord,szBuff,position);
	strncpy(szWordPosTag,szBuff + position + 1,dwLenSzBuff - position - 1);
}

int ABU_WORDS_DICT_outputSingleTrieNode(const TrieNodePtr pTrieNodePtr,char *szBuff,FILE *fp)
{
	ABU_ASSERT(pTrieNodePtr != NULL && szBuff != NULL && fp != NULL);
	
	TrieNodePtr tempTrieNodePtr;
	char tempSzBuff[MAX_WORD_LENGTH];
	char recurSzBuff[MAX_WORD_LENGTH];/*用于递归完成，重新更新szBuff*/
	int i;
	memset(tempSzBuff,0,MAX_WORD_LENGTH);
	memset(recurSzBuff,0,MAX_WORD_LENGTH);
	strcat(recurSzBuff,szBuff);
	strcat(szBuff,pTrieNodePtr->szSingleCharacter);
	for(i=0; i< pTrieNodePtr->dwNumberChild; i++)
	{
		tempTrieNodePtr = pTrieNodePtr->pFirstChildArray +i;
		if(tempTrieNodePtr->byIsWord == 1)/*二层索引*/
		{
			strcpy(tempSzBuff,szBuff);
			strcat(tempSzBuff,tempTrieNodePtr->szSingleCharacter);
		    strcat(tempSzBuff," ");
		    strcat(tempSzBuff,tempTrieNodePtr->szWordPosTag);
		    fputs(strcat(tempSzBuff,"\n"),fp);
		    fflush(fp);

		}

		if(tempTrieNodePtr->dwNumberChild > 0)
		{
			if( ABU_WORDS_DICT_outputSingleTrieNode(tempTrieNodePtr,szBuff,fp) == -1)
				return -1;
		}
	}
	memset(szBuff,0,sizeof(szBuff));
	strcat(szBuff,recurSzBuff);
	return 0;
}

int sortStrcmp(const void *arg1,const void *arg2)
{
	ABU_ASSERT(arg1 != NULL && arg2 != NULL);
	char **szLeft = (char **)arg1;
	char **szRight = (char **)arg2;
	
	return strcmp(*szLeft,*szRight);
}
