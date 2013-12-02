/************************************************
 * Filename: AB_UTILITY_WORDS_UTILITY.h
 * Author:bfzhang
 * Date: 2007-03-21
 * Dec: define some public interface for index and search module
 ***********************************************/
#include "AB_UTILITY_WORDS.h"
#ifndef AB_UTILITY_WORS_UTILITY_H_
#define AB_UTILITY_WORS_UTILITY_H_

/*the following are public interfaces for index and search module**/

/*是否是单个汉字
 */
BOOL ABU_WORDS_UTILITY_isSingleChineseCharacter(const ABU_WORDS_WORD_S *stpWord);

/*是否是除单个汉字之外的可索引单位
 */
BOOL ABU_WORDS_UTILITY_isIndexWordExceptSingleChineseCharacter(const ABU_WORDS_WORD_S *stpWord);

/*是否是省级行政单位地名*/
BOOL ABU_WORDS_UTILITY_isProvince(const ABU_WORDS_WORD_S *stpWord);

/*是否是地级行政单位地名*/
BOOL ABU_WORDS_UTILITY_isCity(const ABU_WORDS_WORD_S *stpWord);

/*是否是县级行政单位地名*/
BOOL ABU_WORDS_UTILITY_isCounty(const ABU_WORDS_WORD_S *stpWord);

/*is English word*/
BOOL ABU_WORDS_UTILITY_isEnglish(const ABU_WORDS_WORD_S *stpWord);

/*is number word*/
BOOL ABU_WORDS_UTILITY_isNumber(const ABU_WORDS_WORD_S *stpWord);

/*the following are private interfaces just for AB_UTILITY_WORDS Module***/
int   ABU_WORDS_PREPROCESS_all(char *szBuff);

UINT32 getDoubleByteHashValue(const BYTE byLow,const BYTE byHigh,const UINT32 dwHashBucketSize);

UINT32 getSingleByteHashValue(const BYTE byValue,const UINT32 dwHashBucketSize);

UINT32 getHashPos(const char *szWord,const UINT32 dwHasCovered);

char * getCurrTime();
#endif /*AB_UTILITY_WORS_UTILITY_H_*/
 
