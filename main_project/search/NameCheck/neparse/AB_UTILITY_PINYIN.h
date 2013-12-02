/**************************************************************
 * Filename: AB_UTILITY_PINYIN.h
 * 
 * Date: 2007-06-13
 * Description: define the data structure and pubic interface of AB_UTILITY_MODULE
 * 			  define two sets api(HANZI->PINYIN,PINYIN->HANZI)
 * Naming: we think that HANZI is more notable than CHINESE CHARACTER
 * This version: velocity
 * (1) ABU_PINYIN_getPinyin 100m->2s
 * (2) ABU_PINYIN_isSamePinyin 100m->10s
 * chang log
 * (1) create the .h file in 2007-06-13
 * ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_HASH.h"


#ifndef AB_UTILITY_PINYIN_H_
#define AB_UTILITY_PINYIN_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define HANZIMAP_SIZE 65536
#define HANZI_LEN 2 //2 bytes
#define TUNE_NUMBER 5//一声，二声，三声，四声，轻声

typedef struct _ABU_HANZINODE_S
{
	//public field
	char szHanzi[HANZI_LEN + 1];
	
	char **szPinyinArray;//corresponding pinyin array,dynamic memory
	
	UINT32 *pinyinHashSign;//ABU_HASH_create32bSign
	
	unsigned char (*pinyinTuneArray)[ TUNE_NUMBER];//array pointer not pointer array
	
	UINT32 pinyinArrayCount;
	
	//private field
	struct _ABU_HANZINODE_S *stpNext;
	
}ABU_HANZINODE_S;

/* this the simple hashmap->array*/
typedef struct _ABU_HANZIMAP_S 
{
	//public field
	ABU_HANZINODE_S stpBucketNodeArray[HANZIMAP_SIZE];
	UINT32 bucketCount;
	
}ABU_HANZIMAP_S;

typedef struct _ABU_PINYINNODE_S
{
	//public field
	char *szPinyin;
	
	char (*szHanziArray)[HANZI_LEN + 1];//array pointer
	
	unsigned char (*pinyinTuneArray)[TUNE_NUMBER];
	
	UINT32 hanziArrayCount;
	
	//private field
	UINT32 keyHashSign;
	UINT32 posHashSign;
	
	struct _ABU_PINYINNODE_S *stpNext;
	
}ABU_PINYINNODE_S;

typedef struct _ABU_PINYINMAP_S
{
	//public field
	ABU_PINYINNODE_S *stpBucketNodeArray;
	UINT32 bucketCount;
	
	//private field
	UINT32 privateBucketSize;
}ABU_PINYINMAP_S;

/*@desc: create the hanzimap
 * please use ABU_PINYIN_destroyHanziMap to free the memory
 * @return: fail return NULL
 */
ABU_HANZIMAP_S *ABU_PINYIN_createHanziMap();

/*@desc: just return the pointer
 * @return: fail return NULL
 */
static const ABU_HANZINODE_S * ABU_PINYIN_getPinyin(const ABU_HANZIMAP_S *stpHanziMap,const char *szHanzi)
{
	ABU_ASSERT(stpHanziMap != NULL && szHanzi != NULL);
	
	if(szHanzi[0]  >= 0 || strlen(szHanzi) != HANZI_LEN) return NULL;
		
	UINT32 keyHashSign = (unsigned char)szHanzi[0];
	keyHashSign = ( keyHashSign << 8 ) | (unsigned char)szHanzi[ 1 ];
		
	
	if( (stpHanziMap->stpBucketNodeArray + keyHashSign)->szHanzi != NULL)
	{
		return (const ABU_HANZINODE_S *)(stpHanziMap->stpBucketNodeArray + keyHashSign);
	}
	else
	{
		return NULL;
	}
}
BOOL ABU_PINYIN_isSamePinyin(const ABU_HANZIMAP_S *stpHanziMap,const char *szHanziLeft,const char *szHanziRight);

void ABU_PINYIN_destroyHanziMap(ABU_HANZIMAP_S *stpHanziMap);

/*
 * @desc: display the hanziMap for debug 
 */
void ABU_PINYIN_displayHanziMap(const ABU_HANZIMAP_S *stpHanziMap,FILE *fpOutput);

/*
 * @desc: creat the pinyin map
 *  	 please use ABU_PINYIN_destroyPinyinMap to free the memory
 * @return:fail return NULL;
 */
ABU_PINYINMAP_S *ABU_PINYIN_createPinyinMap();

const ABU_PINYINNODE_S * ABU_PINYIN_getHanzi(const ABU_PINYINMAP_S *stpPinyinMap,const char *szPinyin);

void ABU_PINYIN_destroyPinyinMap(ABU_PINYINMAP_S *stpPinyinMap);

/*
 * @desc: display the pinyinMap for debug
 */
 void ABU_PINYIN_displayPinyinMap(const ABU_PINYINMAP_S *stpPinyinMap,FILE *fpOutput);
 
#endif /*AB_UTILITY_PINYIN_H_*/


