/**************************************************
*Filename:AB_UTILITY_WORDS_SEG_SL.h
*Author:  bfzhang
*Date:  2007-01-20
*Version:0.3v
*Description:define the public interface about segmentation use single dict
*change log:
*(1) 2007-01-20 create the file
*(2) 2007-03-05 add ABU_WORDS_SEGLIB_S
*Pulbic API;
*Demo:
*	char **szDictArray;//
*   UINT32 dwDictArrayNumber =1;
*	char szBuff[] = "我爱中国";
*   //get dict
*	ABU_WORDS_DICT_S *pBasicDict = ABU_WORDS_DICT_loadDictFromFile(szDictArray,dwDictArrayNumber);
*
*   //get dict lib
*   ABU_WORDS_SEGLIB_S *pSegLib = ABU_WORDS_SEG_createSegLib();
*
*	//get segresult
*	ABU_WORDS_SEGRESULT_SL_S *pSegResult = ABU_WORDS_SEG_SL_getSegResultFromBuff(szBuff,strlen(szBuff),pDict,pSegLib);
*
*	//use pSegResult
*
*	//free segresult
*	ABU_WORDS_SEG_SL_destroySegResult(pSegResult);
*
*	//free dict
*	ABU_WORDS_DICT_destroyDict(pBasicDict);
*
*	//free seglib
*	ABU_WORDS_SEG_destroySegLib(pSegLib);
*******************************************************************************************/
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "HASHSET.h"
#include <stdio.h>

#ifndef AB_UTILITY_WORDS_SEG_SL
#define AB_UTILITY_WORDS_SEG_SL


//the lib: segment needed
typedef struct _ABU_WORDS_SEG_LIB_S
{
	HashSet *pSeparateSet;//4AddrRec
	HashSet *pPunctSet;//4 index
}ABU_WORDS_SEGLIB_S;

/*segmentation result by single dict*/
typedef struct _ABU_WORDS_SEGRESULT_SL_S
{
	//public field
	ABU_WORDS_WORD_S *pWordArray;
	
	UINT32 dwWordNumber;
	
	//private field
	UINT32 dwSizeOfWordArray;
}ABU_WORDS_SEGRESULT_SL_S;


/**ABU_WORDS_SEG: public interface**/

/**
@desc:Max_Matching Segmentation
@param:
	sBuff:欲分词的buffer
	ulLenSBuff: the length of sBuff
	pDict:词典内存索引
	pSegLib:
*/
ABU_WORDS_SEGRESULT_SL_S * ABU_WORDS_SEG_SL_getSegResultFromBuff(const char * sBuff,const UINT64 ulLenSBuff,const ABU_WORDS_DICT_S *pDict,const ABU_WORDS_SEGLIB_S *pSegLib);


/**
@desc:free the content fo pSegResult && free pSegResult
*/
void ABU_WORDS_SEG_SL_destroySegResult(ABU_WORDS_SEGRESULT_SL_S *pSegResult);

/**
@desc:print segement result
*/
void ABU_WORDS_SEG_SL_printSegResult(const ABU_WORDS_SEGRESULT_SL_S *pSegResult,FILE *fp);

/**
@desc:用于对磁盘文件进行分词
@param：
	szDictFile:词典文件数组
	dwDictNumber:要load的词典文件个数
	szInputFile:待切分磁盘文件
	szOutputFile:切分结果输出文件
*return:
*/
void ABU_WORDS_SEG_SL_outputFromFile(const char **szDictFile,const UINT32 dwDictNumber,const char *szInputFile,
								  const char * szOutputFile);

/**
@desc:从分词输出结构体中获取按照基本词词典的切分结果
@param：
@return：切分结果
@Note:内存空间由调用者释放
*/
char * ABU_WORDS_SEG_SL_getSegResult(const ABU_WORDS_SEGRESULT_SL_S *pSegResult);

/**
@desc: create segmentation lib
*/
ABU_WORDS_SEGLIB_S * ABU_WORDS_SEG_createSegLib();

/**
@desc:free memory:destroy segmentation lib
*/
int ABU_WORDS_SEG_destroySegLib(ABU_WORDS_SEGLIB_S *pSegLib);

UINT32 ABU_WORDS_SEG_getMaxMatchLength(const ABU_WORDS_DICT_S *pDict,
									   const char *szBuff,  UINT32 dwHasCovered,
									   char *szWordPosTag,const UINT32 dwWordPosTagLength);
#endif
