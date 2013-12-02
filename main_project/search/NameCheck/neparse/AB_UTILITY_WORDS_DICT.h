/*****************************************************
*Filename:AB_UTILITY_WORDS_DICT.h
*Author:bfzhang
*Date: 2007-01-20
*Version:
*（1）0.1v 2007-01-20 实现中文分词基本功能
*（2）0.2v 2007-02-09 给中文分词具有词性标注功能,词性标注遵循北大标准/中科院标准
*（3）0.3v 2007-03-22 修改bug，增加AB_UTILITY_WORDS_UTILITY.h，以提供外部接口
*Discription: define the public interface of how to build the dictionary for segmentation
*Note:!!!!
*   Dictionary Format:
*		word wordPostag
*		(word与wordPosTag之间用空格隔开,wordPosTag各词性之间用,隔开)
*Note:
*词典文件:
*	(1)词典文件词条保证unique
*词典数据结构:
*	(1)词典一级索引使用哈希表
*	(2)词典二级及其二级以上索引使用Trie树
*词典特性
*	(1)支持纯中文词典 
*	(2)支持纯英文词典
*	(3)支持中英文混合词典
*	(4)支持数字词典
********************************************************/
#include "AB_UTILITY_WORDS.h"
#ifndef AB_UTILITY_WORDS_DICT
#define AB_UTILITY_WORDS_DICT


struct TrieNode /*二级索引*/
{
	char szSingleCharacter[3];/*汉字或英文*/
	UINT16 dwNumberChild;
	BYTE byIsWord;
	char *szWordPosTag;//if(byIsWord == 1) posTag != NULL
	struct TrieNode *pFirstChildArray;
};
typedef struct TrieNode * TrieNodePtr;

 struct HashNode
{
	char szSingleCharacter[3];/*汉字或者英文*/
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
