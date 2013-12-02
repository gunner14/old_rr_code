/**
 *@file number.c
 *@author bfzhang
 *@brief provided the number transformation api
 *       create the file in 2007-08-27
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "number.h"


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FASLE
#define FALSE 0
#endif

const long long  YI_NUMBER = 100000000;
const long long  WAN_NUMBER = 10000;
const long long  QIAN_NUMBER = 1000;
const long long  BAI_NUMBER = 100;

#define GB_LEN 2

//gb2312编码
//the position is important can't change!!
const char GB_NUMBER_COUNT = 15;
const char *GB_NUMBER[] = 
{
    "零","一","二","三","四",
    "五","六","七","八","九",
    "十","百","千","万","亿"
};

const char SHI[ ] = "十";
const char YI[ ] = "一";

/***********************************************/
/***private interface declaration***************/
/***********************************************/

/**
 *@retval success 0;fail -1
 */
int cnum2num_check(const char *zhStr);

/**
 *@brief arabic numeral
 */
long long cnum2num_get(const char *zhSrc); 

/**
 *@brief trans number < 100
 *@retval success >= 0;fail return -1;
 */
int cnum2num_parsebai(const char *zhSrc);

/**
 *@brief trans number < 100(check)
 *@retval success >= 0;fail return -1
 */
int cnum2num_parsebai_check(const char *zhSrc);

/**
 *@brief trans number < 10000
 *@retval success >= 0;fail return -1
 */
int cnum2num_parsewan(const char *zhSrc);

/**
 *@brief trans number < 10000(check)
 *@retval success >= 0;fail return -1
 */
int cnum2num_parsewan_check(const char *zhSrc);

/**
 *@brief get the freq of word in src
 */ 
int getFreq(const char *src,const char *word);    

/**
 *@brief compare GB_NUMBER
 */ 
int getPos(const char *word);

/***********************************************/
/***public interface implementation*************/
/***********************************************/

long long cnum2num(const char *zhStr)
{
    //assert(zhStr != NULL);
    if(NULL == zhStr)
        return -1;
    //step1:check
    if(cnum2num_check(zhStr) == -1)
        return -1;

    //step2: number
    return cnum2num_get(zhStr);
}

/***********************************************/
/***private interface implemenation*************/
/***********************************************/

int cnum2num_check(const char *zhStr)
{
    if(zhStr == NULL || zhStr[0] == '0') 
        return -1;

    int len = strlen(zhStr);
    if(len % 2 != 0)
        return -1;
    
    for(int cover = 0;cover < (len - 1);cover += GB_LEN)
    {
        int pos = -1;
        for(int i = 0;i < GB_NUMBER_COUNT;i++)
        {
            if(strncmp(zhStr + cover,GB_NUMBER[i],GB_LEN) == 0)
            {
                pos = i;
                break;
            }
        }
        if(pos == -1)
            return -1;                
    }

    return 0;
}

long long cnum2num_get(const char *zhSrc)
{
    //assert(zhSrc != NULL);
    if(NULL == zhSrc)
        return -1;
    
    int len = strlen(zhSrc);
    int cover = 0;
    long long result = 0;
    
    //step1:find "亿"
    char *pYi = strstr(zhSrc + cover,"亿");
    if(pYi != NULL)
    {
        int tmpLen = pYi - zhSrc - cover;
        char tmp[tmpLen + 1];
        memset(tmp,0,sizeof(tmp));
        strncpy(tmp,zhSrc + cover,tmpLen);
 
        int parseNumber = cnum2num_parsewan(tmp);
        if(parseNumber == -1)
            return -1;
        else
            result += parseNumber * YI_NUMBER;            
 
        cover += tmpLen + GB_LEN;
        if(cover == len)
            return result;
    }
    
    if(strncmp(zhSrc + cover,"零",GB_LEN) == 0)
        cover += GB_LEN;
    if(cover == len)
        return result;

    //step2:find "万"
    char *pWan = strstr(zhSrc + cover,"万");
    if(pWan != NULL)
    {
        int tmpLen = pWan - zhSrc - cover;
        char tmp[tmpLen + 1];
        memset(tmp,0,sizeof(tmp));
        strncpy(tmp,zhSrc + cover,tmpLen);

        int parseNumber = cnum2num_parsewan(tmp);
        if(parseNumber == -1)
            return -1;
        else
            result += parseNumber * WAN_NUMBER;

        cover += tmpLen + GB_LEN;
        if(cover == len)
            return result;
    }
    
    if(strncmp(zhSrc + cover,"零",GB_LEN) == 0)
        cover += GB_LEN;
    if(cover == len)
        return result;
 
    //step3:parse the left
    char tmp[len + 1];
    memset(tmp,0,sizeof(tmp));
    strncpy(tmp,zhSrc + cover,len - cover);

    int parseNumber = cnum2num_parsewan(tmp);
    if(parseNumber == -1)
        return -1;
    else
        result += parseNumber;
 
    cover = len;  
    return result;
}


int cnum2num_parsebai_check(const char *zhSrc)
{
    //assert(zhSrc != NULL);
    if(NULL == zhSrc)
        return -1;
    if(zhSrc[0] == '\0')
        return -1;

    int len = strlen(zhSrc);
    if(len % 2 != 0)
        return -1;
    if(len /2 > 3)
        return -1;
    
    for(int cover = 0;cover < (len -1);cover += 2)
    {
        int pos = -1;
        for(int i = 0;i < GB_NUMBER_COUNT;i++)
        {
            if(strncmp(zhSrc + cover,GB_NUMBER[i],GB_LEN) == 0)
            {
                pos = i;
                break;
            }
        }
        if(pos == -1 || pos >= 11)
            return -1;
    }
    return 0;
}

int cnum2num_parsebai(const char *zhSrc)
{
    //assert(zhSrc != NULL);
    if(NULL == zhSrc)
        return -1;

    //first check 
     
    int len = strlen(zhSrc);
    int result = 0;
    char word[GB_LEN + 1];
    char bai[len + 1];
    memset(bai,0,sizeof(bai));

    for(int cover = 0;cover < (len - 1);cover += GB_LEN)
    {
        memset(word,0,sizeof(word));
        strncpy(word,zhSrc + cover,GB_LEN);

        int pos = getPos(word);

        if(pos >= 0 && pos <= 9)
        {
            char tmp[GB_LEN];
            memset(tmp,0,sizeof(tmp));
            sprintf(tmp,"%d",pos);
            strcat(bai,tmp);
        }
        else if(pos == 10)
        {
            if(cover/GB_LEN == 0 && len/GB_LEN == 1)//"十"
               strcat(bai,"10");
            else if(cover/GB_LEN == 0 && len/GB_LEN == 2)//"十一"
               strcat(bai,"1");
            else if(cover/GB_LEN == 1 && len/GB_LEN == 2)//"二十"
                strcat(bai,"0");
            else if(cover/GB_LEN == 1 && len/GB_LEN == 3)//"二十五"
                 ;
            else
            {
                fprintf(stderr,"impossible,cnum2num_parsebai\
                                can't parse %s\n",zhSrc);
                return -1;
            }
        }
    }
    
    result = atoi(bai);
    return result;    
}


int cnum2num_parsewan_check(const char *zhSrc)
{
    //assert(zhSrc != NULL);
    if(NULL == zhSrc)
        return -1;
    if(zhSrc[0] == '\0')
        return -1;

    int len = strlen(zhSrc);
    if(len % 2 != 0)
        return -1;
    
    for(int cover = 0;cover < (len -1);cover += 2)
    {
        int pos = -1;
        for(int i = 0;i < GB_NUMBER_COUNT;i++)
        {
            if(strncmp(zhSrc + cover,GB_NUMBER[i],GB_LEN) == 0)
            {
                pos = i;
                break;
            }
        }
        if(pos == -1 || pos >= 13)
            return -1;
    }

    return 0;
}


int cnum2num_parsewan(const char *zhSrc)
{
    //assert(zhSrc != NULL);
    if(NULL == zhSrc)
        return -1;

    int len = strlen(zhSrc);
    int cover = 0;
    int result = 0;

    //first check

    //step1:find "千"
    char *pQian = strstr(zhSrc + cover,"千");
    if(pQian != NULL)
    {
        if(pQian - zhSrc != GB_LEN)
            return -1;

        char tmp[GB_LEN + 1];
        memset(tmp,0,sizeof(tmp));
        strncpy(tmp,zhSrc + cover,GB_LEN);
        int pos = getPos(tmp);
        
        if(pos < 1 || pos > 9)
            return -1;

        result += pos * QIAN_NUMBER;
        cover += GB_LEN + GB_LEN;
        if(cover == len)
            return result;
    }

    if(strncmp(zhSrc + cover,"零",GB_LEN) == 0)
        cover += GB_LEN;
    if(cover == len)
        return result;
    
    //step2:find "百"
    char *pBai = strstr(zhSrc + cover,"百");
    if(pBai != NULL)
    {
        if( (pBai - zhSrc - cover) != GB_LEN)
            return -1;

        char tmp[GB_LEN + 1];
        memset(tmp,0,sizeof(tmp));
        strncpy(tmp,zhSrc + cover,GB_LEN);
        int pos = getPos(tmp);

        if(pos < 1 || pos > 9)
            return -1;
        result += pos * BAI_NUMBER;
        cover += GB_LEN + GB_LEN;
        if(cover == len)
            return result;
    }

    if(strncmp(zhSrc + cover,"零",GB_LEN) == 0)
        cover += GB_LEN;
    if(cover == len)
        return result;

    //step3:case "一千二"
    if(pQian != NULL && pBai == NULL && 
       len / GB_LEN == 3)
    {
        if((cover + GB_LEN) != len)
            return -1;
        char tmp[GB_LEN + 1];
        memset(tmp,0,sizeof(tmp));
        strncpy(tmp,zhSrc + cover,GB_LEN);
        int pos = getPos(tmp);
        
        if(pos < 1 || pos > 9)
            return -1;
        result += pos * 100;
        cover += GB_LEN;
        return result;  
    }
    
    char *pShi = strstr(zhSrc + cover,"十");

    //step5:find case "二百二"
    if(pBai != NULL && pShi == NULL &&
       len / GB_LEN == 3)
    {
        if((cover + GB_LEN) != len)
            return -1;
        char tmp[GB_LEN + 1];
        memset(tmp,0,sizeof(tmp));
        strncpy(tmp,zhSrc + cover,GB_LEN);
        int pos = getPos(tmp);
        
        if(pos < 1 || pos > 9)
            return -1;

        result += pos * 10;
        cover += GB_LEN;
        return result;  
    }
    
    char tmp[len + 1];
    memset(tmp,0,sizeof(tmp));
    strcat(tmp,zhSrc + cover);
    int parseNumber = cnum2num_parsebai(tmp);
    if(parseNumber == -1)
        return -1;
    else
        result += parseNumber;

    return result;
}
   
int getFreq(const char *src,const char *word)
{
    //assert(src != NULL && word != NULL);
    if(NULL == src ||NULL == word)
        return 0;
    int srcLen = strlen(src);
    int wordLen = strlen(word);

    int freq = 0;
    int cover = 0;
    char *p = NULL;
    while( cover < srcLen && (p = strstr(src + cover,word)) != NULL)
    {
        freq += 1;
        cover = p - src + wordLen;
    }

    return freq;
} 

int getPos(const char *word)
{
    //assert(word != NULL);
    if(NULL == word)
        return -1;

    for(int i = 0; i < GB_NUMBER_COUNT;i++)
    {
        if(strncmp(word,GB_NUMBER[i],GB_LEN) == 0)
            return i;
    }
    return -1;
}

