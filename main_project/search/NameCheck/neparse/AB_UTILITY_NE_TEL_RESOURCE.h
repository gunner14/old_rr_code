/*********************************************************
*Filename: AB_UTILITY_NE_TEL_RESOUCE.h
*Author:bfzhang
*Date: 2007-02-25
*Version: 0.1
*Modification Log:
*  (1)2007-02-26 create this head file
*Description:
*   including the resource about telephone information recognition
***********************************************************/

#ifndef _AB_UTILITY_NE_TEL_RESOURCE_H
#define _AB_UTILITY_NE_TEL_RESOURCE_H


/**
@desc:国家电话号码代号，去重
*/
extern const char *CountryAreaCode[];

/**
@desc:国家电话号码代号个数，去重
*/
extern const unsigned int COUNTRY_AREA_CODE_NUMBER;

/**
@desc:中国各城市电话号码代号，去重
*/
extern const char *CityAreaCode[];

/***
 * @brief:中国各城市名，按照电话区号排序成一个直接按照区号访问的数组。
 ***/
extern const char *CityByCode[];

/***
 * @brief: get city name by city code from CityByCode
 ***/
const char *getCityByCode(unsigned int code);

/**
@desc:中国各城市电话号码代号个数，去重
*/
extern const unsigned int CITY_AREA_CODE_NUMBER;

#endif
