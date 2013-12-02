/**************************************************
*Filename:AB_UTILITY_WORDS_SEG_DL.h
*Author:  bfzhang
*Date:  2007-01-20
*Version:0.1
*Description:define the public interface about segmentation use basicDict and groupDict
*change log
*(1) 2007-01-20 create the file
*(2) 2007-03-05 add ABU_WORDS_SEGLIB_S
*词典分词逻辑
*	(1)中文分词时在内存词典索引中进行正向最大匹配
*	   具体策略:一级索引使用hash,二级索引使用二分查找
*	(2)英文单词,标点,数字等进行单独切分
*	(3)由连字符进行连接时,如果连字符连接的为数字则将连字符连接的数字视为一个整体,否则进行切分
*分词性能：
*	(1)分词速度：1.4M/s
*	(2)支持多线程
*	(3)内存占用是词典磁盘文件的4-5倍大小
*Demo:
*	char basicDictFile[] = "/home/bfzhang/basic_dict.txt";
*   char groupDictFile[] = "/home/bfzhang/group_dict.txt";
*	char sBuff[] = "我爱中华人民共和国";
*
*	ABU_WORDS_DICT_S *pBasicDict = ABU_WORDS_DICT_loadDictFromFile(szBasicDictFile);
*	ABU_WORDS_DICT_S *pGroupDict = ABU_WORDS_DICT_loadDictFromFile(szGroupDictFile);
*   ABU_WORDS_SEGLIB_S *pSegLib = ABU_WORDS_SEG_createSegLib();
*
*	//获取分词结果
*	ABU_WORDS_SEGRESULT_DL_S *pSegResult = 
*                            ABU_WORDS_SEG_DL_getSegResultFromBuff(sBuff,ulLenSBuff,pBasicDict,pGroupDict);
*
*	//use pSegResult
*
*	
*	ABU_WORDS_SEG_DL_destroySegResult(pSegResult);
*	ABU_WORDS_DICT_destroyDict(pBasicDict);
*   ABU_WORDS_DICT_destroyDict(pGroupDict);
*   ABU_WORDS_SEG_destroySegLib(pSegLib);
*******************************************************************************************/
#include "AB_UTILITY_WORDS.h"
#include "AB_UTILITY_WORDS_DICT.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"

#ifndef AB_UTILITY_WORDS_SEG_DL
#define AB_UTILITY_WORDS_SEG_DL

//#ifndef ABU_WORDS_SEG_DL_DL_DEBUG
//#define ABU_WORDS_SEG_DL_DL_DEBUG
//#endif

/*使用基本词词典和词组词典获取的切分结果*/
typedef struct _ABU_WORDS_SEGRESULT_DL_S
{
	//public field
	ABU_WORDS_SEGRESULT_SL_S *pBasicSegResult;//basic word segment result


	ABU_WORDS_SEGRESULT_SL_S *pGroupSegResult;//group word segment result
	
	//private field
	UINT32 *dwpRerserved; 

}ABU_WORDS_SEGRESULT_DL_S;


/**
@name:ABU_WORDS_SEG_DL_outputFromBuff
@desc:Max_Matching Segmentation
@param:
	sBuff:欲分词的buffer
	ulLenSBuff:buff 长度
	pBasicDict:基本词内存索引
	pGroupDict:词组内存索引
@return:
	输出结果为分词结构体
*/
ABU_WORDS_SEGRESULT_DL_S * ABU_WORDS_SEG_DL_getSegResultFromBuff(const char * sBuff,UINT64 ulLenSBuff,

							    const ABU_WORDS_DICT_S *pBasicDict,const ABU_WORDS_DICT_S *pGroupDict,const ABU_WORDS_SEGLIB_S *pSegLib);
/**
@name:ABU_WORDS_SEG_DL_destroySegResult
@desc: free the content of pSegResult && free pSegResult
@return: succeed 0;fail -1
*/
void ABU_WORDS_SEG_DL_destroySegResult(ABU_WORDS_SEGRESULT_DL_S *pSegResult);

/**
@name:int ABU_WORDS_SEG_DL_outputFromFile(const char * szBasicDictFile,const char *szGroupDictFile,
                          const char *szInputFile,  const char * szBasicOutputFile,
						  const char *szGroupOutputFile)
@desc:用于对磁盘文件进行分词
@param：
	 szDictFile:词典路径数组
	 dwSmallDictNumber:构建小词典使用的词典数目，szDictFile[0]->szDictFile[dwSmallDictNumber]
	 dwBigDictNumber：构建大词典使用的词典数目,szDictFile[0]->szDictFile[dwBigDictNumber];
	 szInputFile：待分词文件路径
	 szBasicOutputFile：使用基本词典切分，输入文件路径
	 szGroupOutputFile：使用词组词典切分，输入文件路径
*return:
*/
void ABU_WORDS_SEG_DL_outputFromFile(const char **szDictFile,const UINT32 dwSmallDictNumber,
									 const UINT32 dwBigDictNumber,const char *szInputFile,  
									 const char * szBasicOutputFile,const char *szGroupOutputFile);
									 
void ABU_WORDS_SEG_DL_printSegResult(const ABU_WORDS_SEGRESULT_DL_S *pSegResult,FILE * fp);

#endif
