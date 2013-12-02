/************************************************************
*@Filename: STRING_UTILITY.c
*@Author: bfzhang
*@Date:   2007-02-25
*@Version: 0.1
*@Description: collect the string utility funcitons in c language
*@Change Log
*  $Id: STRING_UTILITY.c,v 1.21 2007/07/04 09:57:22 bfzhang Exp $
*************************************************************/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "STRING_UTILITY.h"
#include "ARRAYLIST.h"

typedef unsigned int UINT32;
typedef unsigned char BOOL;

#define TRUE 1
#define FALSE 0

#ifndef MAX_LINE_LENGTH
#define MAX_LINE_LENGTH 1024
#endif

unsigned long long STRU_countSingleChar(const char *szBuff,const char c)
{
    if(NULL == szBuff || c == STRING_END)
        return 0;

	char *p = (char *)szBuff;
	unsigned long long num = 0;
	//assert(szBuff != NULL && c != STRING_END);

	while(*p != STRING_END)
	{
		if(*p == c) num++;
		p++;
	}
	
	return num;
}

char * STRU_fileToBuff(const char *szFile)
{
    if(NULL == szFile)
        return NULL;

	FILE *fp = NULL;
	char *szBuff = NULL;
	char szLineBuff[MAX_LINE_LENGTH + 1];
	unsigned long long lLenSzBuff = 0;
	unsigned long long lSizeOfSzBuff = 0;
	unsigned long long dwTmpLen = 0;

	//assert(szFile != NULL);
    
	fp = fopen(szFile,"r");
	if(fp == NULL) return NULL;

	szBuff = (char *)calloc(MAX_LINE_LENGTH + 1,sizeof(char));
	if(szBuff == NULL) return NULL;
	lSizeOfSzBuff = MAX_LINE_LENGTH + 1;

	while ( !feof(fp))
	{
		memset(szLineBuff,0,MAX_LINE_LENGTH + 1);
		fgets(szLineBuff,MAX_LINE_LENGTH,fp);
		dwTmpLen = strlen(szLineBuff);

		if( (lLenSzBuff + dwTmpLen ) >= lSizeOfSzBuff)
		{
			szBuff = (char *)realloc(szBuff,2 * lSizeOfSzBuff + 1);
			if(szBuff == NULL) return NULL;
			lSizeOfSzBuff = 2 * lSizeOfSzBuff + 1;
		}
		strncat(szBuff,szLineBuff,dwTmpLen);
		lLenSzBuff += dwTmpLen;
	}
	return szBuff;	
}

unsigned char STRU_hasArabia(const char *szBuff,const unsigned int lenBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return FALSE;
	int i = 0;
	for( i = 0; i < lenBuff;i++)
	{
		if(szBuff[i] >= '0' && szBuff[i] <= '9') return TRUE; 
	}
	return FALSE;
}

unsigned char STRU_hasLetter(const char *szBuff,const unsigned int lenBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return FALSE;

	int i = 0;
	for( i = 0;i < lenBuff;i++)
	{
		if(szBuff[i] >= 'A' && szBuff[i] <= 'Z') return TRUE;
		if(szBuff[i] >= 'a' && szBuff[i] <= 'z') return TRUE;
	}
	return FALSE;
}
void STRU_trimStart(char *szBuff,const char *szStart)
{
	//assert(szBuff != NULL && szStart != NULL);
    if(NULL == szBuff || NULL == szStart)
        return;
	
	int lenSzBuff = strlen(szBuff);
	int lenSzStart = strlen(szStart);
	if(lenSzBuff < lenSzStart) return;
	int startPos = 0;
	
	while( STRU_startWith(szBuff + startPos,szStart) == TRUE)
	{
		startPos += lenSzStart;
	}
	
	memmove(szBuff,szBuff + startPos,lenSzBuff - startPos);
	
	szBuff[lenSzBuff - startPos] = STRING_END;
}

void STRU_trimEnd(char *szBuff,const char *szEnd)
{
	//assert(szBuff != NULL && szEnd != NULL);
    if(NULL == szBuff || NULL == szEnd)
        return;
	
	int lenSzBuff = strlen(szBuff);//strlen don't include '\0'
	int lenSzEnd = strlen(szEnd);
	
	if(lenSzBuff < lenSzEnd) return;
	
	while( strncmp(szBuff + lenSzBuff - lenSzEnd,szEnd,lenSzEnd) == 0)
	{
		lenSzBuff -= lenSzEnd;
	}
	szBuff[lenSzBuff] = STRING_END;
}
int STRU_CHAR_getEditDistance(const char *szLeft,unsigned int leftLen,const char *szRight,unsigned int rightLen,
							const unsigned int addWeight,const unsigned int deleteWeight,const unsigned int subsWeight)
{
	//assert(szLeft != NULL && szRight != NULL);
    if(NULL == szLeft)
        return rightLen;
    if(NULL == szRight)
        return leftLen;
	
	int i =0,j = 0;
	//step1
	if(leftLen == 0) return rightLen;
	if(rightLen == 0) return leftLen;
	
	//step2:
	int matrix[leftLen + 1][rightLen + 1];
	for( i = 0;i <= leftLen;i++) matrix[i][0] = i;
	for( j = 0;j <= rightLen;j++) matrix[0][j] = j;
	
	//step3:
	int cost = 0;
	for( i = 1;i <=leftLen;i++)
	{
		for(j = 1;j <= rightLen;j++)
		{
			cost = (szLeft[ i - 1] == szRight[ j - 1])? 0 : subsWeight;
			matrix[ i ][ j ] = minimum(matrix[ i - 1][ j ] + addWeight, matrix[ i ][ j - 1] + deleteWeight,matrix[ i - 1][ j - 1] + cost);
		}
	}
	return matrix[ leftLen ][ rightLen ];
}
void STRU_charToShort(const char *szBuff,unsigned short *pwBuff,unsigned int *pwBuffLen)
{
	//assert(szBuff != NULL && pwBuff != NULL && pwBuffLen != NULL);
    if(NULL == szBuff || NULL == pwBuff || NULL == pwBuffLen)
        return;
	
	int len = strlen(szBuff);
	int i = 0,j = 0;
	for( i = 0; i < len;)
	{
		if(szBuff[i] > 0)
		{
			pwBuff[ j ] = (unsigned char)szBuff[i];
			i++;j++;
		}
		else
		{
			if( ( i + 1) >= len)
			{
				//fprintf(stderr,"there is a half Chinese Character in %s, strlen = %d, error position = %d",szBuff,len,i);
				break;
			} 
			pwBuff[ j ] = (unsigned char)szBuff[ i ];
			pwBuff[ j ] = (pwBuff[ j ] << 8)  | (unsigned char)szBuff[ i + 1];
			i += 2; j++;
		}
	}
	*pwBuffLen = j;
} 

int STRU_CHAR_getMaxComLenFromStart(const char *szLeft,unsigned int leftLen,const char *szRight,unsigned rightLen)
{
	//assert(szLeft != NULL && szRight != NULL);
    if(NULL == szLeft || NULL == szRight)
        return 0;
	
	register  int common = 0;
	for(common = 0; common < leftLen && common < rightLen;common++)
	{
		if( szLeft[common] != szRight[common]) break;
	}
	return common;
}


char * STRU_CHIN_strstr(const char *szParent,const char *szSub)
{
	//assert(szParent != NULL && szSub != NULL);
    if(NULL == szParent || NULL == szSub)
        return NULL;
	int lenParent = strlen(szParent);
	int lenSub = strlen(szSub);
	int i = 0;
	for(i = 0; i < lenParent;)
	{
		if( strncmp(szParent + i,szSub,lenSub) == 0) return (char *)(szParent + i);
		if( *(szParent + i) > 0)
		{ i++;continue;}
		else
		{ i += 2;}
	}	

	return NULL;
}
int STRU_CHIN_strlen(const char *szBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return 0;
	int i = 0;
	int len = strlen(szBuff);
	int chinLen = 0;
	for( i = 0; i < len;)
	{
		if(szBuff[i] > 0)
		{
			chinLen++;
			i++;
		}
		else
		{
			if( ( i + 1) >= len)
			{ 
				//fprintf(stderr,"can't parse this string\n");
				break;
			}
			i += 2;
			chinLen += 1;
		}
	}
	return chinLen;
}

int STRU_CHIN_getMaxComLenFromStart(const unsigned short *pwLeftArray,const unsigned int leftArrayLen,
							    			const unsigned short *pwRightArray,const unsigned int rightArrayLen)
{
	//assert(pwLeftArray != NULL && pwRightArray != NULL);
    if(NULL == pwLeftArray || NULL == pwRightArray)
        return 0;
	
	int i = 0;
	//step1:
	for( i = 0; i < leftArrayLen && i < rightArrayLen;)
	{
		if( pwLeftArray[i] != pwRightArray[i]) break;	
		i++;
	}

	return i;
}
void STRU_CHIN_getMaxCommonSub(const unsigned short *pwLeftArray,const unsigned int leftArrayLen,
							    		const unsigned short *pwRightArray,const unsigned int rightArrayLen,
									unsigned int *leftBegin,unsigned int * rightBegin,unsigned int *maxSubLen)
{
	//assert(pwLeftArray != NULL && pwRightArray != NULL && leftBegin != NULL && rightBegin != NULL && maxSubLen != NULL);
    if(pwLeftArray == NULL || pwRightArray == NULL || leftBegin == NULL || rightBegin == NULL || maxSubLen == NULL)
        return;
	
	//step1: set 0 
	int row = 0,col =0;
	int matrix[leftArrayLen + 1][rightArrayLen + 1];
	
	for(row = 0; row < (leftArrayLen +1);row++)
	{
		for(col = 0; col < (rightArrayLen + 1);col++)
		{
			matrix[ row ][ col ] = 0;
		}
	}
	
	//dynamic programming
	*maxSubLen = 0;
	for(row = 0; row < leftArrayLen;row++)
	{
		for(col = 0;col < rightArrayLen;col++)
		{
			if(pwLeftArray[row] == pwRightArray[col])
			{
				matrix[row + 1][col + 1] = matrix[ row ][ col ] + 1;
				if( matrix[row + 1][col + 1] > *maxSubLen)
				{
					*maxSubLen = matrix[row + 1][col + 1];
					*leftBegin = row + 1 - *maxSubLen;
					*rightBegin = col + 1 - *maxSubLen;
				}
			}
			else
			{
				matrix[row][col] = 0;
			}
		}
	}
}

int STRU_CHIN_getEditDistance(const unsigned short *pwLeftArray,const unsigned int leftArrayLen,
							   const unsigned short *pwRightArray,const unsigned int rightArrayLen,
							   const unsigned int addWeight,const unsigned int deleteWeight,const unsigned int subsWeight)
{
	//assert(pwLeftArray !=NULL && pwRightArray != NULL);
    if(NULL == pwLeftArray)
        return rightArrayLen;
    if(NULL == pwRightArray)
        return leftArrayLen;
	
	int i =0,j = 0;	
	//step1:
	if(leftArrayLen == 0) return rightArrayLen;
	if(rightArrayLen == 0) return leftArrayLen;
	
	//step2:
    static int matrix[1001][1001];
    int leftLen = (leftArrayLen > 1000) ? 1000 : leftArrayLen;
    int rightLen = (rightArrayLen > 1000) ? 1000 : rightArrayLen;
	for(i = 0; i <= leftLen;i++) matrix[i][0] = i;
	for(j = 0; j <= rightLen;j++) matrix[0][j] = j;
	
	//step3:
	int cost = 0;
	for( i = 1;i <= leftLen;i++)
	{
		for( j = 1;j <= rightLen;j++)
		{
			cost = ( pwLeftArray[i - 1]== pwRightArray[j - 1])?0:subsWeight;

			matrix[i][j] = minimum(matrix[i-1][j] +addWeight,matrix[i][j-1] +deleteWeight,matrix[i-1][j-1] + cost);
		}
	}

	return matrix[leftLen][rightLen];
}


char *STRU_substr(const char *szBuff,int beginPos,int length)
{
	char *string = NULL;
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return NULL;

	string = (char *)calloc(length + 1,sizeof(char));
	if(string == NULL) return NULL;

	strncpy(string,szBuff + beginPos,length);

	return string;
}

void STRU_CHAR_remove(char *szBuff,const char *szRemove)
{
    //assert(szBuff != NULL && szRemove != NULL);
    if(NULL == szBuff || NULL == szRemove)
        return;
	
    int lenSzBuff = strlen(szBuff);
    int lenSzRemove = strlen(szRemove);
	
	if(lenSzBuff == 0 || lenSzRemove == 0) return;	

    if(lenSzRemove > lenSzBuff) return;

    int position[lenSzBuff/lenSzRemove];
    memset(position,0,sizeof(position));
	
    int i = 0;
	int number = 0;

	//step1:find all szRemove
 	for(i =0 ;i < lenSzBuff;)
	{
		char *p = strstr(szBuff + i,szRemove);
		if( p != NULL)
		{
			position[number] = p - szBuff;
			i = p -szBuff + lenSzRemove;
			number++;
		}
		else
		{	
			break;
		}
	}
	if( number == 0) return;
	
	//step2:cat all part string
	int newLen = 0;
	char *szTmp = (char *)calloc(lenSzBuff + 1,sizeof(char));
	if(szTmp == NULL) return;

	for( i = 0; i < number;i++)
	{	
		if( i == 0)
		{ 
			strncpy(szTmp,szBuff,position[i]);
			newLen += position[i];
			if( (i + 1) < number)//number >=2
			{
				strncat(szTmp,szBuff + position[i] + lenSzRemove,position[i + 1] - position[i] - lenSzRemove);
				newLen += position[i + 1] - position[i] - lenSzRemove;
			}
			else
			{
				strncat(szTmp,szBuff + position[i] + lenSzRemove,lenSzBuff - position[i] - lenSzRemove);
				newLen += lenSzBuff - position[i] - lenSzRemove;
				break;
			}	
			continue;
		}
		
		//number >= 2
		if(i < (number - 1))
		{
			strncat(szTmp,szBuff + position[i] + lenSzRemove,position[i + 1] - position[i] - lenSzRemove);
			newLen += position[i + 1] - position[i] - lenSzRemove;
		}
		else
		{
			strncat(szTmp,szBuff + position[i] + lenSzRemove,lenSzBuff - position[i] - lenSzRemove);
			newLen += lenSzBuff - position[i] - lenSzRemove;
		}
	}
	//step3:copy string && 
	strncpy(szBuff,szTmp,newLen);
	szBuff[newLen] = '\0';
	FREE(szTmp);
}
void STRU_CHIN_remove(char *szBuff,const char *szRemove)
{
    //assert(szBuff != NULL && szRemove != NULL);
    if(NULL == szBuff || NULL == szRemove)
        return;
	
    int lenSzBuff = strlen(szBuff);
    int lenSzRemove = strlen(szRemove);
	
	if(lenSzBuff == 0 || lenSzRemove == 0) return;	

    if(lenSzRemove > lenSzBuff) return;

    int position[lenSzBuff/lenSzRemove];
    memset(position,0,sizeof(position));
	
    int i = 0;
	int number = 0;

	//step1:find all szRemove
 	for(i =0 ;i < lenSzBuff;)
	{
		char *p = STRU_CHIN_strstr(szBuff + i,szRemove);
		if( p != NULL)
		{
			position[number] = p - szBuff;
			i = p -szBuff + lenSzRemove;
			number++;
		}
		else
		{	
			break;
		}
	}
	if( number == 0) return;
	
	//step2:cat all part string
	int newLen = 0;
	char *szTmp = (char *)calloc(lenSzBuff + 1,sizeof(char));
	if(szTmp == NULL) return;

	for( i = 0; i < number;i++)
	{	
		if( i == 0)
		{ 
			strncpy(szTmp,szBuff,position[i]);
			newLen += position[i];
			if( (i + 1) < number)//number >=2
			{
				strncat(szTmp,szBuff + position[i] + lenSzRemove,position[i + 1] - position[i] - lenSzRemove);
				newLen += position[i + 1] - position[i] - lenSzRemove;
			}
			else
			{
				strncat(szTmp,szBuff + position[i] + lenSzRemove,lenSzBuff - position[i] - lenSzRemove);
				newLen += lenSzBuff - position[i] - lenSzRemove;
				break;
			}	
			continue;
		}
		
		//number >= 2
		if(i < (number - 1))
		{
			strncat(szTmp,szBuff + position[i] + lenSzRemove,position[i + 1] - position[i] - lenSzRemove);
			newLen += position[i + 1] - position[i] - lenSzRemove;
		}
		else
		{
			strncat(szTmp,szBuff + position[i] + lenSzRemove,lenSzBuff - position[i] - lenSzRemove);
			newLen += lenSzBuff - position[i] - lenSzRemove;
		}
	}
	//step3:copy string && 
	strncpy(szBuff,szTmp,newLen);
	szBuff[newLen] = '\0';
	FREE(szTmp);
}
int STRU_toLowCase(char *szBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return -1;
	int i = 0;
       int len = strlen(szBuff);
    	for (i = 0; i< len; i++)
    {
		if((szBuff[i] > 64) && (szBuff[i] < 91))
        {
			szBuff[i] += 32;
		}
    }
	return (i >= len)?0:-1;
}
int STRU_removeTabAndNewline(char *szBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return -1;
	
	int preNumber = 0;
	int currNumber = 0;
	char c;
	int len = strlen(szBuff);
	if(len == 0) return 0;

	for( ;preNumber < len;)
	{
		c = szBuff[preNumber];
		if( STRU_isTab(c) == 0 || STRU_isEnter(c) == 0 || STRU_isNewLine(c) == 0)
		{
			szBuff[currNumber++] = SPACE;
			preNumber++;
		}
		else
		{
			szBuff[currNumber++] = szBuff[preNumber++];
		}
	}
	//remove last space
	if(currNumber >= 1 && szBuff[currNumber - 1] == SPACE) szBuff[currNumber - 1] = STRING_END;

	return (preNumber >= len)?0:-1;
}
int STRU_SBCToDBC(char *szBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return -1;
	
	int len=strlen(szBuff);
	int preNumber = 0;
	int currNumber = 0;
	for(; preNumber < len; )
	{
		if(szBuff[preNumber] < 0 )/*SBC and chinese character*/
		{
			if(szBuff[preNumber] == -93)/*SBC character 163,except SBC space*/
			{
				preNumber++;
				szBuff[currNumber] = szBuff[preNumber] + 128;
				currNumber++;preNumber++;
			}
			else if(szBuff[preNumber] == -95 && szBuff[preNumber+1] == -95)/*161 SBC SPACE*/
			{
				preNumber = preNumber + 2;
				szBuff[currNumber++] = SPACE;/*just add one space*/
			} 
			else/*chinese character*/
			{
				szBuff[currNumber++] = szBuff[preNumber++];
				szBuff[currNumber++] = szBuff[preNumber++];				
			}
		}
		else
		{
			szBuff[currNumber++] = szBuff[preNumber++];
		}
	}
	szBuff[currNumber] = STRING_END;

	return (preNumber >= len)?0:-1;
}



int STRU_combineMultipleSpaceToOne(char *szBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return -1;
	int preNumber = 0 ;
	int currNumber = 0;
	int len = strlen(szBuff);
	for(; preNumber < len; )
	{
		if( szBuff[preNumber] == SPACE)
		{
			if(preNumber == 0)/*first*/
			{
				szBuff[currNumber++] = szBuff[preNumber++];
			}
			else
			{
				if( szBuff[currNumber - 1] != SPACE)
				{ 
					szBuff[currNumber++] = szBuff[preNumber];/*the first space*/
				}
				preNumber++;
			}
		}
		else
		{
			szBuff[currNumber++] = szBuff[preNumber++];			
		}	
	}
	szBuff[currNumber] = STRING_END;

	return (preNumber >= len)?0:-1;
}

/**
@desc: replace Chinese punctuations with English punctuations
	   just mapping
	   Note:确保szBuff已经进行全半角转换
	  中文字符与英文字符的对应关系为多对一
*/
//int   STRU_replaceChinPunc(char *szBuff,HashMap * stpPunctHashMap)
//{
//	assert(szBuff != NULL && stpPunctHashMap != NULL);
//	
//	int buffLen = strlen(szBuff);
//	char szChinPunct[3];
//	int preNumber = 0;
//	int currNumber = 0;
//   	char *szEngPunct= NULL;
//
//	for( ; preNumber < buffLen ; )
//	{
//		if(szBuff[preNumber] > 0) 
//		{
//			preNumber++;
//			continue;
//		}
//		
//	 	 //use map
//		memset(szChinPunct,0,3);
//		strncpy(szChinPunct,szBuff + preNumber ,CHINESE_CHARACTER_LENGTH);
//		szEngPunct = (char *)getValueHashMap(stpPunctHashMap,szChinPunct);
//		if(szEngPunct != NULL)
//		{
//			szBuff[currNumber++] = szEngPunct[0];
//			preNumber += 2;
//			continue;
//		}
//		FREE(szEngPunct);
//		
//		szBuff[currNumber++] = szBuff[preNumber++];
//		szBuff[currNumber++] = szBuff[preNumber++];
//	}
//	return 0;
//}
/**
 * split a string into a arraylist
 */
ArrayList* STRU_split(const char *szInput,const char *szSepa)
{
	//assert(szInput != NULL && szSepa != NULL);
    if(NULL == szInput || NULL == szSepa)
        return NULL;
	
	unsigned int dwArrayListInitSize = 5;
	ArrayList *stpArrayList = ArrayList_create(dwArrayListInitSize);
	if(stpArrayList == NULL) return NULL;
	
	int dwSzSepaLen = strlen(szSepa);
	int dwSzInputLen = strlen(szInput);
	int dwTmpLen = 0;
	
	char *cpPrePos = (char *)szInput;
	char *cpCurrPos = (char *)szInput;

	cpCurrPos = strstr(cpPrePos,szSepa);
	//strstr: failed return NULL
	while(cpCurrPos != NULL)
	{
		/* if( (cpPrePos - cpCurrPos) == 0) add not null*/
		dwTmpLen = cpCurrPos - cpPrePos;
		char *szValue = (char *)calloc(dwTmpLen + 1,sizeof(char));
		if(szValue == NULL) return NULL;
		strncpy(szValue,cpPrePos,dwTmpLen);
		//fprintf(stderr,"%s-",szValue);
		ArrayList_addNode(stpArrayList,szValue);//deep copy
		FREE(szValue);
		
		cpPrePos = cpCurrPos + dwSzSepaLen;
		cpCurrPos = strstr(cpPrePos,szSepa);
	}
	
	//add the last one
	dwTmpLen = szInput +dwSzInputLen - cpPrePos;
	char *szValue = (char *)calloc(dwTmpLen + 1,sizeof(char));
	if(szValue == NULL) return NULL;
	strncpy(szValue,cpPrePos,dwTmpLen);
	ArrayList_addNode(stpArrayList,szValue);
	//fprintf(stderr,"%s-",szValue);
	FREE(szValue);
	
	return stpArrayList;
}
int string_compare(const void *arg1,const void *arg2)
{
	//assert(arg1 != NULL && arg2 != NULL);
    if(NULL == arg1 ||NULL == arg2)
        return -1;
	
	return strcmp((char*)arg1,(char *)arg2);
}

void STRU_CHAR_getMaxCommonSub(const char *szLeft,const char *szRight,unsigned int *leftBegin,unsigned int *maxSubLen)
{
	//assert(szLeft != NULL && szRight != NULL && leftBegin != NULL);
    if(NULL == szLeft || NULL == szRight ||NULL == leftBegin)
        return;
	
	int leftLen = strlen(szLeft);
	int rightLen = strlen(szRight);
	int row = 0,col = 0;
	int matrix[leftLen + 1][rightLen + 1];
	*leftBegin = 0,*maxSubLen =0;
	
	//step1: set 0
	for(row = 0; row < (leftLen + 1);row++)
	{
		for(col = 0; col < (rightLen + 1);col++)
		{
			matrix[row][col] = 0;	
		}
	}
	
	//step2:dynamic programming
	for(row = 0; row < leftLen;row++)
	{
		for(col = 0;col < rightLen;col++)
		{
			if(szLeft[row] == szRight[col])
			{
				matrix[row + 1][col + 1] = matrix[row][col] + 1;
				if(matrix[row + 1][col + 1] > *maxSubLen)
				{
					*maxSubLen = matrix[row + 1][col+1];
					*leftBegin = row + 1 - *maxSubLen;	
				}
			}
			else
			{
				matrix[row][col] = 0;
			}
		}
	}
}
void STRU_CHAR_reverse(char *szBuff)
{
	//assert(szBuff != NULL);
    if(NULL == szBuff)
        return;
	int len = strlen(szBuff);
	if(len == 0) return;
	
	char tmp;
	int i = 0;
	for( i = 0;i < len/2;i++)
	{
		tmp = szBuff[i];
		szBuff[i] = szBuff[len - 1 - i];
		szBuff[len - 1 - i] = tmp;
	}	
}

