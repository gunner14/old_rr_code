/****************************************************************************
*Filename: AB_UTILITY_NE_TEL.h
*
*Version: 0.1v
*Date:   2007-02-12
*Description: define the public interface of Tel informatin recognation
*Resource:
		telephone list
*Node:��ǰ�汾��ҪΪ���ĵ绰��Ϣʶ��
      ���ڵ绰��ʶ��ʹ��Ȩֵ�ۼ�
*****************************************************************************/
#include "HASHMAP.h"
#include "HASHSET.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"
#include <stdio.h>
#include <string.h>

#ifndef AB_UTILITY_NE_TEL
#define  AB_UTILITY_NE_TEL

#ifndef AB_UTILITY_NE_TEL_DEBUG
#define  AB_UTILITY_NE_TEL_DEBUG
#endif

#ifndef AB_FREE
#define AB_FREE(p) if(p){ free(p); p = NULL;}
#endif

typedef struct _ABU_NE_TEL_UNIT_S
{
	char *szFullTel;
	char *szCountryAreaCode;
	char *szCityAreaCode;
	char *szTelNumber;//���Ϊ�ֻ�ֻ�е绰����
	char *szExtNumber;
	UINT32 dwWeight;//����Ϊ100�����ʱ���Ը�����ֵ
}ABU_NE_TEL_UNIT_S;
typedef struct _ABU_NE_TEL_RECRESULT_S
{
	/*public field*/
	ABU_NE_TEL_UNIT_S *pNeUnitArray;//����Ȩֵ���н�������
	UINT32 dwNumberTelList;//���������ĵ绰������Ŀ

	/*private field*/
	UINT32 dwSizeOfTelList;//memory count
	UINT32 *dwReservered;
}ABU_NE_TEL_RECRESULT_S;

/*�绰��Ϣʶ����Ҫ��lib*/
typedef struct _ABU_NE_TEL_LIB_S
{
	HashSet *pTelCountryAreaCodeHashSet;
	HashSet *pTelCityAreaCodeHashSet;
	HashSet *pTelPreWordHashSet;
	HashSet *pTelSpecWordHashSet;
	HashSet *pTelPostWordHashSet;
	HashSet *pMobNumSegHashSet;

	//weight rule lib
	HashMap *pRuleWeightHashMap;
}ABU_NE_TEL_LIB_S;

/***
    @brief test wheter a phone number is a mobile phone number
    @return -1 means error, 0 means not a mobile phone number, 1 means a mobile phone number
 ***/
int ABU_NE_isMobile(const char *phone, const ABU_NE_TEL_LIB_S *pTelRecLib);


/**
@Note: the core of telephone recognition module
@name:getTel_SL
@desc:get Tel number list from SegResult
@param:
@return:
*/
ABU_NE_TEL_RECRESULT_S * ABU_NE_TEL_getRecResult_SL(const ABU_WORDS_SEGRESULT_SL_S *pSegResult,const ABU_NE_TEL_LIB_S *pTelRecLib);


/**
@name:destroyNeTel
@desc:free memory
@param:
@return:
     succeed return 0
	 fail    return -1
*/
int ABU_NE_TEL_destroyRecResult(ABU_NE_TEL_RECRESULT_S *pTelRecResult);


/**
@name:ABU_NE_TEL_printNeTel
@desc:
	dwWeighBegin >= 0:�������ʼ��ֵ
*/
void ABU_NE_TEL_printRecResult(ABU_NE_TEL_RECRESULT_S *pTelRecResult,UINT32 dwWeightBegin);

/**
@name ABU_NE_TEL_printRecResultToFile
@desc:
*/
void ABU_NE_TEL_printRecResultToFile(const ABU_NE_TEL_RECRESULT_S *pTelRecResult,const UINT32 dwWeightBegin,
									 const char *cpOutputFile); 

/**
@name: ABU_NE_TEL_createRecLib
@desc:
	szCityTelFile format: szCity szCityTelNumber
*/
ABU_NE_TEL_LIB_S *ABU_NE_TEL_createRecLib();

/**
@name:ABU_NE_TEL_destroyRecLib()
@desc:free space
@return
	succeed return 0
	fail    return -1
*/
int ABU_NE_TEL_destroyRecLib(ABU_NE_TEL_LIB_S *pTelRecLib);

/**
@name:ABU_NE_TEL_printRecLibToFile(ABU_NE_TEL_LIB_S * pTelRecLib)
@desc:
	openMode: "w" "w+"
*/
void ABU_NE_TEL_printRecLibToFile(ABU_NE_TEL_LIB_S *pTelRecLib,const char *szOutputFile,const char *openMode);

/***
 * @brief get area code from Name Entity Telephone Unit
 ****/
unsigned int ABU_NE_getAreaCode(const ABU_NE_TEL_UNIT_S * stpNETelUnit);

/***
 * @brief get city name by telephone district code
 * @return string for exist code; NULL for nonexist ones
 ***/
const char *ABU_NE_getCityByCode(unsigned int code);

/***
 * @brief judge a number is valid or not
 * @return 0 for invalid; 1 for valid
 ***/
int ABU_NE_isValidPhone(int areacode, const char *phone);
    
#endif
