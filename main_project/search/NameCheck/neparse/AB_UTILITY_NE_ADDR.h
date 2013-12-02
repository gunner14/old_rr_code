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

	ADDR_LEVEL_1 = 0x01,//省
	ADDR_LEVEL_2 = 0x02,//市
	ADDR_LEVEL_3 = 0x03,//县
	ADDR_LEVEL_4 = 0x04,//乡/镇
	ADDR_LEVEL_5 = 0x05,//街道
	ADDR_LEVEL_6 = 0x06 //号
}ADDR_DETAIL_LEVEL;

//地址识别结果
typedef struct _ABU_NE_ADDR_UNIT_S
{
	char *szAddr;
	BOOL bIsAddr; //使用地名确定规则
	int credWeight;//地名可信度
	ADDR_DETAIL_LEVEL detailLevel;
}ABU_NE_ADDR_UNIT_S;

typedef struct _ABU_NE_ADDR_RECRESULT_S
{
	ABU_NE_ADDR_UNIT_S *pAddrUnitArray;//按照权值降序排序，即pAddrUnitArray[0]的weight最高
	
	UINT32 dwNumberAddrUnitArray;//the number of recognised addr 

	UINT32 dwSizeOfAddrUnitArray;//for realloc memory
}ABU_NE_ADDR_RECRESULT_S;

//地名识别使用的库（包括资源库和规则库）
typedef struct _ABU_NE_ADDR_LIB
{
	//单列地址分割符号
	HashSet *pSeparateHashSet;
	/*中国地名用字库,目前尚未使用*/
	//HashMap *cpCharStartHashMap;//中国地名用字库，首字字数，统计自中国地名库
	//HashMap *cpCharMidHashMap;  //中国地名用字库，中字字数，统计自中国地名库
	//HashMap *cpCharEndHashMap;  //中国地名用字库，尾字字数，统计自中国地名库

	/*中国地名用词库，作为扩展,保留使用**/
	//HashMap *cpWordStartHashMap;
	//HashMap *cpWordMidHashMap;
	//HashMap *cpWordEndHashMap;

	/*中国地名二元同现库，尚未定义，留待扩展*/

	
	/*中国地名并列关系词库，作为扩展,目前保留使用*/
	//HashSet *addrCoordWordHashSet;

	//各类规则权值定义库
	HashMap *addrRuleWeightHashMap;

	//网页结构化地址信息指示词，"地址" && "座落"
	HashSet *addrStruWordHashSet;
	HashSet *addStruWordSuffHashSet;

	//中国地名指示词库，即经常与地名共同出现的地名指示词
	HashSet *addrIndiWordHashSet;

	//专有名词词库，“店”，“馆”，“厅”
	HashSet *addrProperNounHashSet;

	//中国地名特征词库，即地名经常使用的后缀，如“里”，“路”
	HashSet *addrFeatWordHashSet;

	//地名结束
	HashSet *addrSuffixWordHashSet;

	//HashSet *addrRegExpHashSet; //中国地名正则表达式库，在判断复杂地名是可能使用

	
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
	  目前暂时只使用两个文件
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
