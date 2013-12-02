/**
*
*$Id: AB_UTILITY_DICT.h,v 1.7 2007/03/10 01:59:55 yhwu Exp $
*
*/
#ifndef _AB_UTILITY_H_
#define _AB_UTILITY_H_
#include "AB_UTILITY_COMMON.h"

/**
 *
 * ABU_DICT
 */

/**
 * Customized Implementation of ABU_GENERALDB_DICT for Link DB.
 * Structure Declaration.
 */
typedef struct ABU_LINKDB_DICT_BLOCKINODE
{   
    UINT32  urlKey1;
    UINT32  urlKey2;
    UINT32  dwLinkBlockPos;
    UINT8   linkStatus; 
    UINT32  dwSiteSign;
}ABU_LINKDB_DICT_BLOCKINODE_S;

typedef struct ABU_LINKDB_DICT_BLOCK{
    ABU_LINKDB_DICT_BLOCKINODE_S* BlockNodeArray;
    struct ABU_LINKDB_DICT_BLOCK* next;
}ABU_LINKDB_DICT_BLOCK_S;

typedef struct ABU_LINKDB_DICT_BUCKETINODE{
    UINT32  urlKey1;
    UINT32  urlKey2;
    UINT32  dwLinkBlockPos;
    UINT8   linkStatus;
    UINT32  dwSiteSign;
    ABU_LINKDB_DICT_BLOCK_S* next;
}ABU_LINKDB_DICT_BUCKETINODE_S;

/**
 *
 *
 * dbFileMetaArray stores the corresponding fd.
 * And the upper bound of file number it sets is 256.
 */
typedef struct ABU_LINKDB_DICT{
    UINT32  dwNumOfDictBuckets; 
    UINT32  dwNumOfNodes;
    UINT8   sizeOfBlock;
    
    ABU_LINKDB_DICT_BUCKETINODE_S* LinkDB_Dict_BucketNodeArray; 
}ABU_LINKDB_DICT_S;

/**
 * ABU_LINKDB_DICT Methods
 * For LINKDB Useage.
 */

/**
 * For LinkDB, create() method scan the harddisk data file 
 * to create the LinkDB memory dictionary.
 *
 */
ABU_LINKDB_DICT_S * ABU_LINKDB_DICT_create(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock); 

// put,get,mode & del
bool ABU_LINKDB_DICT_put(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2, UINT32 dwLinkBlockPos, UINT8 linkStatus, UINT32 dwSiteSign); 

bool ABU_LINKDB_DICT_get(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2, ABU_LINKDB_DICT_BLOCKINODE_S* tempBlockNode); 

bool ABU_LINKDB_DICT_mod(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2, const ABU_LINKDB_DICT_BLOCKINODE_S* tempBlockNode); 

bool ABU_LINKDB_DICT_del(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2);

void ABU_LINKDB_DICT_free(ABU_LINKDB_DICT_S * p);


/**
 * Customized Implementation of ABU_GENERALDB_DICT for Web DB.
 */

//webdb_dict_block structure
typedef struct ABU_WEBDB_DICT_BLOCKINODE
{   
    UINT32  urlKey1;
    UINT32  urlKey2;
    UINT32  dwWebBlockIdx1;
    UINT32  dwWebBlockIdx2;
    UINT32  dwSiteSign;
}ABU_WEBDB_DICT_BLOCKINODE_S;

//webdb_dict_block structure
typedef struct ABU_WEBDB_DICT_BLOCK{
    ABU_WEBDB_DICT_BLOCKINODE_S* BlockNodeArray;
    struct ABU_WEBDB_DICT_BLOCK* next;
}ABU_WEBDB_DICT_BLOCK_S;

//webdb_dict_bucket_node structure
typedef struct ABU_WEBDB_DICT_BUCKETINODE{
    UINT32  urlKey1;
    UINT32  urlKey2;
    UINT32  dwWebBlockIdx1;
    UINT32  dwWebBlockIdx2;
    UINT32  dwSiteSign;
    ABU_WEBDB_DICT_BLOCK_S* next;
}ABU_WEBDB_DICT_BUCKETINODE_S;

//webdb_dict structure
typedef struct ABU_WEBDB_DICT{
    UINT32  dwNumOfDictBuckets; //Indicate the Bucket Number.
    UINT8   sizeOfBlock;        //Indicate the BlockNodeArray Size in Each Block.
    ABU_WEBDB_DICT_BUCKETINODE_S* WebDB_Dict_BucketNodeArray;  
   
    UINT32  dwNumOfNodes;       //Count the number of nodes in WEBDB.Use for Stat.
}ABU_WEBDB_DICT_S;


/**
 * ABU_DICT Methods
 * For WEBDB Useage.
 */
ABU_WEBDB_DICT_S *ABU_WEBDB_DICT_create(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock); 

ABU_WEBDB_DICT_S *ABU_WEBDB_DICT_load(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock, const char *szDictFileName);

// put,get,mode & del
int ABU_WEBDB_DICT_putWithoutLock(ABU_WEBDB_DICT_S * p, UINT32 key1, UINT32 key2, UINT32 dwWebBlockIdx1, UINT32 dwWebBlockIdx2, UINT32 dwSiteSign); 

int ABU_WEBDB_DICT_put(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, UINT32 dwWebBlockIdx1, UINT32 dwWebBlockIdx2, UINT32 dwSiteSign); 

int ABU_WEBDB_DICT_get(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, ABU_WEBDB_DICT_BLOCKINODE_S *tempBlockNode);
int ABU_WEBDB_DICT_getBySiteSign(ABU_WEBDB_DICT_S *p, UINT32 siteSign, UINT32 **resultArray, UINT32 *matchPageNum);

bool ABU_WEBDB_DICT_mod(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, const ABU_WEBDB_DICT_BLOCKINODE_S *tempBlockNode);

bool ABU_WEBDB_DICT_del(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, UINT32 *pBlockIdx1, UINT32 *pBlockIdx2);

bool ABU_WEBDB_DICT_sync(ABU_WEBDB_DICT_S *p, const char *szDictFileName); 

void ABU_WEBDB_DICT_free(ABU_WEBDB_DICT_S *p);

/**
 * Implementation of ABU_GENERALDB_DICT.
 */

//webdb_dict_block structure
typedef struct ABU_GENERALDB_DICT_BLOCKINODE
{   
    UINT32  urlKey1;
    UINT32  urlKey2;
    int     pointer;
    int     other1;
    int     other2;
}ABU_GENERALDB_DICT_BLOCKINODE_S;

//webdb_dict_block structure
typedef struct ABU_GENERALDB_DICT_BLOCK{
    ABU_GENERALDB_DICT_BLOCKINODE_S* BlockNodeArray;
    struct ABU_GENERALDB_DICT_BLOCK* next;
}ABU_GENERALDB_DICT_BLOCK_S;

//webdb_dict_bucket_node structure
typedef struct ABU_GENERALDB_DICT_BUCKETINODE{
    UINT32  urlKey1;
    UINT32  urlKey2;
    int     pointer;
    int     other1;
    int     other2;
    ABU_GENERALDB_DICT_BLOCK_S* next;
}ABU_GENERALDB_DICT_BUCKETINODE_S;

//webdb_dict structure
typedef struct ABU_GENERALDB_DICT{
    UINT32  dwNumOfDictBuckets; //Indicate the Bucket Number.
    UINT32  dwNumOfNodes;       //Count the number of nodes in WEBDB.
    UINT8   sizeOfBlock;        //Indicate the BlockNodeArray Size in Each Block.

    ABU_GENERALDB_DICT_BUCKETINODE_S* GeneralDB_Dict_BucketNodeArray;  
}ABU_GENERALDB_DICT_S;

/**
@ABU_GENERALDB_DICT Methods
@For General Useage.
*/

ABU_GENERALDB_DICT_S * ABU_GENERALDB_DICT_create(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock);

ABU_GENERALDB_DICT_S * ABU_GENERALDB_DICT_load(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock, const char * szDictFileName);

// put,get,mode & del
bool ABU_GENERALDB_DICT_put(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2, int pointer, int other1, int other2); 

bool ABU_GENERALDB_DICT_get(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2, ABU_GENERALDB_DICT_BLOCKINODE_S* tempBlockNode);

bool ABU_GENERALDB_DICT_mod(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2, const ABU_GENERALDB_DICT_BLOCKINODE_S* tempBlockNode);

bool ABU_GENERALDB_DICT_del(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2);

bool ABU_GENERALDB_DICT_sync(ABU_GENERALDB_DICT_S* p, const char * szDictFileName);

void ABU_GENERALDB_DICT_free(ABU_GENERALDB_DICT_S * p);

#endif
