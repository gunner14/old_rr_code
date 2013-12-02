/**
 *@file AB_UTILITY_ADDR_RECOGNISE.h
 *@author bfzhang
 *@brief recognise addr
 * create the file in 2007-07-03
 */ 

#ifndef AB_UTILITY_ADDR_RECOGNISE_H_
#define AB_UTILITY_ADDR_RECOGNISE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"

/**
 * @desc:addr recognise result
 */
typedef struct _ABU_ADDR_RECOG_S
{
    ABU_ADDR_S **stpAddrArray;
    uint32_t uiAddrCount;
    
    uint32_t uiAddrPoolSize;
}ABU_ADDR_RECOG_S;

/**
 * @desc: addr recognise lib
 */
typedef struct _ABU_ADDR_RECOG_LIB_S
{
    //addr separate set
    HashSet *stpSeparSet;
    
    //中国地名用字库，首字字数，统计自中国地名库
    //HashMap *stpCharStartMap;
    
    //中国地名用字库，中字字数，统计自中国地名库
    //HashMap *stpCharMidMap;  
    
    //中国地名用字库，尾字字数，统计自中国地名库
    //HashMap *stpCharEndMap;  

    //中国地名用词库,首词词数，统计自中国地名库
    //HashMap *stpWordStartMap;
    
    //中国地名用词库，中词词数，统计自中国地名库
    //HashMap *stpWordMidMap;
    
    //中国地名用词库，尾词词数，统计自中国地名库
    //HashMap *stpWordEndMap;

    //中国地名二元同现库，尚未定义，留待扩展

    //中国地名并列关系词库，作为扩展,目前保留使用
    //HashSet *coordWordSet;

    //rule weight map
    HashMap *stpRuleWeightMap;

    //addr prefix set
    HashSet *stpPrefixSet;
    
    //addr prefix flag set
    HashSet *stpPrefixFlagSet;

    //addr index word set
    HashSet *stpIndexWordSet;

    //company suffix set
//    HashSet *stpCmpSuffixSet;
    ABU_WORDS_DICT_S *stpCmpSuffixDict;
    
    //addr featrue  word set
    HashSet *stpFeatWordSet;

    //addr suffix word
    HashSet *stpSuffixSet;

    //regular expression set
    //HashSet *stpRegExpSet; 
    
    //abu addr lib
    ABU_ADDR_LIB_S *stpAddrLib;
}ABU_ADDR_RECOG_LIB_S;

/**
 * @desc: create addr recognise lib
 * @param szDictArray basic dict and group dict
 */
ABU_ADDR_RECOG_LIB_S *ABU_ADDR_RECOG_createLib(
                const char **szDictArray,const uint32_t uiCount);

/**
 * @desc: destroy addr recognise lib
 */
void ABU_ADDR_RECOG_destroyLib(ABU_ADDR_RECOG_LIB_S *stpAddrRecogLib);

/**
 * @desc:parse string to recognise addr result
 */
ABU_ADDR_RECOG_S *ABU_ADDR_RECOG_parse(
                    const ABU_ADDR_RECOG_LIB_S  *stpRecogLib,
                    const char *szBuff);

/**
 * @desc:destroy recognise addr result
 */
void ABU_ADDR_RECOG_destroy(ABU_ADDR_RECOG_S *stpAddrRecog);

//#ifndef AB_DEBUG
/**
 * @desc print the addr recognise result
 * @param stpAddrRecog addr recog result
 * @param bVerbose print the  addr recog result in verbose format
 */
void ABU_ADDR_RECOG_print(const ABU_ADDR_RECOG_S *stpAddrRecog,
                              const BOOL bVerbose,FILE *fp);
//#endif

#endif /*AB_UTILITY_ADDR_RECOGNISE_H_*/
