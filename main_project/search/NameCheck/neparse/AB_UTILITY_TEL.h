/*****************************************************
 * @Filename AB_UTILITY_TEL.h
 * @Author bfzhang
 * @Date 2007-07-05
 * @Description parse the tel(standard format)
 * @Note: the tel parser support following format
 *         format 1;    tel                    81863566
 *         format 2:        city-tel                0539-5262416
 *         format 3:    city tel                0539 5262416
 *         foramt 4:        citytel                01081863566 
 *         format 5:        tel-ext                5262416-0414
 *         format 6:    tel ext                5262416 5623
 *         format 7:        city-tel-ext             0539-52622416-5678 
 *         format 8:    city-tel ext            010-65783421 4567
 *         format 9:    city tel-ext             010 65783421-4567
 *         format 11:    citytel ext            01084562134 6578
 *         format 12:    city tel ext             010 81863566 5623
 *         format 13:        country-city-tel        86-0539-5262416
 *         format 14:        country-city-tel-ext    86-0539-5262416-5678
 *         format 15:        mobile                13581863566
 *         format 16:        countrymobile        8613581863566 * 
 * @Change log:$Id: $
 * **************************************************/
 
#ifndef AB_UTILITY_TEL_H_
#define AB_UTILITY_TEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "HASHSET.h"
#include "HASHMAP.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include "AB_UTILITY_TEL_RESOURCE.h"

/**
 * @desc telephone structured information
 */
typedef struct _ABU_TEL_ATTACH_S
{
    //origin tel
	uint32_t originTelSign;
	
    //country area code
	uint32_t countryCodeSign;
	
    //city area code
	uint32_t cityCodeSign;

    uint16_t telLen;
}ABU_TEL_ATTACH_S;


typedef struct _ABU_TEL_RECORD_S
{
    //country area code
    char *szCountryCode;
    
    //city area code
    char *szCityCode;
    
    //telephone number
    char *szTel;
    
    //telephone extension number
    char *szExt;
}ABU_TEL_RECORD_S;

/**
 * @desc tel parsing reslut
 */
typedef struct _ABU_TEL_S
{
    //origin tel
    char *szOriginTel;
    
    //format tel,concatenated by '-'
    char *szFormatTel;
    
    //telephone structural information
    ABU_TEL_RECORD_S *stpTelRecord;

    ABU_TEL_ATTACH_S *stpTelAttach;
    
    //credit weight
    //int iCredWeight;
}ABU_TEL_S;

typedef struct _ABU_TEL_LIB_S
{    
    //coutry area code set
    HashSet *stpCountryCodeSet;
    
    //city area code set
    HashSet *stpCityCodeSet;
    
    //mobile prefix set
    HashSet *stpMobPrefixSet;
}ABU_TEL_LIB_S;

/**
 * @desc create lib for tel parsing
 */
ABU_TEL_LIB_S *ABU_TEL_createLib();
 

/**
 * @desc destroy lib
 * @param stpTelLib for tel parsing
 */
void ABU_TEL_destroyLib(ABU_TEL_LIB_S *stpTelLib);

/**
 * @desc parse tel to get structured information
 */
ABU_TEL_RECORD_S *ABU_TEL_parseRecord(const char *szTelBuf,
                                      const ABU_TEL_LIB_S *stpTelLib);

/**
 * @desc deep copy the tel record
 */
void ABU_TEL_copyRecord(ABU_TEL_RECORD_S *stpDst, 
                        const ABU_TEL_RECORD_S *stpSrc);

/**
 * @desc destroy structured record
 */
void ABU_TEL_destroyRecord(ABU_TEL_RECORD_S *stpTelRecord);

/**
 * @desc:parse the tel
 */
ABU_TEL_S *ABU_TEL_parse(const char *szTelBuf,const ABU_TEL_LIB_S *stpTelLib);


/**
 * @desc copy ABU_TEL_S
 */
void ABU_TEL_copy(ABU_TEL_S *stpDst,const ABU_TEL_S *stpSrc);

/**
 * @desc: destroy the tel parsing result
 */
void ABU_TEL_destroy(void *stpTel);

/** 
 * used for ABU_ADDR_S signature..
 */
void ABU_TEL_sign(const void *_p, char *p_sign_buf, size_t *p_buf_size);

/**
 * @desc: print tel record
 */
void ABU_TEL_printRecord(const ABU_TEL_RECORD_S *stpTelRecord,FILE *fp);

/**
 * @desc:print tel
 */
void ABU_TEL_print(const ABU_TEL_S *stpTel,FILE *fp);
#endif /*AB_UTILITY_TEL_H_*/

