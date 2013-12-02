/******************************************************
#Filename:HASHSET.h
*Author:bfzhang
*DatE:2007-02-12
*Description: define the public interface of a simple hash map
*Note:
*   HashSet format: <key value> pair
	Two keys have same hashSign,but they may be different!!!
******************************************************/
#include <stdio.h>
#include <stdlib.h>

#ifndef HASHSET_H
#define HASHSET_H


typedef struct _HashSetNode
{
	char *key;
	char *value;
	struct _HashSetNode * next;
}HashSetNode;
typedef HashSetNode * HashSetNodePtr;
typedef struct _HashSet
{
	HashSetNodePtr pBucketArray;
	unsigned int dwBucketSize;
	unsigned int dwNumberHashSetNode;
}HashSet;
typedef HashSet * HashSetPtr;

/**
@name:
@desc:create hashmap
@return:
	fail return NULL
*/
HashSetPtr createHashSet(const unsigned int dwBucketSize);

/**
@name:
@desc: destroy hashmap
@return
	succeed return 0
	fail    return -1
*/
int destroyHashSet(HashSetPtr pHashSet);
/**
@name:
@desc:insert (key,value) pair
	  桶内插入复杂度为线性复杂度
@param:
@return:
	succeed return 0;
	fail and (key,value) pair has  been in hashmap return -1
*/
int insertHashSetNode(HashSet * pHashSet,const char *key);
/**
@name:searchHashSetNode
@desc:桶内查找复杂度为线性
@return:
	   succeed return 0
	   fail    return -1
*/
int searchHashSetNode(const HashSet * pHashSet,const char *key);

/**
@name:removeHashSetNode
@desc:
@return 
	succeed return 0
	fail  return -1;
	if( <key,value> doesn't exist in hashmap) removeHashSet(hashMap,key,value) == -1
*/
int removeHashSetNode(HashSet * pHashSet,const char *key);

/**
@name:printHashSet
@desc: print <key,value> pair
*/
void printHashSet(const HashSet * pHashSet);

/**
@name:printHashSetToFile
@desc:output <key,value>pair to file
	  openMode: "w" "w+"
**/
void printHashSetToFile(const HashSet * pHashSet,const char *outFile,const char *openMode);

/**
@name: getHashSetSign
@desc: a simple method to get hash sign
@Note: same hashSign, but the keys may be not the same
*/
unsigned int getHashSetSign(const char *key,const unsigned int dwKeyLen,const unsigned int dwBucketSize);

#endif
