/**************************************************
*Filename:AB_UTILITY_WORDS_SEG_DL.h
*Author:  bfzhang
*Date:  2007-01-20
*Version:0.1
*Description:define the public interface about segmentation use basicDict and groupDict
*change log
*(1) 2007-01-20 create the file
*(2) 2007-03-05 add ABU_WORDS_SEGLIB_S
*�ʵ�ִ��߼�
*	(1)���ķִ�ʱ���ڴ�ʵ������н����������ƥ��
*	   �������:һ������ʹ��hash,��������ʹ�ö��ֲ���
*	(2)Ӣ�ĵ���,���,���ֵȽ��е����з�
*	(3)�����ַ���������ʱ,������ַ����ӵ�Ϊ���������ַ����ӵ�������Ϊһ������,��������з�
*�ִ����ܣ�
*	(1)�ִ��ٶȣ�1.4M/s
*	(2)֧�ֶ��߳�
*	(3)�ڴ�ռ���Ǵʵ�����ļ���4-5����С
*Demo:
*	char basicDictFile[] = "/home/bfzhang/basic_dict.txt";
*   char groupDictFile[] = "/home/bfzhang/group_dict.txt";
*	char sBuff[] = "�Ұ��л����񹲺͹�";
*
*	ABU_WORDS_DICT_S *pBasicDict = ABU_WORDS_DICT_loadDictFromFile(szBasicDictFile);
*	ABU_WORDS_DICT_S *pGroupDict = ABU_WORDS_DICT_loadDictFromFile(szGroupDictFile);
*   ABU_WORDS_SEGLIB_S *pSegLib = ABU_WORDS_SEG_createSegLib();
*
*	//��ȡ�ִʽ��
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

/*ʹ�û����ʴʵ�ʹ���ʵ��ȡ���зֽ��*/
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
	sBuff:���ִʵ�buffer
	ulLenSBuff:buff ����
	pBasicDict:�������ڴ�����
	pGroupDict:�����ڴ�����
@return:
	������Ϊ�ִʽṹ��
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
@desc:���ڶԴ����ļ����зִ�
@param��
	 szDictFile:�ʵ�·������
	 dwSmallDictNumber:����С�ʵ�ʹ�õĴʵ���Ŀ��szDictFile[0]->szDictFile[dwSmallDictNumber]
	 dwBigDictNumber��������ʵ�ʹ�õĴʵ���Ŀ,szDictFile[0]->szDictFile[dwBigDictNumber];
	 szInputFile�����ִ��ļ�·��
	 szBasicOutputFile��ʹ�û����ʵ��з֣������ļ�·��
	 szGroupOutputFile��ʹ�ô���ʵ��з֣������ļ�·��
*return:
*/
void ABU_WORDS_SEG_DL_outputFromFile(const char **szDictFile,const UINT32 dwSmallDictNumber,
									 const UINT32 dwBigDictNumber,const char *szInputFile,  
									 const char * szBasicOutputFile,const char *szGroupOutputFile);
									 
void ABU_WORDS_SEG_DL_printSegResult(const ABU_WORDS_SEGRESULT_DL_S *pSegResult,FILE * fp);

#endif
