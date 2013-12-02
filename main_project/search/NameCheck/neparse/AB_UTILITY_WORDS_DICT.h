/*****************************************************
*Filename:AB_UTILITY_WORDS_DICT.h
*Author:bfzhang
*Date: 2007-01-20
*Version:
*��1��0.1v 2007-01-20 ʵ�����ķִʻ�������
*��2��0.2v 2007-02-09 �����ķִʾ��д��Ա�ע����,���Ա�ע��ѭ�����׼/�п�Ժ��׼
*��3��0.3v 2007-03-22 �޸�bug������AB_UTILITY_WORDS_UTILITY.h�����ṩ�ⲿ�ӿ�
*Discription: define the public interface of how to build the dictionary for segmentation
*Note:!!!!
*   Dictionary Format:
*		word wordPostag
*		(word��wordPosTag֮���ÿո����,wordPosTag������֮����,����)
*Note:
*�ʵ��ļ�:
*	(1)�ʵ��ļ�������֤unique
*�ʵ����ݽṹ:
*	(1)�ʵ�һ������ʹ�ù�ϣ��
*	(2)�ʵ�������������������ʹ��Trie��
*�ʵ�����
*	(1)֧�ִ����Ĵʵ� 
*	(2)֧�ִ�Ӣ�Ĵʵ�
*	(3)֧����Ӣ�Ļ�ϴʵ�
*	(4)֧�����ִʵ�
********************************************************/
#include "AB_UTILITY_WORDS.h"
#ifndef AB_UTILITY_WORDS_DICT
#define AB_UTILITY_WORDS_DICT


struct TrieNode /*��������*/
{
	char szSingleCharacter[3];/*���ֻ�Ӣ��*/
	UINT16 dwNumberChild;
	BYTE byIsWord;
	char *szWordPosTag;//if(byIsWord == 1) posTag != NULL
	struct TrieNode *pFirstChildArray;
};
typedef struct TrieNode * TrieNodePtr;

 struct HashNode
{
	char szSingleCharacter[3];/*���ֻ���Ӣ��*/
	UINT16 dwNumberEntry;	
	BYTE byIsWord; 
	char *szWordPosTag;
	struct TrieNode *pFirstTrieNodeArray; 
	struct HashNode *pNext;
};
typedef struct HashNode * HashNodePtr;

typedef struct _ABU_WORDS_DICT_S
{
//###	pthread_mutex_t dictMutexLock;
	HashNodePtr pGeneralHashTable[BUCKET_SIZE];
	UINT32 *dwpRerverved; 	
}ABU_WORDS_DICT_S;

/*
 * @desc: load  segment dict  from file
 * @return: fail return NULL;
 */
ABU_WORDS_DICT_S* ABU_WORDS_DICT_loadDictFromFile(const char **szDictFile,const UINT32 dwDictNumber);

/*
*@desc: load segment dict 
*@return: fail return NULL;
*/
ABU_WORDS_DICT_S *ABU_WORDS_DICT_loadDictFromArray(const char **szWordItemArray,const UINT32 dwWordItemNumber);

/**
@desc:search a word in Dict
	  use hash in first layer index,use binary search in other layers' index
@Notice:
	szWord: must be preprocessed by ABU_WORDS_PREPROCESS_all
@return:
		succeed return 0
		fail    return -1
*/
int ABU_WORDS_DICT_searchWord(const ABU_WORDS_DICT_S *pDict,const char *szWord);

/**
@desc:search a word in dict
	use hash in first layer index,use binary search in other layers' index
@Notice: 
	szWord:must be preprocessed by ABU_WORDS_PREPROCESS_all
@return:
	succeed: return the posTag of szWord
	fail: return -1
*/
const char* ABU_WORDS_DICT_getPosTag(const ABU_WORDS_DICT_S *pDict,const char *szWord);

/**
@desc: free the content of pDict && free pDict
*/
void ABU_WORDS_DICT_destroyDict(ABU_WORDS_DICT_S * pDict);

/**
@desc: ouput pDict
*/
int ABU_WORDS_DICT_outputDict(ABU_WORDS_DICT_S *pDict, char *szOutputDictFile);


/***private interface**********/
HashNodePtr ABU_WORDS_DICT_getHashNodePtr(const ABU_WORDS_DICT_S * pDict,const char *szWord,const UINT32 dwHasCovered);

TrieNodePtr ABU_WORDS_DICT_getNextTrieNodePtrByHashNodePtr(const HashNodePtr pCurrHashNodePtr,const char * szWord,
														   const UINT32 dwHasCovered);
TrieNodePtr ABU_WORDS_DICT_getNextTrieNodePtrByTrieNodePtr(const TrieNodePtr pCurrTrieNodePtr,const char *szWord,
														   const UINT32 dwHasCovered);
#endif
