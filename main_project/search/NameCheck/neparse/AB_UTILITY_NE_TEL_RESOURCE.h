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
@desc:���ҵ绰������ţ�ȥ��
*/
extern const char *CountryAreaCode[];

/**
@desc:���ҵ绰������Ÿ�����ȥ��
*/
extern const unsigned int COUNTRY_AREA_CODE_NUMBER;

/**
@desc:�й������е绰������ţ�ȥ��
*/
extern const char *CityAreaCode[];

/***
 * @brief:�й��������������յ绰���������һ��ֱ�Ӱ������ŷ��ʵ����顣
 ***/
extern const char *CityByCode[];

/***
 * @brief: get city name by city code from CityByCode
 ***/
const char *getCityByCode(unsigned int code);

/**
@desc:�й������е绰������Ÿ�����ȥ��
*/
extern const unsigned int CITY_AREA_CODE_NUMBER;

#endif
