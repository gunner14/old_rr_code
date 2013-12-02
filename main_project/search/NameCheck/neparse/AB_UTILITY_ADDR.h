/**
 *@file AB_UTILITY_ADDR.h
 *@author bfzhang
 *@brief the lib for parse addr
 * create the file in 2007-07-02
 */
#include <stdint.h>
#include "HASHMAP.h"
#include "HASHSET.h"
#include "AB_UTILITY_MAPTREE.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
  
#ifndef AB_UTILITY_ADDR_H_
#define AB_UTILITY_ADDR_H_

#define MAX_ADDR_CREDIT_WEIGHT 100
#define MIN_ADDR_CREDIT_WEIGHT 0

typedef enum _ADDR_DETAILLEVEL
{
    UNKNOWN_LEVEL   = 0x00,
    PROVINCE_LEVEL  = 0x01,
    CITY_LEVEL      = 0x02,
    COUNTY_LEVEL    = 0x03,
    TOWN_LEVEL      = 0x04,
    STREET_LEVEL    = 0x05,
    STREETNO_LEVEL  = 0x06
}ADDR_DETAILLEVEL;

/**
 * @desc:strutural information of addr
 * @Note:szAddrLeft---the left address information can't be parsed
 */
typedef struct _ABU_ADDR_ATTACH_S
{
	//origin addr
    uint32_t originAddrSign;
    //uint16_t *gbOriginAddr;
	//uint16_t gbOriginAddrLen;
		
	//province city county
	uint32_t province4Char;
	uint32_t city4Char;
	uint32_t county4Char;
	
	//town
	uint16_t *gbTown;
	uint16_t gbTownLen;
		
	//street
	uint16_t *gbStreet;
	uint16_t gbStreetLen;
		
	//addr left
	uint16_t *gbAddrLeft;
	uint16_t gbAddrLeftLen;
		
	//brief addr
	//uint16_t *gbBriefAddr;
	//uint16_t gbBriefAddrLen;
}ABU_ADDR_ATTACH_S;


typedef struct _ABU_ADDR_RECORD_S
{
    char *szProvince;
    char *szCity;
    char *szCounty;
    char *szTown;
    char *szStreet;
    char *szStreetNO;
    int streetNO;
    char *szAddrLeft;
    char *szBriefAddr;

    ADDR_DETAILLEVEL detailLevel;
}ABU_ADDR_RECORD_S;

/**
 * @desc:addr infomation
 */
typedef struct _ABU_ADDR_S
{
    char *szOriginAddr;

    char *szFormatAddr;
    
    ABU_ADDR_RECORD_S *stpAddrRecord;

    ABU_ADDR_ATTACH_S *stpAddrAttach;

    unsigned char iCredWeight;//credit weight
}ABU_ADDR_S;

/**
 * @desc: addr lib for addr parsing
 */ 
typedef struct _ABU_ADDR_LIB_S
{
    ABU_WORDS_DICT_S *stpSegDict;
    
    ABU_WORDS_SEGLIB_S *stpSegLib;
    
    //addr feature word hashset
    HashSet *stpFeatWordSet;
    
    ABU_MAPTREE_S *stpMapTree;
    
}ABU_ADDR_LIB_S;


/**
 * @desc:init the lib for addr-parsing
 */
ABU_ADDR_LIB_S *ABU_ADDR_createLib(const char **szDictArray,
                                   const uint32_t uiDictCount);

/**
 * @desc:clear resource
 */
void ABU_ADDR_destroyLib(ABU_ADDR_LIB_S *stpAddrLib);


/**
 * @desc: get the address structural information
 */
ABU_ADDR_RECORD_S *ABU_ADDR_parseRecord(const ABU_ADDR_LIB_S *stpAddrLib,
                                        const char *szAddr);

/**
 * @desc: get the address structural informat by city
 */
ABU_ADDR_RECORD_S *ABU_ADDR_parseRecord_bycity(const ABU_ADDR_LIB_S *stpAddrLib,
                                               const char *szAddr,
                                               const char *szCity);

/**
 * @desc deepCopy the address strutural information
 * @param stpDst addr record destination
 * @param stpSrc addr record source
 */
void ABU_ADDR_copyRecord(ABU_ADDR_RECORD_S *stpDst,
                         const ABU_ADDR_RECORD_S *stpSrc);

/**
 * @desc join the address record
 * @param stpRecord addr record
 * @param szDst
 * @param uiLen
 * @retval success return 0;fail return -1
 */
int ABU_ADDR_joinRecord(const ABU_ADDR_RECORD_S *stpRecord,
                        char *szDst,
                        const unsigned int uiLen);

/**
 * @desc:free the memory of stpAddrRecord
 */
void ABU_ADDR_destroyRecord(ABU_ADDR_RECORD_S *stpAddrRecord);

/**
 * @desc: parse the addr
 */
ABU_ADDR_S *ABU_ADDR_parse(const ABU_ADDR_LIB_S *stpAddrLib,
                           const char *szAddr);

/**
 * @desc: parse the addr by city
 */
ABU_ADDR_S *ABU_ADDR_parse_bycity(const ABU_ADDR_LIB_S *stpAddrLib,
                                  const char *szAddr,
                                  const char *szCity);

/**
 * @desc:free the memory of stpAddr
 */
void ABU_ADDR_destroy(void *stpAddr);

/** 
 * used for ABU_ADDR_S signature..
 */
void ABU_ADDR_sign(const void *_p, char *p_sign_buf, size_t *p_buf_size);

/**
 * @desc: debug addr record parsing
 */
void ABU_ADDR_printRecord(const ABU_ADDR_RECORD_S *stpAddrRecord,FILE *fp);

/**
 * @desc: debug addr attach
 */
void ABU_ADDR_printAttach(const ABU_ADDR_ATTACH_S *stpAddrAttach, FILE *fp); 

/**
 * @desc: debug addr parsing
 */
void ABU_ADDR_print(const ABU_ADDR_S *stpAddr,FILE *fp);
#endif /*AB_UTILITY_ADDR_H_*/

