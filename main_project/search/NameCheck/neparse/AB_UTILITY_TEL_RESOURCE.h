/******************************************************
 * @Filename AB_UTILITY_TEL_RESOURCE.h
 * @Author bfzhang
 * @Date 2007-07-05
 * @Description  the tel resource
 * @Change Log
 *         $Id: $
 * ****************************************************/
#ifndef AB_UTILITY_TEL_RESOURCE_H_
#define AB_UTILITY_TEL_RESOURCE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern const char CHINA_CODE[ ];

/**
 * @desc coutry area code
 */
extern const uint32_t COUNTRY_CODE_COUNT;
extern const char *COUNTRY_CODE[ ];

/**
 * @desc city area code
 */
extern const uint32_t CITY_CODE_COUNT;
extern const char *CITY_CODE[ ]; 

/**
 * @desc mobile prefix
 */
extern const uint32_t MOBILE_PREFIX_COUNT;
extern const char *MOBILE_PREFIX[ ]; 

/**
 * @desc tel feature word
 */
extern const uint32_t TEL_FEAT_WORD_COUNT;
extern const char *TEL_FEAL_WORD[ ];

/**
 * @desc for tel parsing
 */

/**
 * @desc (country area code) length range
 */
extern const uint32_t MIN_COUNTRY_CODE_LEN;
extern const uint32_t MAX_COUNTRY_CODE_LEN;

/**
 * @desc (city area code length) range
 */
extern const uint32_t MIN_CITY_CODE_LEN;
extern const uint32_t MAX_CITY_CODE_LEN;

/**
 * @desc (telephone number) lengh range
 */
extern const uint32_t MIN_TEL_LEN;
extern const uint32_t MAX_TEL_LEN;

/**
 * @desc (citycode + telephone number) length range
 */
extern const uint32_t MIN_CITYTEL_LEN;
extern const uint32_t MAX_CITYTEL_LEN;

/**
 * @desc (telephone ext number) length range
 */
extern const uint32_t MIN_EXT_LEN;
extern const uint32_t MAX_EXT_LEN;

/**
 * @desc (mobile telephone) length range
 */
extern const uint32_t MOBILE_TEL_LEN_SHORT;
    //86-country area code
extern const uint32_t MOBILE_TEL_LEN_LONG;

extern const uint32_t MOBILE_PREFIX_LEN; 

#endif /*AB_UTILITY_TEL_RESOURCE_H_*/
