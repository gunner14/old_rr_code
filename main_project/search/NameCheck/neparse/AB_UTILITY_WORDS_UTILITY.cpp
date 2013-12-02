/************************************************
 * Filename: AB_UTILITY_WORDS_UTILITY.h
 * Author:bfzhang
 * Date: 2007-03-21
 * Dec: define some public interface for index and search module
 ***********************************************/
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_UTILITY.h" 
#include "STRING_UTILITY.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
 
/*the following are public interfaces for index and search module**/
BOOL ABU_WORDS_UTILITY_isSingleChineseCharacter(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	return (strcmp(stpWord->szWordPosTag,POS_UN) == 0)?TRUE:FALSE;
}

BOOL ABU_WORDS_UTILITY_isIndexWordExceptSingleChineseCharacter(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	//printf("postag[%s] => [%s]\n", stpWord->szWord, stpWord->szWordPosTag);
	if( strstr(stpWord->szWordPosTag,POS_W) == 0 && strstr(stpWord->szWordPosTag,POS_UN) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL ABU_WORDS_UTILITY_isProvince(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	
	return (strstr(stpWord->szWordPosTag,POS_NS1) != NULL)?TRUE:FALSE;
}

BOOL ABU_WORDS_UTILITY_isCity(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	
	return (strstr(stpWord->szWordPosTag,POS_NS2) != NULL)?TRUE:FALSE;
}

BOOL ABU_WORDS_UTILITY_isCounty(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	
	return (strstr(stpWord->szWordPosTag,POS_NS3) != NULL)?TRUE:FALSE;
}
/*is english word*/
BOOL ABU_WORDS_UTILITY_isEnglish(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	
	return (strstr(stpWord->szWordPosTag,POS_ENG) != NULL)?TRUE:FALSE;
}
/*is number word*/
BOOL ABU_WORDS_UTILITY_isNumber(const ABU_WORDS_WORD_S *stpWord)
{
	ABU_ASSERT(stpWord != NULL);
	
	return (strstr(stpWord->szWordPosTag,POS_ABA) != NULL)?TRUE:FALSE;
}

 /**the following are private interface just for AB_UTILITY_WORDS Module**/
 int  ABU_WORDS_PREPROCESS_all(char *szBuff)
{
	ABU_ASSERT( szBuff != NULL);
	
    if(STRU_SBCToDBC(szBuff) == -1)
        return -1;
    if(STRU_removeTabAndNewline(szBuff) == -1)
        return -1;
    if(STRU_combineMultipleSpaceToOne(szBuff) == -1)
        return -1;
    if(STRU_toLowCase(szBuff) == -1)
        return -1;
        
    return 0;
}
UINT32 getDoubleByteHashValue(const BYTE byLow,const BYTE byHigh,const UINT32 dwHashBucketSize)
{
	UINT32 sum = 0;
	sum = byLow;
	sum = sum << 8;
	sum += byHigh;
	return (sum %dwHashBucketSize);
}

UINT32 getSingleByteHashValue(const BYTE byValue,const UINT32 dwHashBucketSize)
{
	return (byValue %dwHashBucketSize);
}

UINT32 getHashPos(const char *szWord,const UINT32 dwHasCovered)
{
	if(szWord[dwHasCovered] < 0)
		return getDoubleByteHashValue((BYTE)szWord[dwHasCovered],(BYTE)szWord[dwHasCovered + 1],BUCKET_SIZE);
	else
		return getSingleByteHashValue((BYTE)szWord[dwHasCovered],BUCKET_SIZE);
}

char * getCurrTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time_t now_time=time( &rawtime);
	timeinfo=localtime(&now_time);
	return asctime(timeinfo);
}
