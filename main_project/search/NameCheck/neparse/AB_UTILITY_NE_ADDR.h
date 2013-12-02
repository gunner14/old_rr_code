/*********************************************************
*Filename: AB_UTILITY_NE_ADDR.h
*
*Version: 0.1v
*Date:  2007-02-13
*Description: define the public interface of address information recognition
*Note:
***********************************************************/
#include "HASHMAP.h"
#include "HASHSET.h"

#include "AB_UTILITY_WORDS_SEG_SL.h"

#ifndef AB_UTILITY_NE_ADDR
#define AB_UTILITY_NE_ADDR

#ifndef AB_UTILITY_NE_ADDR_DEBUG
#define AB_UTILITY_NE_ADDR_DEBUG
#endif


typedef enum
{
	ADDR_LEVEL_UN = 0x00,

	ADDR_LEVEL_1 = 0x01,//ʡ
	ADDR_LEVEL_2 = 0x02,//��
	ADDR_LEVEL_3 = 0x03,//��
	ADDR_LEVEL_4 = 0x04,//��/��
	ADDR_LEVEL_5 = 0x05,//�ֵ�
	ADDR_LEVEL_6 = 0x06 //��
}ADDR_DETAIL_LEVEL;

//��ַʶ����
typedef struct _ABU_NE_ADDR_UNIT_S
{
	char *szAddr;
	BOOL bIsAddr; //ʹ�õ���ȷ������
	int credWeight;//�������Ŷ�
	ADDR_DETAIL_LEVEL detailLevel;
}ABU_NE_ADDR_UNIT_S;

typedef struct _ABU_NE_ADDR_RECRESULT_S
{
	ABU_NE_ADDR_UNIT_S *pAddrUnitArray;//����Ȩֵ�������򣬼�pAddrUnitArray[0]��weight���
	
	UINT32 dwNumberAddrUnitArray;//the number of recognised addr 

	UINT32 dwSizeOfAddrUnitArray;//for realloc memory
}ABU_NE_ADDR_RECRESULT_S;

//����ʶ��ʹ�õĿ⣨������Դ��͹���⣩
typedef struct _ABU_NE_ADDR_LIB
{
	//���е�ַ�ָ����
	HashSet *pSeparateHashSet;
	/*�й��������ֿ�,Ŀǰ��δʹ��*/
	//HashMap *cpCharStartHashMap;//�й��������ֿ⣬����������ͳ�����й�������
	//HashMap *cpCharMidHashMap;  //�й��������ֿ⣬����������ͳ�����й�������
	//HashMap *cpCharEndHashMap;  //�й��������ֿ⣬β��������ͳ�����й�������

	/*�й������ôʿ⣬��Ϊ��չ,����ʹ��**/
	//HashMap *cpWordStartHashMap;
	//HashMap *cpWordMidHashMap;
	//HashMap *cpWordEndHashMap;

	/*�й�������Ԫͬ�ֿ⣬��δ���壬������չ*/

	
	/*�й��������й�ϵ�ʿ⣬��Ϊ��չ,Ŀǰ����ʹ��*/
	//HashSet *addrCoordWordHashSet;

	//�������Ȩֵ�����
	HashMap *addrRuleWeightHashMap;

	//��ҳ�ṹ����ַ��Ϣָʾ�ʣ�"��ַ" && "����"
	HashSet *addrStruWordHashSet;
	HashSet *addStruWordSuffHashSet;

	//�й�����ָʾ�ʿ⣬�������������ͬ���ֵĵ���ָʾ��
	HashSet *addrIndiWordHashSet;

	//ר�����ʴʿ⣬���ꡱ�����ݡ���������
	HashSet *addrProperNounHashSet;

	//�й����������ʿ⣬����������ʹ�õĺ�׺���硰�����·��
	HashSet *addrFeatWordHashSet;

	//��������
	HashSet *addrSuffixWordHashSet;

	//HashSet *addrRegExpHashSet; //�й�����������ʽ�⣬���жϸ��ӵ����ǿ���ʹ��

	
}ABU_NE_ADDR_LIB_S;

/**
@name:ABU_NE_ADDR_getRecResult_SL
@desc: get address information recognition result
@param:
*/
ABU_NE_ADDR_RECRESULT_S *ABU_NE_ADDR_getRecResult_SL(ABU_WORDS_SEGRESULT_SL_S *pSegResult,
													 ABU_NE_ADDR_LIB_S *pAddrRecLib);

/**
@name:ABU_NE_ADDR_destroyRecResult
@desc:free memory of ABU_NE_ADDR_RECRESULT_S
@return:
	succeed return 0
	fail    return -1
*/
int ABU_NE_ADDR_destroyRecResult(ABU_NE_ADDR_RECRESULT_S *pAddrRecResult);

/**
@name:ABU_NE_ADDR_createRecLib
@desc:create ABU_NE_ADDR_LIB_S for recognition
@param:
	  Ŀǰ��ʱֻʹ�������ļ�
@return 
*/
ABU_NE_ADDR_LIB_S *ABU_NE_ADDR_createRecLib();

/**
@name:ABU_NE_ADDR_destroyRecLib
@desc:destroy ABU_NE_ADDR_LIB_S
@param:
@return:
*/
int ABU_NE_ADDR_destroyRecLib(ABU_NE_ADDR_LIB_S *pAddrRecLib);


/**
@name:ABU_NE_ADDR_printRecLibToFile
@desc:
@param:
	openMode: "w","w+","a+"
*/
void ABU_NE_ADDR_printRecLibToFile(const ABU_NE_ADDR_LIB_S *pAddrRecLib,
								   const char *szOutputFile,const char *openMode);

/**
@name:ABU_NE_ADDR_printRecResult
@desc:
@param:
*/
void ABU_NE_ADDR_printRecResult(const ABU_NE_ADDR_RECRESULT_S *pRecResult,const int credWeightBegin,
								const ADDR_DETAIL_LEVEL detailLevelBegin);

/**
@name:ABU_NE_ADDR_printRecResultToFile
@desc:
*/
void ABU_NE_ADDR_printRecResultToFile(const ABU_NE_ADDR_RECRESULT_S *pRecResult,const int credWeightBegin,
									  const ADDR_DETAIL_LEVEL detailLevelBegin, const char *cpOutputFile);

/****************************************************************
 *@brief create city and province map to get province by city
 *
 ***************************************************************/
HashMap *ABU_NE_createCityProvinceMap();

/****************************************************************
 *@brief create city and area code map to get code by city
 *
 ***************************************************************/
HashMap *ABU_NE_createCityCodeMap();

/****************************************************************
 *@brief get normalized name for a city 
 *@return normalized name for recognized city name;
 *        NULL for unknown city name
 ***************************************************************/
const char *ABU_NE_getRegularCityName(const char *name);

#endif
