/**********************************************************
 * @Filename AB_UTILITY_TEL_RECOGNISE.h
 * @Author bfzhang
 * @Date 20070-07-05
 * @Description: tel recognise 
 * @Change log
 *         $Id:         $
 * ********************************************************/
#ifndef AB_UTILITY_TEL_RECOGNISE_H_
#define AB_UTILITY_TEL_RECOGNISE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "HASHSET.h"
#include "HASHMAP.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include "AB_UTILITY_TEL.h"
#include "AB_UTILITY_TEL_RESOURCE.h"

/**
 * @desc tel recognised result
 */
typedef struct _ABU_TEL_RECOG_S
{
    //public interface
    ABU_TEL_S  **stpTelArray;
    uint32_t uiTelCount;
    
    //private interface
    uint32_t uiTelPoolSize;
}ABU_TEL_RECOG_S;

/**
 * @desc tel recognised lib
 */
typedef struct _ABU_TEL_RECOG_LIB_S
{
    //tel lib for tel parsing
    ABU_TEL_LIB_S *stpTelLib;
    
    //create by basicDict
    ABU_WORDS_DICT_S *stpSmallDict;
    
    //create by basicDict && groupDict
    ABU_WORDS_DICT_S *stpLargeDict;
    
    //seg lib
    ABU_WORDS_SEGLIB_S *stpSegLib;
        
    //tel feature word
    HashSet *stpFeatWordSet;
    
}ABU_TEL_RECOG_LIB_S;

/**
 * @desc create recog lib
 */
ABU_TEL_RECOG_LIB_S *ABU_TEL_RECOG_createLib(const char **szDictArray,
                                             const uint32_t dictCount);

/**
 * @desc destroy recog lib
 */
void ABU_TEL_RECOG_destroyLib(ABU_TEL_RECOG_LIB_S  *stpTelRecogLib);

/**
 * @desc tel recognise 
 */
ABU_TEL_RECOG_S *ABU_TEL_RECOG_parse(
        const ABU_TEL_RECOG_LIB_S  *stpTelRecogLib,
        const char *szBuff);

/**
 * @desc destroy tel recognise result
 */
void ABU_TEL_RECOG_destroy(void *stpTelRecog);

/** 
 * used for ABU_TEL_RECOG_S signature..
 */
void ABU_TEL_RECOG_sign(const void *_p, char *p_sign_buf, size_t *p_buf_size);

/**
 * @desc print tel recognise result
 */
void ABU_TEL_RECOG_print(const ABU_TEL_RECOG_S *stpTelRecog,
                            const BOOL bVerbose,FILE *fp);

#endif /*AB_UTILITY_TEL_RECOGNISE_H_*/
