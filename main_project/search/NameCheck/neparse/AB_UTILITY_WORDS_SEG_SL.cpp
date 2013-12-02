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
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include "HASHSET.h"
#include "STRING_UTILITY.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/****************************************************************
****private interface declaration*******************************
****************************************************************/
//UINT32 ABU_WORDS_SEG_getMaxMatchLength(const ABU_WORDS_DICT_S *pDict,
//									   const char *szBuff,  UINT32 dwHasCovered,
//									   char *szWordPosTag,const UINT32 dwWordPosTagLength);
int ABU_WORDS_SEG_SL_outputFromBuff_OtherLogic(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szBuff,
											char *szNumberWord,char *szLetterWord,
											UINT32 *pdwBasicHasCovered,const UINT32 dwLenSzBuff,const ABU_WORDS_SEGLIB_S *pSegLib);
int ABU_WORDS_SEG_SL_outputFromBuff_ChinLogic(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szBuff,
											  const ABU_WORDS_DICT_S *pDict,UINT32 *pdwHasCovered,const UINT32 dwLenSzBuff,const ABU_WORDS_SEGLIB_S *pSegLib);
/**
@desc:常用的中文命名实体分隔符：，。！；？
	        英文命名实体分隔符:,.!;?
*/
BOOL isNextNeSepa(const char *szBuff,const UINT32 dwHasCovered,const UINT32 dwLenSzBuff,const ABU_WORDS_SEGLIB_S *pSegLib);

/**
*@name: ABU_WORDS_SEG_SL_createSegResult
*@desc: 生成ABU_WORDS_SEGRESULT_SL_S结构体
*       dwInitSize > 0
*/
ABU_WORDS_SEGRESULT_SL_S * ABU_WORDS_SEG_SL_createSegResult(UINT32 dwInitSize);

int ABU_WORDS_SEG_SL_changeSegResult(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szWord,
									 const char *szWordPosTag,const UINT32 dwOffset,BOOL isNextNeSepa);

HashSet *getSeparateSet();
HashSet *getPunctSet();

/*****************************************************************
***public interface implementation********************************
******************************************************************/


char * ABU_WORDS_SEG_SL_getSegResult(const ABU_WORDS_SEGRESULT_SL_S *pSegResult)
{
	ABU_ASSERT(pSegResult != NULL);
	
	UINT32 i = 0;	
	UINT32 dwCurrSizeOfSegResult = MAX_LINE_LENGTH_4WORDS + 1;
	UINT32 dwSegResultLength = 0;
      UINT32 dwTmpLength = 0;

	char *szSegResult = (char *)calloc(MAX_LINE_LENGTH_4WORDS + 1,sizeof(char));
	if(  szSegResult == NULL) { return NULL;}

	for( i = 0 ;i < pSegResult->dwWordNumber ; i++)
	{
		dwTmpLength = strlen((pSegResult->pWordArray + i)->szWord);
		
       dwSegResultLength += (dwTmpLength + 1);//add space
	    if(dwSegResultLength > dwCurrSizeOfSegResult)//realloc memory
		{
			szSegResult =(char *)realloc(szSegResult,2 * dwCurrSizeOfSegResult * sizeof(char));
			if(szSegResult == NULL) return NULL;
			dwCurrSizeOfSegResult = 2 * dwCurrSizeOfSegResult;
		}
		strncat(szSegResult,(pSegResult->pWordArray + i)->szWord,dwTmpLength);
		strncat(szSegResult," ",OTHER_CHARACTER_LENGTH);
	}
	return szSegResult;
}

/**
@desc: create segmentation lib
*/
ABU_WORDS_SEGLIB_S * ABU_WORDS_SEG_createSegLib()
{
	ABU_WORDS_SEGLIB_S *pSegLib = (ABU_WORDS_SEGLIB_S *)calloc(1,sizeof(ABU_WORDS_SEGLIB_S));
	if(pSegLib == NULL) return NULL;

	pSegLib->pSeparateSet = getSeparateSet();
	if(pSegLib->pSeparateSet == NULL)
    {
        ABU_WORDS_SEG_destroySegLib(pSegLib);
        return NULL;
    }
	
	pSegLib->pPunctSet = getPunctSet();
	if(pSegLib->pPunctSet == NULL)
    {
        ABU_WORDS_SEG_destroySegLib(pSegLib);
        return NULL;
    }

	return pSegLib;
}


/**
@desc:free memory:destroy segmentation lib
*/
int ABU_WORDS_SEG_destroySegLib(ABU_WORDS_SEGLIB_S *pSegLib)
{
	ABU_ASSERT(pSegLib != NULL);
	
	if(pSegLib->pSeparateSet != NULL && destroyHashSet(pSegLib->pSeparateSet) == -1)
		return -1;
	
	if(pSegLib->pPunctSet != NULL && destroyHashSet(pSegLib->pPunctSet) == -1)
		return -1;

	ABU_FREE(pSegLib);

	return 0;
}


ABU_WORDS_SEGRESULT_SL_S * ABU_WORDS_SEG_SL_getSegResultFromBuff(const char * sBuff,const UINT64 ulLenSBuff,const ABU_WORDS_DICT_S *pDict,const ABU_WORDS_SEGLIB_S *pSegLib)

{
	ABU_ASSERT(sBuff != NULL && pDict != NULL && pSegLib != NULL);
	
	UINT32 dwHasCovered = 0;
	UINT64 lenSzBuff = 0;
	char szLetterWord[MAX_WORD_LENGTH];/*英语单词 || 标识符 */
	char szNumberWord[MAX_WORD_LENGTH];/*数字*/
	ABU_WORDS_SEGRESULT_SL_S *pSegResult = NULL;
	BOOL bIsNextNeSepa = FALSE;

	//pre check
	if(ulLenSBuff == 0) return NULL;
	
	//make a copy
	//the accidental case
      char *szCopyBuff = (char *)calloc(ulLenSBuff + 1,sizeof(char));
      if(szCopyBuff == NULL) return NULL;

	UINT32 i = 0;
	for( i = 0;i < ulLenSBuff;)
	{
		//no Chinese
		if(sBuff[ i ] >  0) 
		{
			szCopyBuff[ dwHasCovered++] = sBuff[i++];
		}
		if(sBuff[ i ] == 0 ) break;
		//Chinese character
		if(sBuff[i] < 0 )
		{
			if((dwHasCovered + 2) <= ulLenSBuff)
			{
				szCopyBuff[dwHasCovered++]  = sBuff[i++];
				szCopyBuff[dwHasCovered++]  = sBuff[i++];
			}
			else
			{
				break;
			}
		}  
	}
	
	if(dwHasCovered == 0) 
    {
        ABU_FREE(szCopyBuff);
        ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
        return NULL;
    }
	
	//sBuff is end with \0! need preprocess
	if(ABU_WORDS_PREPROCESS_all(szCopyBuff) != 0)
    {
        //fprintf(stderr, "ABU_WORDS_PREPROCESS_all != 0");
        //return NULL;
        ABU_FREE(szCopyBuff);
        ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
        return NULL;
    }

	lenSzBuff = strlen(szCopyBuff);

	
	memset(szLetterWord,0,MAX_WORD_LENGTH);
	memset(szNumberWord,0,MAX_WORD_LENGTH);
    
	pSegResult = (ABU_WORDS_SEGRESULT_SL_S *)ABU_WORDS_SEG_SL_createSegResult(WEB_PAGE_WORD_NUMBER + 1);
	if(pSegResult == NULL)
    {
        ABU_FREE(szCopyBuff);
        ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
        return NULL;
    }
	
	dwHasCovered = 0;

	for(; dwHasCovered < lenSzBuff ; )
	{
		/*step1: no chinese segmentation logic*/
		if(szCopyBuff[dwHasCovered] > 0)
		{	
			if( ABU_WORDS_SEG_SL_outputFromBuff_OtherLogic
				(pSegResult,szCopyBuff,szNumberWord,szLetterWord,&dwHasCovered,lenSzBuff,pSegLib) == -1)
			{
                ABU_FREE(szCopyBuff);
                ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
                return NULL;
			}
            if ((strlen(szLetterWord) < MAX_WORD_LENGTH - 1) && \
                (strlen(szNumberWord) < MAX_WORD_LENGTH - 1))
			    continue;
		}

		/*step2:logci before chinese segmentation*/
		bIsNextNeSepa = isNextNeSepa(szCopyBuff,dwHasCovered,lenSzBuff,pSegLib);

		if( szLetterWord[0] != STRING_END)
		{
			ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szLetterWord,POS_ENG,dwHasCovered - strlen(szLetterWord),bIsNextNeSepa);	
			memset(szLetterWord,0,MAX_WORD_LENGTH);
		}
		if( szNumberWord[0] != STRING_END)
		{
			ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szNumberWord,POS_ABA,dwHasCovered - strlen(szNumberWord),bIsNextNeSepa);
			memset(szNumberWord,0,MAX_WORD_LENGTH);
		}
        if(szCopyBuff[dwHasCovered] > 0)//still no Chinese
            continue;        

		/*step3:chinese segmentation logic*/
		if( ABU_WORDS_SEG_SL_outputFromBuff_ChinLogic(pSegResult,szCopyBuff,pDict,&dwHasCovered,lenSzBuff,pSegLib) == -1)
        {
            ABU_FREE(szCopyBuff);
            ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
            return NULL;
        }
					
	}

	///*Note：！！！中文分词之后将已有 szNumberWord || szLetterWord 处理*/
	if( szLetterWord[0] != STRING_END)
	{
		ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szLetterWord,POS_ENG,dwHasCovered - strlen(szLetterWord),FALSE);
		memset(szLetterWord,0,MAX_WORD_LENGTH);

	}
	if( szNumberWord[0] != STRING_END)
	{
		ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szNumberWord,POS_ABA,dwHasCovered - strlen(szNumberWord),FALSE);
		memset(szNumberWord,0,MAX_WORD_LENGTH);
	}
	
	//free memory
	ABU_FREE(szCopyBuff);
	
	return pSegResult;
	
}



void ABU_WORDS_SEG_SL_destroySegResult(ABU_WORDS_SEGRESULT_SL_S *pSegResult)
{
	if(pSegResult == NULL) return;
	
	UINT32 i = 0;
	ABU_WORDS_WORD_S *currWord;
	
	
	/*step1:free BasicWord and free pBasicWordArray*/
	for (i = 0;i < pSegResult->dwWordNumber; i++)
	{
		currWord = pSegResult->pWordArray + i;
		ABU_FREE(currWord->szWord);
		ABU_FREE(currWord->szWordPosTag);
	}
	
	ABU_FREE(pSegResult->pWordArray);
	
	ABU_FREE(pSegResult);
	
}
void ABU_WORDS_SEG_SL_outputFromFile(const char **szDictFile,const UINT32 dwDictNumber,const char *szInputFile,
								  const char * szOutputFile)
{
	ABU_ASSERT(szDictFile != NULL && szInputFile != NULL && szOutputFile != NULL);
	
	FILE *fpInput,*fpOutput;
	char szBuff[MAX_LINE_LENGTH_4WORDS + 1];
	ABU_WORDS_SEGRESULT_SL_S *pSegResult; 
	UINT32 lineNumber = 0;
	int tempLen = 0;
	ABU_WORDS_DICT_S * pDict = NULL;
	ABU_WORDS_SEGLIB_S *pSegLib = NULL;

	char *szSegResult = NULL;

	

	pDict = (ABU_WORDS_DICT_S *)calloc(1,sizeof(ABU_WORDS_DICT_S));
	if(pDict == NULL) return;

	pSegLib = ABU_WORDS_SEG_createSegLib();
	if(pSegLib == NULL) return;



	/*open dict*/
	if( (pDict = ABU_WORDS_DICT_loadDictFromFile(szDictFile,dwDictNumber)) == NULL)
	{
		fprintf(stderr,"fail to load dict\n");
		return;
	}

	fpInput = fopen(szInputFile,"r");
	fpOutput = fopen(szOutputFile,"w+");
	if( fpInput == NULL || fpOutput == NULL)
	{
		if(fpInput == NULL)
			fprintf(stderr,"can't open %s\n",szInputFile);
		if(fpOutput == NULL)
			fprintf(stderr,"can't open %s\n",szOutputFile);
        ABU_WORDS_DICT_destroyDict(pDict);
        ABU_WORDS_SEG_destroySegLib(pSegLib);

        if( fclose(fpInput) != 0)
            fprintf(stderr,"can't close %s\n",szInputFile);
        if( fclose(fpOutput) != 0)
            fprintf(stderr,"can't close %s\n",szOutputFile);
        return;
	}
	/*segmentatin process*/
	while( !feof(fpInput))
	{
		memset(szBuff,0,MAX_LINE_LENGTH_4WORDS);
		fgets(szBuff,MAX_LINE_LENGTH_4WORDS,fpInput);
		tempLen = strlen(szBuff);

		if( szBuff[tempLen - 1] == STRING_NEW_LINE)
			szBuff[tempLen - 1] = STRING_END;
		ABU_WORDS_PREPROCESS_all(szBuff);

		tempLen = strlen(szBuff);
		if(tempLen == 0)  continue;
		
		pSegResult = (ABU_WORDS_SEGRESULT_SL_S *)ABU_WORDS_SEG_SL_getSegResultFromBuff(szBuff,tempLen,pDict,pSegLib);

		if(pSegResult != NULL)
		{
			//ABU_WORDS_SEG_SL_displaySegResult(pSegResult);
			szSegResult = ABU_WORDS_SEG_SL_getSegResult(pSegResult);
			strncat(szSegResult,"\n",OTHER_CHARACTER_LENGTH);
			fputs(szSegResult,fpOutput);

			ABU_FREE(szSegResult);
	
			ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
		}
		else
		{
			ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
            ABU_WORDS_DICT_destroyDict(pDict);
            ABU_WORDS_SEG_destroySegLib(pSegLib);

            if( fclose(fpInput) != 0)
                fprintf(stderr,"can't close %s\n",szInputFile);
            if( fclose(fpOutput) != 0)
                fprintf(stderr,"can't close %s\n",szOutputFile);
		}
		lineNumber++;
		if(lineNumber % 10000 == 0)
			printf(".");
	}
	printf("%d lines has been processed\n",lineNumber);
	ABU_WORDS_DICT_destroyDict(pDict);
	ABU_WORDS_SEG_destroySegLib(pSegLib);

	if( fclose(fpInput) != 0)
		fprintf(stderr,"can't close %s\n",szInputFile);
	if( fclose(fpOutput) != 0)
		fprintf(stderr,"can't close %s\n",szOutputFile);
}

void ABU_WORDS_SEG_SL_printSegResult(const ABU_WORDS_SEGRESULT_SL_S *pSegResult,FILE *fp)
{
	ABU_ASSERT(pSegResult != NULL);
	
	fprintf(fp,"SegResult:\n");
	fprintf(fp,"dwWordNumber = %d\n",pSegResult->dwWordNumber);
	fprintf(fp,"dwSizeOfWordArray = %d\n",pSegResult->dwSizeOfWordArray);
	fprintf(fp,"Word--WordPosTag--Offset--isNextNeSepa\n");
	
	int i =0;
	for ( i = 0 ;i < pSegResult->dwWordNumber; i++)
	{
		ABU_WORDS_WORD_S *currWord = pSegResult->pWordArray + i;
		fprintf(fp,"%s--%s--%d--%d\n",currWord->szWord,currWord->szWordPosTag,currWord->dwOffset,currWord->isNextNeSepa);	
	}
}

/*****************************************************************
***private interface implementation********************************
******************************************************************/


/**
@name:ABU_WORDS_SEG_SL_outputFromBuff_OtherLogic
@desc:
此函数主要用于ABU_WORDS_SEG_SL_outputFromBuff,用于处理非中文的逻辑
例如英文,数字,标点符号等
@param:
@return:
	succeed return 0
	fail    return -1
*/
int ABU_WORDS_SEG_SL_outputFromBuff_OtherLogic(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szBuff,
											char *szNumberWord,char *szLetterWord,
											UINT32 *pdwHasCovered,const UINT32 dwLenSzBuff,const ABU_WORDS_SEGLIB_S *pSegLib)
{
	            ABU_ASSERT(pSegResult != NULL && szBuff != NULL && szNumberWord != NULL && szLetterWord != NULL &&
				pdwHasCovered != NULL && pSegLib != NULL);
				
			char word[MAX_WORD_LENGTH + 1];
			BOOL bIsNextNeSepa = 0;
			ABU_ASSERT(pSegResult != NULL && szBuff != NULL && 
				  szNumberWord != NULL && szLetterWord != NULL && pdwHasCovered != NULL);

			if( STRU_isSpace(szBuff[*pdwHasCovered]) == 0)
			{
				if( szNumberWord[0] != STRING_END )
				{
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szNumberWord,POS_ABA,
						                       *pdwHasCovered - strlen(szNumberWord),TRUE);
					memset(szNumberWord,0,MAX_WORD_LENGTH);	
				}

				if( szLetterWord[0] != STRING_END)
				{
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szLetterWord,POS_ENG,
												  *pdwHasCovered - strlen(szLetterWord),TRUE);
					memset(szLetterWord,0,MAX_WORD_LENGTH);
				}	

				*pdwHasCovered += OTHER_CHARACTER_LENGTH;
				return 0;
			}
			
			if( STRU_isLetter(szBuff[*pdwHasCovered]) == 0)/*letter*/
			{
				
				if( szNumberWord[0] != STRING_END)
				{	
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szNumberWord,POS_ABA,
								*pdwHasCovered - strlen(szNumberWord),FALSE);
					memset(szNumberWord,0,MAX_WORD_LENGTH);	
				}
				strncat(szLetterWord,szBuff + *pdwHasCovered,OTHER_CHARACTER_LENGTH);
				*pdwHasCovered += OTHER_CHARACTER_LENGTH;
				return 0;
			}
			if( STRU_isNumber(szBuff[*pdwHasCovered]) == 0 )/*number*/
			{
				
				if( szLetterWord[0] != STRING_END)
				{					
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szLetterWord,POS_ENG,
							*pdwHasCovered - strlen(szLetterWord),FALSE);
					memset(szLetterWord,0,MAX_WORD_LENGTH);
				}
				strncat(szNumberWord,szBuff + *pdwHasCovered,OTHER_CHARACTER_LENGTH);
				*pdwHasCovered += OTHER_CHARACTER_LENGTH;
				return 0;
			}
			 if ( STRU_isHyphen(szBuff[*pdwHasCovered]) == 0 )/*连字符的处理逻辑*/
			{
				if( szNumberWord[0] != STRING_END)
				{
					//连字符之后下一个字符依然是数字
					if( *(szBuff + *pdwHasCovered + OTHER_CHARACTER_LENGTH) >= '0' &&
						*(szBuff + *pdwHasCovered + OTHER_CHARACTER_LENGTH) <= '9')
					{
						strncat(szNumberWord,szBuff + *pdwHasCovered,OTHER_CHARACTER_LENGTH);
					}
					else
					{
                      //输出前面的数字	
						 ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szNumberWord,POS_ABA,*pdwHasCovered - strlen(szNumberWord),FALSE);
						 memset(szNumberWord,0,MAX_WORD_LENGTH);

						 //输出此连字符
						 bIsNextNeSepa = isNextNeSepa(szBuff,*pdwHasCovered + OTHER_CHARACTER_LENGTH,dwLenSzBuff,pSegLib);
						 memset(word,0,MAX_WORD_LENGTH + 1);
						 strncpy(word,szBuff + *pdwHasCovered,OTHER_CHARACTER_LENGTH);
						 ABU_WORDS_SEG_SL_changeSegResult(pSegResult,word,POS_W,*pdwHasCovered,bIsNextNeSepa);
					}
					*pdwHasCovered += OTHER_CHARACTER_LENGTH;
					return 0;
				}
				if( szLetterWord[0] != STRING_END)
				{
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szLetterWord,POS_ENG,
												  *pdwHasCovered - strlen(szLetterWord),FALSE);
					memset(szLetterWord,0,MAX_WORD_LENGTH);
						
					memset(word,0,MAX_WORD_LENGTH + 1);
					strncpy(word,szBuff + *pdwHasCovered,OTHER_CHARACTER_LENGTH);

					//此处标点为英文标点
					
			       bIsNextNeSepa = isNextNeSepa(szBuff,*pdwHasCovered + OTHER_CHARACTER_LENGTH,dwLenSzBuff,pSegLib);

					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,word,POS_W,*pdwHasCovered,bIsNextNeSepa);
					*pdwHasCovered += OTHER_CHARACTER_LENGTH;
					return 0;
				}								
			}
			if ( STRU_isPunctuation(szBuff[*pdwHasCovered]) == 0 )/*除连字符之外的英文标点符号逻辑*/
			{
				bIsNextNeSepa = isNextNeSepa(szBuff,*pdwHasCovered,dwLenSzBuff,pSegLib);//判定此标点是否为NE分隔符

				if( szLetterWord[0] != STRING_END)
				{
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szLetterWord,POS_ENG,
													*pdwHasCovered - strlen(szLetterWord),bIsNextNeSepa);
					memset(szLetterWord,0,MAX_WORD_LENGTH);
				}

				if( szNumberWord[0] != STRING_END)
				{
					ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szNumberWord,POS_ABA,
													*pdwHasCovered - strlen(szNumberWord),bIsNextNeSepa);
					memset(szNumberWord,0,MAX_WORD_LENGTH);
				}

				memset(word,0,MAX_WORD_LENGTH + 1);
				strncpy(word,szBuff + *pdwHasCovered,OTHER_CHARACTER_LENGTH);
				//判断此标点符号下一步是否为分隔符号
				bIsNextNeSepa = isNextNeSepa(szBuff,*pdwHasCovered + OTHER_CHARACTER_LENGTH ,dwLenSzBuff,pSegLib);
				
				ABU_WORDS_SEG_SL_changeSegResult(pSegResult,word,POS_W,*pdwHasCovered,bIsNextNeSepa);

				*pdwHasCovered += OTHER_CHARACTER_LENGTH;
				return 0;
			}
			
		return -1;
}
/**
@name:ABU_WORDS_SEG_SL_outputFromBuff_ChinLogic
@desc:此函数用于ABU_WORDS_SEG_SL_outputFromBuff,处理中文的逻辑
@param:
@return:
	succeed return 0
	fail    return -1
*/
int ABU_WORDS_SEG_SL_outputFromBuff_ChinLogic(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szBuff,
											  const ABU_WORDS_DICT_S *pDict,UINT32 *pdwHasCovered,const UINT32 dwLenSzBuff,const ABU_WORDS_SEGLIB_S *pSegLib)
{
	 	 ABU_ASSERT(pSegResult != NULL && szBuff != NULL && pDict != NULL && pdwHasCovered != NULL);
	  
	   	char szWordPosTag[MAX_WORD_LENGTH + 1];
		char szWord[MAX_WORD_LENGTH + 1];	
	  	 BOOL bIsNextNeSepa = FALSE;
		UINT32 dwMaxMatch = 0;
		memset(szWordPosTag,0,MAX_WORD_LENGTH + 1);
		
		dwMaxMatch = ABU_WORDS_SEG_getMaxMatchLength(pDict,szBuff,*pdwHasCovered,szWordPosTag,MAX_WORD_LENGTH + 1);
		

		/*目前版本的词库中加入了中文标点,dwMaxMatch 可以为2，此时szWordPosTag 为POS_W*/
		/*单个汉子，或者是一些在词库中不存在的标点符合集合*/
		if(dwMaxMatch == 0)
		{
			memset(szWord,0,MAX_WORD_LENGTH + 1);
			strncpy(szWord,szBuff + *pdwHasCovered,CHINESE_CHARACTER_LENGTH);

			//isNextNeSepa
			bIsNextNeSepa = isNextNeSepa(szBuff,*pdwHasCovered + CHINESE_CHARACTER_LENGTH,dwLenSzBuff,pSegLib);

//			if(searchHashSetNode(pPunct,szWord) == 0)
//			{
//				//chinese punct
//				ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szWord,POS_W,*pdwHasCovered,bIsNextNeSepa);
//			}
//			else
//			{
		//		ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szWord,POS_UN,*pdwHasCovered,bIsNextNeSepa);
		//	}
		
			ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szWord,POS_UN,*pdwHasCovered,bIsNextNeSepa);
			*pdwHasCovered += CHINESE_CHARACTER_LENGTH;
			return 0;			
		}
		else
		{
			memset(szWord,0,MAX_WORD_LENGTH + 1);
			strncpy(szWord,szBuff + *pdwHasCovered,dwMaxMatch);
           
			//isNextNeSepa
			bIsNextNeSepa = isNextNeSepa(szBuff,*pdwHasCovered + dwMaxMatch,dwLenSzBuff,pSegLib);

			ABU_WORDS_SEG_SL_changeSegResult(pSegResult,szWord,szWordPosTag,*pdwHasCovered,bIsNextNeSepa);
			
			*pdwHasCovered += dwMaxMatch;
			return 0;
		}
		
}

ABU_WORDS_SEGRESULT_SL_S * ABU_WORDS_SEG_SL_createSegResult(UINT32 dwInitSize)
{
	ABU_ASSERT(dwInitSize != 0);
	
	ABU_WORDS_SEGRESULT_SL_S *pSegResult = (ABU_WORDS_SEGRESULT_SL_S *)calloc(1, sizeof(ABU_WORDS_SEGRESULT_SL_S) );
	if(pSegResult == NULL) return NULL;
	
	pSegResult->pWordArray = (ABU_WORDS_WORD_S *)calloc(dwInitSize,sizeof(ABU_WORDS_WORD_S));
	if(pSegResult->pWordArray == NULL) 
    {
        ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
        return NULL;
    }

	pSegResult->dwSizeOfWordArray = dwInitSize;

	pSegResult->dwWordNumber= 0;

	return pSegResult;
}
/**
@name:ABU_WORDS_SEG_SL_changeSegResult(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szWord,const char *szWordPosTag)
@desc:change SegmentResult structure
@param:
@return:
    succeed return 0
	fail    return -1;
*/
int ABU_WORDS_SEG_SL_changeSegResult(ABU_WORDS_SEGRESULT_SL_S *pSegResult,const char *szWord,
									 const char *szWordPosTag,const UINT32 dwOffset,BOOL isNextNeSepa)
{
	ABU_ASSERT(pSegResult != NULL && szWord != NULL && szWordPosTag != NULL);
	
	ABU_WORDS_WORD_S *currWord = NULL;
	UINT32 dwTmpLen = 0;

	if( ( pSegResult->dwWordNumber + 1) > pSegResult->dwSizeOfWordArray)
	{
		//realloc memory:容量扩大两倍
		pSegResult->pWordArray = (ABU_WORDS_WORD_S *)realloc(pSegResult->pWordArray,
										2 * pSegResult->dwSizeOfWordArray * sizeof(ABU_WORDS_WORD_S));
										
	   if(pSegResult->pWordArray == NULL) return -1;
	   
	   pSegResult->dwSizeOfWordArray = 2 * pSegResult->dwSizeOfWordArray;	
	}
	currWord = pSegResult->pWordArray + pSegResult->dwWordNumber;
	
	/*step1: szWord*/
	dwTmpLen = strlen(szWord);
	currWord->szWord = (char *)calloc(1,dwTmpLen + 1);
	if(currWord->szWord == NULL) 
    {
        ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
        return -1;
    }
	strncpy(currWord->szWord,szWord,dwTmpLen);
   
	/*step2:szWordPosTag*/
	dwTmpLen = strlen(szWordPosTag);
	currWord->szWordPosTag = (char *)calloc(1,dwTmpLen + 1);
	if(currWord->szWordPosTag == NULL) 
    {
        ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
        return -1;
    }
	strncpy(currWord->szWordPosTag,szWordPosTag,dwTmpLen);

	/*step3:dwOffset*/
	currWord->dwOffset = dwOffset;

	/*step4:isNextNeSepa*/

	currWord->isNextNeSepa = isNextNeSepa;

	pSegResult->dwWordNumber += 1;

	return 0;
}


/**
@desc:常用的中文命名实体分隔符：，。！；？
	        英文命名实体分隔符:,.!;?
			空格
*/
BOOL isNextNeSepa(const char *szBuff,const UINT32 dwHasCovered,const UINT32 dwLenSzBuff,const ABU_WORDS_SEGLIB_S *pSegLib)
{
	ABU_ASSERT(szBuff != NULL && pSegLib != NULL);
	
	char cpSepa[CHINESE_CHARACTER_LENGTH + 1];
	HashSet *pSeparateSet = pSegLib->pSeparateSet;

	ABU_ASSERT(szBuff != NULL && pSegLib != NULL);
	if(dwHasCovered == dwLenSzBuff) return TRUE;

	memset(cpSepa,0,CHINESE_CHARACTER_LENGTH + 1);
	if(szBuff[dwHasCovered] > 0)
	{	
		strncpy(cpSepa,szBuff + dwHasCovered,OTHER_CHARACTER_LENGTH);
		return ( searchHashSetNode(pSeparateSet,cpSepa) == 0 )?TRUE:FALSE;
	}
	else if(szBuff[dwHasCovered] < 0)
	{
		strncpy(cpSepa,szBuff + dwHasCovered,CHINESE_CHARACTER_LENGTH);
		return ( searchHashSetNode(pSeparateSet,cpSepa) == 0)?TRUE:FALSE;
	}
	return FALSE;
}
/**
@name:ABU_WORDS_SEG_getMaxMatchLength
@desc: special case:连篇累牍
@param:
	 szWordPosTag: value-result
@return:
	succeed return 0
	fail    return -1
*/
UINT32 ABU_WORDS_SEG_getMaxMatchLength(const ABU_WORDS_DICT_S *pDict,
									   const char *szBuff,  UINT32 dwHasCovered,
									   char *szWordPosTag,const UINT32 dwWordPosTagLength)
{
	ABU_ASSERT( pDict != NULL && szBuff != NULL && szWordPosTag != NULL);
	
	UINT32 dwPreMaxMatchLength = 0;
	UINT32 dwCurrMaxMatchLength = 0;
	UINT32 dwMaxMatchLength = 0;
	TrieNodePtr currTrieNodePtr = NULL;
	TrieNodePtr preTrieNodePtr = NULL;
	HashNodePtr currHashNodePtr = NULL;	

	UINT32 dwCharacterLength = 0;

	ABU_ASSERT(pDict != NULL && szBuff != NULL && szWordPosTag != NULL);

	
	/*hash index layer == 1*/
	dwCharacterLength = GET_NEXT_COMP_LENGTH(szBuff[dwHasCovered]);
	currHashNodePtr = ABU_WORDS_DICT_getHashNodePtr(pDict,szBuff,dwHasCovered);

	if(currHashNodePtr == NULL)
		return 0;
	dwMaxMatchLength += dwCharacterLength;
	dwHasCovered += dwCharacterLength;
	if(currHashNodePtr->byIsWord == 1)
	{
		dwPreMaxMatchLength = dwMaxMatchLength;
		dwCurrMaxMatchLength = dwMaxMatchLength;
		memset(szWordPosTag,0,dwWordPosTagLength);
		strncpy(szWordPosTag,currHashNodePtr->szWordPosTag,strlen(currHashNodePtr->szWordPosTag));
	
	}
	//fprintf(stderr,"%s-",currHashNodePtr->szSingleCharacter);
	

	/*trie index layer == 2*/
	dwCharacterLength = GET_NEXT_COMP_LENGTH(szBuff[dwHasCovered]);
	currTrieNodePtr = ABU_WORDS_DICT_getNextTrieNodePtrByHashNodePtr(currHashNodePtr,szBuff,dwHasCovered);
	if(currTrieNodePtr == NULL )
	{
		return (currHashNodePtr->byIsWord == 1)?dwMaxMatchLength:0;
	}
	dwMaxMatchLength += dwCharacterLength;
	dwHasCovered += dwCharacterLength;
	if(currTrieNodePtr->byIsWord == 1)
	{
		dwPreMaxMatchLength = dwMaxMatchLength;
		dwCurrMaxMatchLength = dwMaxMatchLength;
		memset(szWordPosTag,0,dwWordPosTagLength);
		strncpy(szWordPosTag,currTrieNodePtr->szWordPosTag,strlen(currTrieNodePtr->szWordPosTag));
		
	}
	//fprintf(stderr,"%s-",currTrieNodePtr->szSingleCharacter);
	
	//trie index layer >=3
	dwCurrMaxMatchLength = dwMaxMatchLength;
	
	currTrieNodePtr = ABU_WORDS_DICT_getNextTrieNodePtrByTrieNodePtr(currTrieNodePtr,szBuff,dwHasCovered);
	while(currTrieNodePtr != NULL)
	{
		//fprintf(stderr,"%s-",currTrieNodePtr->szSingleCharacter);
		
		dwCharacterLength = GET_NEXT_COMP_LENGTH(szBuff[dwHasCovered]);
		
		if(currTrieNodePtr->byIsWord == 1)
		{
			dwPreMaxMatchLength = dwCurrMaxMatchLength + dwCharacterLength;
			memset(szWordPosTag,0,dwWordPosTagLength);
			strncpy(szWordPosTag,currTrieNodePtr->szWordPosTag,strlen(currTrieNodePtr->szWordPosTag));	
		}
		dwCurrMaxMatchLength += dwCharacterLength;
		dwHasCovered += dwCharacterLength;
		preTrieNodePtr = currTrieNodePtr;
		currTrieNodePtr = ABU_WORDS_DICT_getNextTrieNodePtrByTrieNodePtr(currTrieNodePtr,szBuff,dwHasCovered);
	}

	return dwPreMaxMatchLength;
//	if(preTrieNodePtr != NULL && preTrieNodePtr->byIsWord == 1)
//	{
//		return dwCurrMaxMatchLength;
//	}
//	else
//	{
//		return dwPreMaxMatchLength;
//	}

}
/**
@desc: just dbc character
@desc: just create once
*/
HashSet *getSeparateSet()
{
	const UINT32 SEPARATE_SET_SIZE = 55;
	HashSet *pSeparateHashSet = createHashSet(SEPARATE_SET_SIZE);
	if(pSeparateHashSet == NULL) return NULL;
	
	//space
	insertHashSetNode(pSeparateHashSet," ");

	//punct
	insertHashSetNode(pSeparateHashSet,"－");
	insertHashSetNode(pSeparateHashSet,"-");
	insertHashSetNode(pSeparateHashSet,"！");
	insertHashSetNode(pSeparateHashSet,"!");
	insertHashSetNode(pSeparateHashSet,"\"");
	insertHashSetNode(pSeparateHashSet,"%");
	insertHashSetNode(pSeparateHashSet,"％");
	insertHashSetNode(pSeparateHashSet,",");
	insertHashSetNode(pSeparateHashSet,"，");
	insertHashSetNode(pSeparateHashSet,"、");
	insertHashSetNode(pSeparateHashSet,"．");
	insertHashSetNode(pSeparateHashSet,".");
	insertHashSetNode(pSeparateHashSet,"。");
	insertHashSetNode(pSeparateHashSet,":");
	insertHashSetNode(pSeparateHashSet,"：");
	insertHashSetNode(pSeparateHashSet,";");
	insertHashSetNode(pSeparateHashSet,"；");
	insertHashSetNode(pSeparateHashSet,"?");
	insertHashSetNode(pSeparateHashSet,"？");
	insertHashSetNode(pSeparateHashSet,"[");
	insertHashSetNode(pSeparateHashSet,"［");
	insertHashSetNode(pSeparateHashSet,"］");
	insertHashSetNode(pSeparateHashSet,"]");
	insertHashSetNode(pSeparateHashSet,"~");
	insertHashSetNode(pSeparateHashSet,"～");
	insertHashSetNode(pSeparateHashSet,"‘");	
	insertHashSetNode(pSeparateHashSet,"’");
	insertHashSetNode(pSeparateHashSet,"“");
	insertHashSetNode(pSeparateHashSet,"”");
	insertHashSetNode(pSeparateHashSet,"《");
	insertHashSetNode(pSeparateHashSet,"》");
	insertHashSetNode(pSeparateHashSet,"【");
	insertHashSetNode(pSeparateHashSet,"】");
	insertHashSetNode(pSeparateHashSet,"<");
	insertHashSetNode(pSeparateHashSet,"＜");
	insertHashSetNode(pSeparateHashSet,">");
	insertHashSetNode(pSeparateHashSet,"＞");
	insertHashSetNode(pSeparateHashSet,"·");
	insertHashSetNode(pSeparateHashSet,"·");
	insertHashSetNode(pSeparateHashSet,"…");
	insertHashSetNode(pSeparateHashSet,"……");
	insertHashSetNode(pSeparateHashSet,"℃");

	insertHashSetNode(pSeparateHashSet,"\\");
	insertHashSetNode(pSeparateHashSet,"+");
	insertHashSetNode(pSeparateHashSet,"-");
	insertHashSetNode(pSeparateHashSet,"*");
	insertHashSetNode(pSeparateHashSet,"/");
	insertHashSetNode(pSeparateHashSet,"{");
	insertHashSetNode(pSeparateHashSet,"}");
	insertHashSetNode(pSeparateHashSet,"@");
	insertHashSetNode(pSeparateHashSet,"#");
	insertHashSetNode(pSeparateHashSet,"$");
	insertHashSetNode(pSeparateHashSet,"￥");
	insertHashSetNode(pSeparateHashSet,"&");

	return pSeparateHashSet;
	
}

HashSet *getPunctSet()
{
	const UINT32 PUNCT_SIZE = 54;
	HashSet *pPunctHashSet = createHashSet(PUNCT_SIZE);
	if(pPunctHashSet == NULL) return NULL;

		//punct
	insertHashSetNode(pPunctHashSet,"－");
	insertHashSetNode(pPunctHashSet,"-");
	insertHashSetNode(pPunctHashSet,"！");
	insertHashSetNode(pPunctHashSet,"!");
	insertHashSetNode(pPunctHashSet,"\"");
	insertHashSetNode(pPunctHashSet,"%");
	insertHashSetNode(pPunctHashSet,"％");
	insertHashSetNode(pPunctHashSet,",");
	insertHashSetNode(pPunctHashSet,"，");
	insertHashSetNode(pPunctHashSet,"、");
	insertHashSetNode(pPunctHashSet,"．");
	insertHashSetNode(pPunctHashSet,".");
	insertHashSetNode(pPunctHashSet,"。");
	insertHashSetNode(pPunctHashSet,":");
	insertHashSetNode(pPunctHashSet,"：");
	insertHashSetNode(pPunctHashSet,";");
	insertHashSetNode(pPunctHashSet,"；");
	insertHashSetNode(pPunctHashSet,"?");
	insertHashSetNode(pPunctHashSet,"？");
	insertHashSetNode(pPunctHashSet,"[");
	insertHashSetNode(pPunctHashSet,"［");
	insertHashSetNode(pPunctHashSet,"］");
	insertHashSetNode(pPunctHashSet,"]");
	insertHashSetNode(pPunctHashSet,"~");
	insertHashSetNode(pPunctHashSet,"～");
	insertHashSetNode(pPunctHashSet,"‘");
	insertHashSetNode(pPunctHashSet,"’");
	insertHashSetNode(pPunctHashSet,"“");
	insertHashSetNode(pPunctHashSet,"”");
	insertHashSetNode(pPunctHashSet,"《");
	insertHashSetNode(pPunctHashSet,"》");
	insertHashSetNode(pPunctHashSet,"【");
	insertHashSetNode(pPunctHashSet,"】");
	insertHashSetNode(pPunctHashSet,"<");
	insertHashSetNode(pPunctHashSet,"＜");
	insertHashSetNode(pPunctHashSet,">");
	insertHashSetNode(pPunctHashSet,"＞");
	insertHashSetNode(pPunctHashSet,"·");
	insertHashSetNode(pPunctHashSet,"·");
	insertHashSetNode(pPunctHashSet,"…");
	insertHashSetNode(pPunctHashSet,"……");
	insertHashSetNode(pPunctHashSet,"℃");

	insertHashSetNode(pPunctHashSet,"\\");
	insertHashSetNode(pPunctHashSet,"+");
	insertHashSetNode(pPunctHashSet,"-");
	insertHashSetNode(pPunctHashSet,"*");
	insertHashSetNode(pPunctHashSet,"/");
	insertHashSetNode(pPunctHashSet,"{");
	insertHashSetNode(pPunctHashSet,"}");
	insertHashSetNode(pPunctHashSet,"(");
	insertHashSetNode(pPunctHashSet,")");
	insertHashSetNode(pPunctHashSet,"）");
	insertHashSetNode(pPunctHashSet,"（");
	insertHashSetNode(pPunctHashSet,"@");
	insertHashSetNode(pPunctHashSet,"#");
	insertHashSetNode(pPunctHashSet,"$");
	insertHashSetNode(pPunctHashSet,"￥");
	insertHashSetNode(pPunctHashSet,"&");

	return pPunctHashSet;
}

