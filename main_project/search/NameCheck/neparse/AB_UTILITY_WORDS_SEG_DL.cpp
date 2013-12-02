/**************************************************
*Filename:AB_UTILITY_WORDS_SEG.h
*Author:  bfzhang
*Date:  2007-01-20
*Version:0.1
*Description:define the implementation about segmentation
***************************************************/
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_UTILITY.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_DL.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include "STRING_UTILITY.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


ABU_WORDS_SEGRESULT_DL_S * ABU_WORDS_SEG_DL_getSegResultFromBuff(const char * sBuff,UINT64 ulLenSBuff,
							    const ABU_WORDS_DICT_S *pBasicDict,const ABU_WORDS_DICT_S *pGroupDict,const ABU_WORDS_SEGLIB_S *pSegLib)
{
	ABU_ASSERT(sBuff != NULL && pBasicDict != NULL && pGroupDict != NULL && pSegLib != NULL);
	
	ABU_WORDS_SEGRESULT_DL_S *pSegResult = NULL;

	ABU_ASSERT(sBuff != NULL && pBasicDict != NULL && pGroupDict != NULL);

    	pSegResult = (ABU_WORDS_SEGRESULT_DL_S *)calloc(1,sizeof(ABU_WORDS_SEGRESULT_DL_S));
	if(pSegResult == NULL) return NULL;

	pSegResult->pBasicSegResult = ABU_WORDS_SEG_SL_getSegResultFromBuff(sBuff,ulLenSBuff,pBasicDict,pSegLib);
	if(pSegResult->pBasicSegResult == NULL) 
    {
        ABU_WORDS_SEG_DL_destroySegResult(pSegResult);
        return NULL;
    }

	pSegResult->pGroupSegResult = ABU_WORDS_SEG_SL_getSegResultFromBuff(sBuff,ulLenSBuff,pGroupDict,pSegLib);
	if(pSegResult->pGroupSegResult == NULL)
    {
        ABU_WORDS_SEG_DL_destroySegResult(pSegResult);
        return NULL;
    }

	return pSegResult;
}

/**
@name:ABU_WORDS_SEG_DL_destroySegResult(ABU_WORDS_SEGRESULT_DL_S *pSegResult)
@desc: free the content of pSegResult and free pSegResult
@return:  succeed return 0  ;fail    return -1
*/
void ABU_WORDS_SEG_DL_destroySegResult(ABU_WORDS_SEGRESULT_DL_S *pSegResult)
{
	ABU_ASSERT(pSegResult != NULL);
	
	/*step1:free*/
	ABU_WORDS_SEG_SL_destroySegResult(pSegResult->pBasicSegResult);

	/*step2:free groupWord and free pBasicWordArray*/
	ABU_WORDS_SEG_SL_destroySegResult(pSegResult->pGroupSegResult);

	ABU_FREE(pSegResult);
	
}
void ABU_WORDS_SEG_DL_outputFromFile(const char **szDictFile,const UINT32 dwSmallDictNumber,
									 const UINT32 dwBigDictNumber,const char *szInputFile,  
									 const char * szBasicOutputFile,const char *szGroupOutputFile)
{
	ABU_ASSERT(szDictFile != NULL && szInputFile != NULL && szBasicOutputFile != NULL && szGroupOutputFile != NULL);
	ABU_ASSERT(dwSmallDictNumber >= 1 && dwBigDictNumber >= 1);

	char szBuff[MAX_LINE_LENGTH_4WORDS + 1];
	UINT32 lineNumber = 0;
	int tempLen = 0;
	ABU_WORDS_DICT_S *pBasicDict = NULL;
	ABU_WORDS_DICT_S *pGroupDict = NULL;

	
	ABU_WORDS_SEGLIB_S *pSegLib = ABU_WORDS_SEG_createSegLib();
	if(pSegLib == NULL) return;

	/*open dict*/
	if( (pBasicDict = ABU_WORDS_DICT_loadDictFromFile(szDictFile,dwSmallDictNumber)) == NULL)
	{
		fprintf(stderr,"fail to load dict\n");
		return;
	}
    if( (pGroupDict = ABU_WORDS_DICT_loadDictFromFile(szDictFile,dwBigDictNumber)) == NULL)
	{
		fprintf(stderr,"fail to load group dict\n");
	}

	FILE *fpInput = fopen(szInputFile,"r");
	FILE *fpBasicOutput = fopen(szBasicOutputFile,"w+");
	FILE *fpGroupOutput = fopen(szGroupOutputFile,"w+");
	if( fpInput == NULL || fpBasicOutput == NULL || fpGroupOutput == NULL)
	{
		if(fpInput == NULL)
			printf("can't open %s\n",szInputFile);
		if(fpBasicOutput == NULL)
			printf("can't open %s\n",szBasicOutputFile);
		if(fpGroupOutput == NULL)
			printf("can't open %s\n",szGroupOutputFile);
        ABU_WORDS_DICT_destroyDict(pBasicDict);

        ABU_WORDS_DICT_destroyDict(pGroupDict);

        ABU_WORDS_SEG_destroySegLib(pSegLib);
        
        if( fclose(fpInput) != 0)
            printf("can't close %s\n",szInputFile);
        if( fclose(fpBasicOutput) != 0)
            printf("can't close %s\n",szBasicOutputFile);
        if( fclose(fpGroupOutput) != 0)
            printf("can't close %s\n",szBasicOutputFile);
        return;    
	}
	/*segmentatin process*/
	printf("%s",getCurrTime());
	while( !feof(fpInput))
	{
		memset(szBuff,0,MAX_LINE_LENGTH_4WORDS);
		fgets(szBuff,MAX_LINE_LENGTH_4WORDS,fpInput);
		tempLen = strlen(szBuff);
		if(tempLen == 0)
			continue;
		if( szBuff[tempLen - 1] == '\n')
			szBuff[tempLen - 1] = '\0';
		ABU_WORDS_PREPROCESS_all(szBuff);
		tempLen = strlen(szBuff);
		if(tempLen == 0)
			continue;
		ABU_WORDS_SEGRESULT_DL_S *pSegResult = ABU_WORDS_SEG_DL_getSegResultFromBuff(szBuff,tempLen,pBasicDict,pGroupDict,pSegLib);
		if(pSegResult == NULL) continue;
		
		ABU_WORDS_SEG_SL_printSegResult(pSegResult->pBasicSegResult,fpBasicOutput);
//		char * szBasicSegResult = ABU_WORDS_SEG_SL_getSegResult(pSegResult->pBasicSegResult);
//		fputs(szBasicSegResult,fpBasicOutput);
//		fputs("\n",fpBasicOutput);
//		fflush(fpBasicOutput);
//		ABU_FREE(szBasicSegResult);

		ABU_WORDS_SEG_SL_printSegResult(pSegResult->pGroupSegResult,fpGroupOutput);
//		char *szGroupSegResult = ABU_WORDS_SEG_SL_getSegResult(pSegResult->pGroupSegResult);
//		fputs(szGroupSegResult,fpGroupOutput);
//		fputs("\n",fpGroupOutput);
//		fflush(fpGroupOutput);
//		ABU_FREE(szGroupSegResult);
			
		ABU_WORDS_SEG_DL_destroySegResult(pSegResult);
		
		lineNumber++;
		if(lineNumber % 10000 == 0)
			fprintf(stderr,".");
	}
	printf("%d lines has been processed\n",lineNumber);
	printf("%s",getCurrTime());

	ABU_WORDS_DICT_destroyDict(pBasicDict);

	ABU_WORDS_DICT_destroyDict(pGroupDict);

	ABU_WORDS_SEG_destroySegLib(pSegLib);
	
	if( fclose(fpInput) != 0)
		printf("can't close %s\n",szInputFile);
	if( fclose(fpBasicOutput) != 0)
		printf("can't close %s\n",szBasicOutputFile);
	if( fclose(fpGroupOutput) != 0)
		printf("can't close %s\n",szBasicOutputFile);	

}



void ABU_WORDS_SEG_DL_printSegResult(const ABU_WORDS_SEGRESULT_DL_S *pSegResult,FILE * fp)
{
	ABU_ASSERT(pSegResult != NULL && pSegResult->pBasicSegResult != NULL && pSegResult->pGroupSegResult != NULL);
	ABU_ASSERT(fp != NULL);
	
	ABU_WORDS_WORD_S *currWord = NULL;
	int i = 0;
	fprintf(fp,"BasicSegResult:\n");
	fprintf(fp,"pBasicSegResult->dwWordNumber = %d\n",pSegResult->pBasicSegResult->dwWordNumber);
	fprintf(fp,"pBasicSegResult->dwSizeOfWordArray = %d\n",pSegResult->pBasicSegResult->dwSizeOfWordArray);
	fprintf(fp,"Word----------WordPosTag----------Offset---------isNextNeSepa\n");
	for ( i = 0 ;i < pSegResult->pBasicSegResult->dwWordNumber; i++)
	{
		currWord = pSegResult->pBasicSegResult->pWordArray + i;
		fprintf(fp,"%s----------%s----------%d-------%d\n",currWord->szWord,currWord->szWordPosTag,currWord->dwOffset,currWord->isNextNeSepa);
	}
	fprintf(fp,"GroupSegResult:\n");
	fprintf(fp,"pGroupSegResult->dwWordNumber = %d\n",pSegResult->pGroupSegResult->dwWordNumber);
	fprintf(fp,"pGroupSegResult->dwSizeOfWordArray = %d\n",pSegResult->pGroupSegResult->dwSizeOfWordArray);
	fprintf(fp,"Word----------WordPosTag----------Offset------isNextNeSepa\n");
	for ( i = 0 ;i < pSegResult->pGroupSegResult->dwWordNumber; i++)
	{
		currWord = pSegResult->pGroupSegResult->pWordArray + i;
		fprintf(fp,"%s----------%s----------%d----------%d\n",currWord->szWord,currWord->szWordPosTag,currWord->dwOffset,currWord->isNextNeSepa);
	}
	
}
