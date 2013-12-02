/******************************************************
#Filename:HASHMAP.h
*Author:bfzhang
*DatE:2007-02-12
*Description: define the public interface of a simple hash map
*Note:
*   HashMap format: <key value> pair
	Two keys have same hashSign,but they may be different!!!
	key具有唯一性，不同的key可能对应相同的value
******************************************************/

#ifndef HASHMAP_H
#define HASHMAP_H


#ifndef AB_FREE
#define AB_FREE(p) if(p) { free(p); p = NULL;} 
#endif 
typedef struct _HashMapNode
{
	char *key;
	char *value;
	struct _HashMapNode * next;
}HashMapNode;
typedef HashMapNode * HashMapNodePtr;
typedef struct _HashMap
{
	HashMapNodePtr pBucketArray;
	unsigned int dwBucketSize;
	unsigned int dwNumberHashMapNode;
}HashMap;
typedef HashMap * HashMapPtr;

/**
@name:
@desc:create hashmap
@return:
	fail return NULL
*/
HashMapPtr createHashMap(const unsigned int dwBucketSize);

/**
@name:
@desc: destroy hashmap
@return
	succeed return 0
	fail    return -1
*/
int destroyHashMap(HashMapPtr pHashMap);
/**
@name:
@desc:insert (key,value) pair
	  桶内插入复杂度为线性复杂度
@param:
@return:
	succeed return 0;
	fail and (key,value) pair has  been in hashmap return -1
*/
int insertHashMapNode(HashMap * pHashMap,const char *key,const char *value);
/**
@name:searchHashMapNode
@desc:桶内查找复杂度为线性
@return:
	   succeed return 0
	   fail    return -1
*/
int searchHashMapNode(const HashMap * pHashMap,const char *key,const char *value);

/**
@name: changeHashMapNode
@desc: reset the value of the key
@return: 
	succeed return 0
	fail return -1
*/
int changeHashMapNode(HashMap *pHashMap,const char *szKey,const char *szNewValue);
/**
@name:removeHashMapNode
@desc:
@return 
	succeed return 0
	fail  return -1;
	if( <key,value> doesn't exist in hashmap) removeHashMap(hashMap,key,value) == -1
*/
int removeHashMapNode(HashMap * pHashMap,const char *key,const char *value);

/**
@name:getValue
@desc:get value by key
@return:
*/
const char * getValueHashMap(const HashMap *pHashMap,const char *key);

/**
@name:printHashMap
@desc: print <key,value> pair
*/
void printHashMap(const HashMap * pHashMap);

/**
@name:printHashMapToFile
@desc:output <key,value>pair to file
	  openMode "w" "w+"
**/
void printHashMapToFile(const HashMap * pHashMap,const char *outFile,const char *openMode);

/**
@name: getHashMapSign
@desc: a simple method to get hash sign
@Note: same hashSign, but the keys may be not the same
*/
unsigned int getHashMapSign(const char *key,const unsigned int dwKeyLen,const unsigned int dwBucketSize);

#endif
