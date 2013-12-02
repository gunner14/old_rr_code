/**
 *@file AB_UTILITY_NAME.h
 *@author bfzhang
 *@brief parse the name
 *@change log
 * 1.create the file in 2007-08-16
 * 2.reconstruce the file in 2007-11-15
 * 3.fix the following bug(bugId:2024 2104 2371 2372)
 * 4.reconstruct name parsing logic 2008-09-02 
 */

#ifndef AB_UTILITY_NAME_H
#define AB_UTILITY_NAME_H

#include <stdint.h>
#include "ab_hmap.h"
#include "ab_hset.h"
#include "AB_UTILITY_MAPTREE.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"

/**
 *define the name structure information
 */
typedef struct _ABU_NAME_ATTACH_S
{
	//origin name
    uint32_t originNameSign;
	uint16_t *gbOriginName;
	uint16_t gbOriginNameLen;
	
    //province city county
	uint32_t province4Char;
	uint32_t city4Char;
	uint32_t county4Char;
	
    //stem
    uint16_t *gbStemPre;
    uint16_t gbStemPreLen;  
	uint16_t *gbStem;
    uint16_t gbStemLen;
    uint16_t *gbStemPost;
    uint16_t gbStemPostLen;    

    //type
    uint16_t *gbType;
    uint16_t gbTypeLen;
    
    //assoprefix
    uint16_t *gbAssoPrefix;
    uint16_t gbAssoPrefixLen;

    //assosuffix
    uint16_t *gbAssoSuffix;
    uint16_t gbAssoSuffixLen;

    //appendix
    uint16_t *gbAppendix;
    uint16_t gbAppendixLen;
}ABU_NAME_ATTACH_S;

typedef struct _ABU_NAME_RECORD_S
{
    char *szProvince;
    char *szCity;    
    char *szCounty;
   
    char *szStemPre;    
    char *szStem;
    char *szStemPost;//(±±¾©)
    char *szType;
    char *szOrg;
    
    char *szAssoPrefix;
    char *szAssoSuffix;
    
    char *szAppendix;
    
    //special info
    //szStem(max match) szMinStem(min match)
    char *szMinStem;
    char *szMaxType;
    char *szBestType;//just for cluster splitter 
    char *szBranchPlace;
    char *szMultipleShop;
}ABU_NAME_RECORD_S;

/**
 * define the full name structure
 */
typedef struct _ABU_NAME_S
{
    char *szOriginName;
    
    ABU_NAME_RECORD_S *stpNameRecord;

    ABU_NAME_ATTACH_S *stpNameAttach;
}ABU_NAME_S;

/**
 * define the name lib for name parsing
 */
typedef struct _ABU_NAME_LIB_S
{
    ABU_WORDS_DICT_S *stpSegDict;
    ABU_WORDS_DICT_S *stpSegDictNameType;
    ABU_WORDS_SEGLIB_S *stpSegLib;

    ab_hmap *stpNameStemMap;
    ab_hmap *stpNameTypeMap;
    ab_hset *stpNameOrgSet;
    ab_hset *stpNameStemOrgFeatSet;
    ab_hset *stpNameTypeOrgFeatSet; 
    ab_hset *stpNamePlaceSet;
    ab_hset *stpAddrFeatWordSet;
    
    ABU_MAPTREE_S *stpMapTree;
    char *szDictDir;
}ABU_NAME_LIB_S;

/**
 * init the lib for name-parsing
 */
ABU_NAME_LIB_S *ABU_NAME_createLib(const char *szDictDir,
                                   const char **szDictArray,
                                   const uint32_t uiDictCount);

/**
 * clear lib
 */
void ABU_NAME_destroyLib(ABU_NAME_LIB_S *stpNameLib);

/**
 * parse name structure info
 */
ABU_NAME_RECORD_S *ABU_NAME_parseRecord(const ABU_NAME_LIB_S *stpNameLib,
                                        const char *szName);

/**
 * parse name structure info by city
 */
ABU_NAME_RECORD_S *ABU_NAME_parseRecord_bycity(const ABU_NAME_LIB_S *stpNameLib,
                                               const char *szName,
                                               const char *szCity);
/**
 * parse the merchant name
 */
ABU_NAME_S *ABU_NAME_parse(const ABU_NAME_LIB_S *stpNameLib,
                           const char *szName);

/**
 * parse the merchant name by city
 */
ABU_NAME_S *ABU_NAME_parse_bycity(const ABU_NAME_LIB_S *stpNameLib,
                                  const char *szName,
                                  const char *szCity);

/**
 * free the memory of name record
 */
void ABU_NAME_destroyRecord(ABU_NAME_RECORD_S *stpNameRecord);


/**
 * free the memory of name structure
 */
void ABU_NAME_destroy(void *stpName);

/** 
 * used for ABU_NAME_S signature..
 */
void ABU_NAME_sign(const void *p, char *p_sign_buf, size_t *p_buf_size);

/**
 * check if the word is the short best type or not.
 */
unsigned char NAME_isShortBestType(const ABU_NAME_LIB_S *stpNameLib,
                                 const char *szWord);
#endif
