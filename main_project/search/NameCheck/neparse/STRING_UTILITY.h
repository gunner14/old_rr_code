/************************************************************
*Filename: STRING_UTILITY.h
*
*Date:   2007-02-25
*Version: 0.1
*Description: collect the string utility funcitons in c language
* change log
* (1)create the file 2007-02-25
*(2) add functions 2007-03-13
*************************************************************/
#include "ARRAYLIST.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _STRING_UTILITY_H
#define _STRING_UTILITY_H
 
#define STRING_END '\0'
#define STRING_NEW_LINE '\n'
#define HYPHEN '-'
#define SPACE ' '


#ifndef MAX_WORD_LENGTH 
#define MAX_WORD_LENGTH 1024
#endif

#ifndef FREE
#define FREE(p) if(p){free(p); p = NULL;}
#endif

#ifndef CHINESE_CHARACTER_LENGTH
#define CHINESE_CHARACTER_LENGTH 2
#endif

#ifndef OTHER_CHARACTER_LENGTH
#define OTHER_CHARACTER_LENGTH 1
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/*
@return: success 0;fail -1;
*/
static int STRU_isSpace(char c)
{
	return (c == ' ')?0:-1;
}
/*
@return: success 0;fail -1;
*/
static int  STRU_isTab(char c)
{
	return ( c == '\t')?0:-1;
}
/*
@return: success 0;fail -1;
*/
static int STRU_isEnter(char c)
{
	return ( c == '\r')?0:-1;
}
/*
@return: success 0;fail -1;
*/
static int STRU_isNewLine(char c)
{
	return ( c == '\n')?0:-1;
}
/*
@return: success 0;fail -1;
*/
static int STRU_isHyphen(char c)
{
	return ( c == '-')?0:-1;
}
/*
@return: success 0;fail -1;
*/
static int STRU_isNumber(char c)
{
	return (c >= '0' && c<='9')?0 : -1;
}
/*
@return: success 0;fail -1;
*/
static int STRU_isLetter(char c)
{
	return (c >= 'a' && c <= 'z') || ( c >= 'A' && c<= 'Z')? 0 : -1;
}
/*
@return: success 0;fail -1;
*/
static int STRU_isPunctuation(char c)
{
	//15 + 7 + 6 + 4 = 32
	if( ( c >= 33 && c <= 47 ) ||( c >= 58 && c <= 64 ) ||
		( c >= 91 && c <= 96 ) ||( c >= 123 && c <= 126 ) )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
/**
@desc:count the number of c in szBuff
*/
unsigned long long STRU_countSingleChar(const char *szBuff,const char c);

/**
@desc: file to buff
*/
char * STRU_fileToBuff(const char *szFile);

char *STRU_substr(const char *szBuff,int beginPos,int length);

/*
@return: success TRUE;fail FALSE;
*/
 static unsigned char STRU_startWith(const char *szBuff,const char *szStart)
{
	//assert(szBuff != NULL && szStart != NULL);
    if(NULL == szBuff  || NULL == szStart)
        return FALSE;
	return strncmp(szBuff,szStart,strlen(szStart)) == 0 ?TRUE:FALSE;
}
/*
@return: success TRUE;fail FALSE;
*/
static unsigned char STRU_endWith(const char *szBuff,const char *szEnd)
{
	//assert(szBuff != NULL && szEnd != NULL);
    if(NULL == szBuff || NULL == szEnd)
        return FALSE;
	
	int lenSzEnd = strlen(szEnd);
	int lenSzBuff = strlen(szBuff);
	if(lenSzBuff < lenSzEnd) return FALSE;
	
	return strncmp(szBuff + lenSzBuff - lenSzEnd,szEnd,lenSzEnd)== 0?TRUE:FALSE;
}

/*
@description: check if the string include arabia number
@return: success TRUE;fail FALSE
*/
unsigned char STRU_hasArabia(const char *szBuff,const unsigned int lenBuff);

/*
@description:check if the string include letter
@return: success TRUE;fail FALSE
*/
unsigned char STRU_hasLetter(const char *szBuff,const unsigned int lenBuff);

/*
@return: success TRUE;FAIL FALSE 
@description:
*/
static int STRU_CHAR_isCommonLenLessThan4(const char *szLeft,
                                                 unsigned int leftLen,const char *szRight, unsigned int rightLen)
{
	//assert(szLeft != NULL && szRight != NULL);
    if(NULL == szLeft ||NULL == szRight)
        return FALSE;

    if (leftLen < 4 || rightLen <4)
        return TRUE;
	int *left = (int *)szLeft;
    int *right = (int *)szRight;
    if (*left == *right)
        return FALSE;
	return TRUE;
}

void STRU_trimStart(char *szBuff,const char *szStart);

 void STRU_trimEnd(char *szBuff,const char *szEnd);
 
int STRU_toLowCase(char *szBuff);

int STRU_removeTabAndNewline(char *szBuff);

int STRU_SBCToDBC(char *szBuff);

int STRU_combineMultipleSpaceToOne(char *szBuff);

/*
 * zhang,bu,feng=>zhang bu feng
 * zhang,,bu,feng=>zhang null bu feng
 * @strtok
 */
ArrayList* STRU_split(const char *szInput,const char *szSepa);

int STRU_stringCompare(const void *arg1,const void *arg2);


static int minimum(int x ,int y ,int z)
{
	int min = (x <= y)?x:y;
	return (min <= z)?min:z;
}
/*the algorithm from http://www.merriampark.com/ld.htm#FLAVORS*/
int STRU_CHAR_getEditDistance(const char *szLeft,unsigned int leftLen,const char *szRight,unsigned int rightLen,
				const unsigned int addWeight,const unsigned int deleteWeight,const unsigned int subsWeight);

						    			
void STRU_charToShort(const char *szBuff,unsigned short *pwBuff,unsigned int *pwBuffLen);

//global remove
void STRU_CHAR_remove(char *szBuff,const char *szRemove);

void STRU_CHIN_remove(char *szBuff,const char *szRemove);

//get max common continuous string
void STRU_CHAR_getMaxCommonSub(const char *szLeft,const char *szRight,unsigned int *leftBegin,unsigned int *maxSubLen);

int STRU_CHAR_getMaxComLenFromStart(const char *szLeft,unsigned int leftLen,const char *szRight,unsigned rightLen);

char * STRU_CHIN_strstr(const char *szParent,const char *szSub);

int STRU_CHIN_strlen(const char *szBuff);

int STRU_CHIN_getMaxComLenFromStart(const unsigned short *pwLeftArray,const unsigned int leftArrayLen,
							    			const unsigned short *pwRightArray,const unsigned int rightArrayLen);

void STRU_CHIN_getMaxCommonSub(const unsigned short *pwLeftArray,const unsigned int leftArrayLen,
							    		const unsigned short *pwRightArray,const unsigned int rightArrayLen,
									unsigned int *leftBegin,unsigned int * rightBegin,unsigned int *maxSubLen);

int STRU_CHIN_getEditDistance(const unsigned short *pwLeftArray,const unsigned int leftArrayLen,
							    const unsigned short *pwRightArray,const unsigned int rightArrayLen,
							   const unsigned int addWeight,const unsigned int deleteWeight,const unsigned int subsWeight);


void STRU_CHAR_reverse(char *szBuff);
	
#endif
