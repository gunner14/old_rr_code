/**
  *$Id: AB_UTILITY_DICT.c,v 1.18 2007/04/07 09:32:36 xhe Exp $
  *
  */
/**
  * Log: 
  * 1) 2007-02-10 add: ABU_WEBDB_DICT_S *ABU_WEBDB_DICT_putWithoutLock() ...To boost scan() performance...
  * 2) 2007-03-04 add: int ABU_WEBDB_DICT_getBySiteSign() ... To boost ab_webdb_getBySite() performance... 
  *
  */
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

#include "AB_UTILITY_DICT.h"
#include "AB_UTILITY_LOG.h"

//*********************************************************/
// 
// Implemetation of LinkInfo DB Dictionary API.
// 
//*********************************************************/

static pthread_rwlock_t linkDBDictRwLock = PTHREAD_RWLOCK_INITIALIZER;

static pthread_rwlock_t webDBDictRwLock = PTHREAD_RWLOCK_INITIALIZER;

static pthread_rwlock_t generalDBDictRwLock = PTHREAD_RWLOCK_INITIALIZER;

#define DICTFILEACCESS (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)
/**
@ Author: yhwu
@ Date: 2007-01-11
@ Des:For LinkDB, create() func just create AB_LINKDB_MemoryDICT.
@ malloc space for it.
@ return AB_LINKDB_MemoryDICT.
*/

ABU_LINKDB_DICT_S* ABU_LINKDB_DICT_create(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock)
{
    ABU_LINKDB_DICT_S* AB_LINKDB_MemoryDICT;

    if((AB_LINKDB_MemoryDICT = (ABU_LINKDB_DICT_S*)
         calloc(1, sizeof(ABU_LINKDB_DICT_S))) == NULL)
    {
         perror("Failed to allocate AB_LINKDB_MemoryDICT");
         return NULL;
    }
                                        
    AB_LINKDB_MemoryDICT->dwNumOfDictBuckets = dwNumOfDictBuckets;
                                            
    AB_LINKDB_MemoryDICT->dwNumOfNodes = 0;
                                                
    AB_LINKDB_MemoryDICT->sizeOfBlock = sizeOfBlock;
    
    if((AB_LINKDB_MemoryDICT->LinkDB_Dict_BucketNodeArray = (ABU_LINKDB_DICT_BUCKETINODE_S*)
        calloc(dwNumOfDictBuckets,sizeof(ABU_LINKDB_DICT_BUCKETINODE_S))) == NULL)
    {
        perror("Failed to allocate AB_LINKDB_MemoryDICT->DictNodeArray");
        return NULL;
    }
    
    return AB_LINKDB_MemoryDICT;
}

bool ABU_LINKDB_DICT_put(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2, UINT32 dwLinkBlockPos, UINT8 linkStatus, UINT32 dwSiteSign)
{   
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    //change log: modify rdlock->unlock->wrlock->unlock to wrlock->unlock.
    //inorder to avoid being interrupted between unlock->wrlock.
    //2007-01-18, yhwu
    //the same modification applys to webdb and generaldb.
    if(((error = pthread_rwlock_wrlock(&linkDBDictRwLock))))
    {
        errno = error;
        return 0;
    }
    
    
    if((p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == 0)&&
        (p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == 0))
    //bucket node is empty;
    {
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey1 = key1;
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey2 = key2;
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].dwLinkBlockPos = dwLinkBlockPos;
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].linkStatus = linkStatus;
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign = dwSiteSign;
        
        if(p->LinkDB_Dict_BucketNodeArray[bucketIndex].next == NULL)
        {
            if((p->LinkDB_Dict_BucketNodeArray[bucketIndex].next = 
                (ABU_LINKDB_DICT_BLOCK_S*)calloc(1, sizeof(ABU_LINKDB_DICT_BLOCK_S))) == NULL)
            {
                perror("Failed to allocate ABU_LINKDB_DICT_BLOCK in ABU_LINKDB_DICT_put");  
                return 0;
            }
            if(((p->LinkDB_Dict_BucketNodeArray[bucketIndex].next)->BlockNodeArray = 
                (ABU_LINKDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_LINKDB_DICT_BLOCKINODE_S)))==NULL)
            {
                perror("Failed to allocate ABU_LINKDB_DICT_BLOCKINODE_S in ABU_LINKDB_DICT_put");   
                return 0;
            }
        }
        
        ++(p->dwNumOfNodes);
        
        if(((error = pthread_rwlock_unlock(&linkDBDictRwLock))))
        {
            errno = error;
            return 0;
        }
        
        return 1;       
    }
    else //bucket node has been occupied
    {
        ABU_LINKDB_DICT_BLOCK_S* tempBlock;

        tempBlock = p->LinkDB_Dict_BucketNodeArray[bucketIndex].next;
        
        int i;
        while(tempBlock)
        {
            for(i=0; i< p->sizeOfBlock; i++)
            {
                if((tempBlock->BlockNodeArray[i].urlKey1 == 0)&&
                    (tempBlock->BlockNodeArray[i].urlKey2 == 0))                    
                {
                    tempBlock->BlockNodeArray[i].urlKey1 = key1;
                    tempBlock->BlockNodeArray[i].urlKey2 = key2;
                    tempBlock->BlockNodeArray[i].dwLinkBlockPos = dwLinkBlockPos;
                    tempBlock->BlockNodeArray[i].linkStatus = linkStatus;
                    tempBlock->BlockNodeArray[i].dwSiteSign = dwSiteSign;
                                                        
                    ++(p->dwNumOfNodes);
                
                    if(((error = pthread_rwlock_unlock(&linkDBDictRwLock))))
                    {
                        errno = error;
                        return 0;
                    }
                    
                    return 1;   
                }
                else
                    continue;
            }
            if(tempBlock->next)
                tempBlock = tempBlock->next;
            else
                break;
        }
        
        //No Available Block;
        //Create a new Block
       if((tempBlock->next = (ABU_LINKDB_DICT_BLOCK_S* )calloc(1, sizeof(ABU_LINKDB_DICT_BLOCK_S))) == NULL)
        {
            perror("Failed to allocate ABU_LINKDB_DICT_BLOCK in ABU_LINKDB_DICT_put");  
            return 0;
        }
        
        tempBlock = tempBlock->next;
        
        if((tempBlock->BlockNodeArray = 
                (ABU_LINKDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_LINKDB_DICT_BLOCKINODE_S)))==NULL)
        {
            perror("Failed to allocate ABU_LINKDB_DICT_BLOCKINODE_S in ABU_LINKDB_DICT_put");   
            return 0;
        }

        tempBlock->BlockNodeArray[0].urlKey1 = key1;
        tempBlock->BlockNodeArray[0].urlKey2 = key2;
        tempBlock->BlockNodeArray[0].dwLinkBlockPos = dwLinkBlockPos;
        tempBlock->BlockNodeArray[0].linkStatus = linkStatus;
        tempBlock->BlockNodeArray[0].dwSiteSign = dwSiteSign;
        
        ++(p->dwNumOfNodes);
        
        if(((error = pthread_rwlock_unlock(&linkDBDictRwLock))))
        {
            errno = error;
            return 0;
        }
        
        return 1;
    }         
}

/**
@
@
@
@
*/
bool ABU_LINKDB_DICT_get(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2, ABU_LINKDB_DICT_BLOCKINODE_S* tempBlockNode)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if(((error = pthread_rwlock_rdlock(&linkDBDictRwLock))))
    {
        errno = error;
        return 0;
    }
    
    if((p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        tempBlockNode->urlKey1 = p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey1;
        tempBlockNode->urlKey2 = p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey2;
        tempBlockNode->dwLinkBlockPos = p->LinkDB_Dict_BucketNodeArray[bucketIndex].dwLinkBlockPos;
        tempBlockNode->linkStatus = p->LinkDB_Dict_BucketNodeArray[bucketIndex].linkStatus;
        tempBlockNode->dwSiteSign = p->LinkDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign;
        
        if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;   
    }
    
    ABU_LINKDB_DICT_BLOCK_S* tempBlock = p->LinkDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                tempBlockNode->urlKey1 = tempBlock->BlockNodeArray[i].urlKey1;
                tempBlockNode->urlKey2 = tempBlock->BlockNodeArray[i].urlKey2;
                tempBlockNode->dwLinkBlockPos = tempBlock->BlockNodeArray[i].dwLinkBlockPos;
                tempBlockNode->linkStatus = tempBlock->BlockNodeArray[i].linkStatus;
                tempBlockNode->dwSiteSign = tempBlock->BlockNodeArray[i].dwSiteSign;
                
                if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}

/**
@
@
@
@
*/
bool ABU_LINKDB_DICT_mod(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2, 
        const ABU_LINKDB_DICT_BLOCKINODE_S* tempBlockNode)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if((error = pthread_rwlock_wrlock(&linkDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    if((p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].dwLinkBlockPos = tempBlockNode->dwLinkBlockPos;
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].linkStatus = tempBlockNode->linkStatus;
        p->LinkDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign = tempBlockNode->dwSiteSign;
        
        if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;   
    }
    
    ABU_LINKDB_DICT_BLOCK_S* tempBlock = p->LinkDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                tempBlock->BlockNodeArray[i].dwLinkBlockPos = tempBlockNode->dwLinkBlockPos;
                tempBlock->BlockNodeArray[i].linkStatus = tempBlockNode->linkStatus;
                tempBlock->BlockNodeArray[i].dwSiteSign = tempBlockNode->dwSiteSign;
                
                if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}


/**
@
@
@
@
*/
bool ABU_LINKDB_DICT_del(ABU_LINKDB_DICT_S * p, UINT32 key1, UINT32 key2)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    error = pthread_rwlock_wrlock(&linkDBDictRwLock);
    if(error != 0)
    {
        _AB_ERROR("Failed to lock linkDBDictRwLock");
        return 0;
    }
    
    if((p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->LinkDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        memset(&(p->LinkDB_Dict_BucketNodeArray[bucketIndex]), 0, sizeof(ABU_LINKDB_DICT_BUCKETINODE_S));
        
        if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        --(p->dwNumOfNodes);
        return 1;   
    }
    
    ABU_LINKDB_DICT_BLOCK_S* tempBlock = p->LinkDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                memset(&(tempBlock->BlockNodeArray[i]),0, sizeof(ABU_LINKDB_DICT_BUCKETINODE_S));
                            
                if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                --(p->dwNumOfNodes);
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&linkDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}

/**
*/
void ABU_LINKDB_DICT_free(ABU_LINKDB_DICT_S * p)
{
    int i;
    
    ABU_LINKDB_DICT_BLOCK_S* tempBlock;
    ABU_LINKDB_DICT_BLOCK_S* nextBlock;
    
    for( i = 0; i<p->dwNumOfDictBuckets; i++)
    {
        tempBlock = p->LinkDB_Dict_BucketNodeArray[i].next;
        
        while(tempBlock)
        {
            nextBlock = tempBlock->next;
            free(tempBlock->BlockNodeArray);
            free(tempBlock);
            tempBlock = nextBlock;
        }
    }
    
    free(p->LinkDB_Dict_BucketNodeArray);
    free(p);
    p = NULL;

}


//*********************************************************/
// 
// Implemetation of WebInfo DB Dictionary API.
// 
//*********************************************************/
/**
@ Provide following APIs For AB_WEBDB Module. 
@
@
@
@
@
@
@
*/

ABU_WEBDB_DICT_S *ABU_WEBDB_DICT_create(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock)
{
    ABU_WEBDB_DICT_S* AB_WEBDB_MemoryDICT;
    
    if((AB_WEBDB_MemoryDICT = (ABU_WEBDB_DICT_S*)calloc(1, sizeof(ABU_WEBDB_DICT_S))) == NULL)
    {
        ABU_LOG_FATAL_r("Failed to allocate AB_WEBDB_MemoryDICT");
        exit(-1);
    }
    
    AB_WEBDB_MemoryDICT->dwNumOfDictBuckets = dwNumOfDictBuckets;
    
    AB_WEBDB_MemoryDICT->dwNumOfNodes = 0;
    
    AB_WEBDB_MemoryDICT->sizeOfBlock = sizeOfBlock;
    
    if((AB_WEBDB_MemoryDICT->WebDB_Dict_BucketNodeArray = (ABU_WEBDB_DICT_BUCKETINODE_S*)
                calloc(dwNumOfDictBuckets, sizeof(ABU_WEBDB_DICT_BUCKETINODE_S))) == NULL)
    {
        ABU_LOG_FATAL_r("Failed to allocate AB_WEBDB_MemoryDICT->DictNodeArray");
        exit(-1);
    }
        
    return AB_WEBDB_MemoryDICT;
}

/**
@ func name:ABU_WEBDB_DICT_load(const char* szDictFileName) 
@ author: yhwu
@ date: 2007-01-11
@ desc:Load the sequence file: szDictFileName, to rebuild the WebDB dictionary
@ without scanning the harddist data file again.
@ param: 
@ szDictFileName-sequence filename, which contains the info of WebDB dictionary.
@ return: 1-success; 0-fail
*/
ABU_WEBDB_DICT_S *ABU_WEBDB_DICT_load(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock, const char * szDictFileName)
{
    ABU_WEBDB_DICT_S* p;
    p = ABU_WEBDB_DICT_create(dwNumOfDictBuckets, sizeOfBlock);
   
    int     error = 0;
    int     MAXBUFSIZE = 1024*45;   //one buffer contains 1024 nodes' info.
    int     bufLen = 0;
    char    buffer[MAXBUFSIZE];
    char*   tempBufIndex = NULL;
    int     fd = 0;                     //file descriptor of szDictFileName;
        
    UINT32     key1, key2, idx1, idx2, siteSign;
    key1 = key2 = idx1 = idx2 = siteSign = 0x00000000;
    fd = open(szDictFileName, O_RDONLY);
    
    if( fd < 0)
    {
        ABU_LOG_WARN_r("unable to open the dict sync file\n");
        exit(-1);
    }
    while((read(fd, buffer, MAXBUFSIZE))>0)
    {
        if(buffer)
            bufLen = strlen(buffer);
        else
        {
            ABU_LOG_WARN_r("fatal error when reading dict sync file.\n");
            exit(-1);
        }
        
        if(bufLen > 1024*45)
            bufLen = 1024*45;
        tempBufIndex = buffer;
        
        while(bufLen>0)
        {
             error = sscanf(tempBufIndex,"%08x,%08x,%08x,%08x,%08x ",&key1, &key2, &idx1, &idx2, &siteSign);
             if(error == 5)
             {       
                 ABU_WEBDB_DICT_put(p, key1, key2, idx1, idx2, siteSign);
                 bufLen -= 45;
                 tempBufIndex += 45;
             }       
             else    
             {
                 ABU_LOG_WARN_r("error occurs when sscanf\n");
                 return NULL;
             }
        }

        memset(buffer, 0, MAXBUFSIZE);
    }

    close(fd);
    return p;
}

/**
@ func name:ABU_WEBDB_DICT_sync(const char* szDictFileName) 
@ author: yhwu
@ date: 2007-01-11
@
@ desc:Write the dictionary info into sequence file: szDictFileName.
@ For each Node in dictionary, we abstract 5 UINT32: key1, key2, idx1, idx2, siteSign.
@ Then put them into tempbuffer, whose size is 1024*sizeof(5*(8+1)).
@ each UINT32(except the last one) is transformatted to 8 char in hexadecimal form plus a comma(,).
@ Blank Space(" ") follows each Node info(5 UINT32).
@ The form is as following:
@ 12345678,    5678,      12,        8,    678 87654321,....
@
@ param: 
@ szDictFileName-sequence filename, which is used to write the info.
@ return: 1-success; 0-fail
*/

bool ABU_WEBDB_DICT_sync(ABU_WEBDB_DICT_S *p, const char * szDictFileName)
{
    int     MAXBUFSIZE = 1024*45;   //one buffer contains 1024 nodes' info.
    int     numOfNodesInBuf=0;
    int     bucketIndex = 0;            //just to indicate the index of bucket.
    
    char    buffer[MAXBUFSIZE];

    char    *tempBufIndex = buffer;

    ABU_WEBDB_DICT_BLOCKINODE_S tempNode;
    ABU_WEBDB_DICT_BLOCK_S* iteraterBlock = NULL;

    int     fd;
    int error = 0;

    fd = open(szDictFileName, O_WRONLY|O_CREAT|O_TRUNC, DICTFILEACCESS);
    
    error = pthread_rwlock_rdlock(&webDBDictRwLock);
    if(error != 0)
    {
        ABU_LOG_FATAL_r("unable to lock webDBDictRwLock in ABU_WEBDB_DICT_sync()\n");
        exit(-1);
    }
     
    while(bucketIndex<(p->dwNumOfDictBuckets))
    {
        if((p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1!=0)||(p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2)!=0)
        {
            tempNode.urlKey1 = p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1;
            tempNode.urlKey2 = p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2;
            tempNode.dwWebBlockIdx1 = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1;
            tempNode.dwWebBlockIdx2 = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2;
            tempNode.dwSiteSign = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign;

            sprintf(tempBufIndex, "%08x,%08x,%08x,%08x,%08x ", 
                    tempNode.urlKey1, tempNode.urlKey2, tempNode.dwWebBlockIdx1, tempNode.dwWebBlockIdx2, tempNode.dwSiteSign);
            
            numOfNodesInBuf++;
            tempBufIndex += 45;
            
            //if numOfNodesInBuf == 1024, buffer is full.
            //write buffer into file.
            if(numOfNodesInBuf == 1024)
            {
                write(fd, buffer, MAXBUFSIZE);
                numOfNodesInBuf = 0;
                tempBufIndex = buffer;
            }
            
        }

        iteraterBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;

        int i;
        
        while(iteraterBlock)
        {
            for(i=0; i<p->sizeOfBlock;i++)
            {
                if((iteraterBlock->BlockNodeArray[i].urlKey1!=0)||(iteraterBlock->BlockNodeArray[i].urlKey2!=0))
                {
                    tempNode.urlKey1 = iteraterBlock->BlockNodeArray[i].urlKey1;
                    tempNode.urlKey2 = iteraterBlock->BlockNodeArray[i].urlKey2;
                    tempNode.dwWebBlockIdx1 = iteraterBlock->BlockNodeArray[i].dwWebBlockIdx1;
                    tempNode.dwWebBlockIdx2 = iteraterBlock->BlockNodeArray[i].dwWebBlockIdx2;
                    tempNode.dwSiteSign = iteraterBlock->BlockNodeArray[i].dwSiteSign;

 
                    sprintf(tempBufIndex, "%08x,%08x,%08x,%08x,%08x ", tempNode.urlKey1, tempNode.urlKey2, tempNode.dwWebBlockIdx1, tempNode.dwWebBlockIdx2, tempNode.dwSiteSign);
            
                    numOfNodesInBuf++;
                    tempBufIndex += 45;
            
                    //if numOfNodesInBuf == 1024, buffer is full.
                    //write buffer into file.
                    if(numOfNodesInBuf == 1024)
                    {
                        write(fd, buffer, MAXBUFSIZE);
                        numOfNodesInBuf = 0;
                        tempBufIndex = buffer;
                    }
                        
                }
                else
                    continue;
            }//end of for

            iteraterBlock = iteraterBlock->next;
        
        }//end of while(iteraterBlock)
        
        bucketIndex++;
    }//end of while(bucketIndex<...)
    
        
    error = pthread_rwlock_unlock(&webDBDictRwLock);
    if(error != 0)
    {
        ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_sync()\n");
        exit(-1);
    }
    
    buffer[numOfNodesInBuf*45]='\0'; //enable strlen() func;
    
    if(numOfNodesInBuf>0)
    {
        write(fd, buffer, numOfNodesInBuf*45+1);
    }
    close(fd);
    return 1;
}

/**
  @
  @
  @
  @
  @
  */
int ABU_WEBDB_DICT_putWithoutLock(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, 
        UINT32 dwWebBlockIdx1, UINT32 dwWebBlockIdx2, UINT32 dwSiteSign)
{   
    int blockNodeIdx = 0;
    int i = 0;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);

    if((p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == 0)&&
        (p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == 0))
    //bucket node is empty;
    {
        p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 = key1;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 = key2;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1 = dwWebBlockIdx1;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2 = dwWebBlockIdx2;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign = dwSiteSign;
        
        if(p->WebDB_Dict_BucketNodeArray[bucketIndex].next == NULL)
        {
            if((p->WebDB_Dict_BucketNodeArray[bucketIndex].next = 
                (ABU_WEBDB_DICT_BLOCK_S*)calloc(1, sizeof(ABU_WEBDB_DICT_BLOCK_S))) == NULL)
            {
                ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCK in ABU_WEBDB_DICT_put()");   
                exit(-1);
            }
            if(((p->WebDB_Dict_BucketNodeArray[bucketIndex].next)->BlockNodeArray = 
                (ABU_WEBDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_WEBDB_DICT_BLOCKINODE_S)))==NULL)
            {
                ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCKINODE_S in ABU_LINKDB_DICT_put()\n");    
                exit(-1);
            }
        }
        p->dwNumOfNodes += 1;
        //ABU_LOG_DEBUG_r("put page with hash value %08x, %08x into webdb's %llu th bucket successfully.\n", key1, key2, bucketIndex);
        return 1;       
    }
    else //bucket node has been occupied
    {
        ABU_WEBDB_DICT_BLOCK_S* tempBlock;
        tempBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;
        do{
            for(i=0; i< p->sizeOfBlock; i++)
            {
                if((tempBlock->BlockNodeArray[i].urlKey1 == 0)&&(tempBlock->BlockNodeArray[i].urlKey2 == 0))                    
                {
                    tempBlock->BlockNodeArray[i].urlKey1 = key1;
                    tempBlock->BlockNodeArray[i].urlKey2 = key2;
                    tempBlock->BlockNodeArray[i].dwWebBlockIdx1 = dwWebBlockIdx1;
                    tempBlock->BlockNodeArray[i].dwWebBlockIdx2 = dwWebBlockIdx2;
                    tempBlock->BlockNodeArray[i].dwSiteSign = dwSiteSign;
                    //ABU_LOG_DEBUG_r("put page with hash value %08x, %08x, into webdb's %llu th bucket, %d th blocknode, %d th node successfully.\n", 
                            //key1, key2, bucketIndex, blockNodeIdx, i);
                    p->dwNumOfNodes += 1;
                    return 1;   
                }
            }
            
            if(NULL != tempBlock->next)
            {
                tempBlock = tempBlock->next;
                blockNodeIdx += 1;
            }
            else
                break;
        }while(1);
        
        //No Available Block;
        //Create a new Block
        if((tempBlock->next = (ABU_WEBDB_DICT_BLOCK_S* )calloc(1, sizeof(ABU_WEBDB_DICT_BLOCK_S))) == NULL)
        {
            ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCK in ABU_WEBDB_DICT_put\n");   
            exit(-1);
        }
        
        tempBlock = tempBlock->next;
        blockNodeIdx += 1;
        if((tempBlock->BlockNodeArray = (ABU_WEBDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_WEBDB_DICT_BLOCKINODE_S)))==NULL)
        {
            ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCK in ABU_WEBDB_DICT_put\n");   
            exit(-1);
        }

        tempBlock->BlockNodeArray[0].urlKey1 = key1;
        tempBlock->BlockNodeArray[0].urlKey2 = key2;
        tempBlock->BlockNodeArray[0].dwWebBlockIdx1 = dwWebBlockIdx1;
        tempBlock->BlockNodeArray[0].dwWebBlockIdx2 = dwWebBlockIdx2;
        tempBlock->BlockNodeArray[0].dwSiteSign = dwSiteSign;
        
        p->dwNumOfNodes += 1;
        //ABU_LOG_DEBUG_r("after creating a new blocknode, put page with hash value %08x, %08x, into webdb's %llu th bucket, %d th blocknode, 0th node successfully.\n", key1, key2, bucketIndex, blockNodeIdx);
        return 1;
    }         
}

int ABU_WEBDB_DICT_put(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, UINT32 dwWebBlockIdx1, UINT32 dwWebBlockIdx2, UINT32 dwSiteSign)
{   
    int error = 0;
    int blockNodeIdx = 0;
    int i = 0;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
     
    error = pthread_rwlock_wrlock(&webDBDictRwLock);
    if(error != 0)
    {
        ABU_LOG_FATAL_r("unable to lock webDBDictRwLock in ABU_WEBDB_DICT_put()\n");
        exit(-1);
    }

    if((p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == 0)&&
        (p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == 0))
    //bucket node is empty;
    {
        p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 = key1;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 = key2;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1 = dwWebBlockIdx1;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2 = dwWebBlockIdx2;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign = dwSiteSign;
        
        if(p->WebDB_Dict_BucketNodeArray[bucketIndex].next == NULL)
        {
            if((p->WebDB_Dict_BucketNodeArray[bucketIndex].next = 
                (ABU_WEBDB_DICT_BLOCK_S*)calloc(1, sizeof(ABU_WEBDB_DICT_BLOCK_S))) == NULL)
            {
                ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCK in ABU_WEBDB_DICT_put()");   
                exit(-1);
            }
            if(((p->WebDB_Dict_BucketNodeArray[bucketIndex].next)->BlockNodeArray = 
                (ABU_WEBDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_WEBDB_DICT_BLOCKINODE_S)))==NULL)
            {
                ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCKINODE_S in ABU_LINKDB_DICT_put()\n");    
                exit(-1);
            }
        }
        
        p->dwNumOfNodes += 1;
        
        //ABU_LOG_DEBUG_r("put page with hash value %08x, %08x into webdb's %llu th bucket successfully.\n", key1, key2, bucketIndex);
        
        error = pthread_rwlock_unlock(&webDBDictRwLock);
        if(error != 0)
        {
            ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_put()\n");
            exit(-1);
        }   
        
        return 1;       
    }
    else //bucket node has been occupied
    {
        ABU_WEBDB_DICT_BLOCK_S* tempBlock;

        tempBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;
        
        do{
            for(i=0; i< p->sizeOfBlock; i++)
            {
                if((tempBlock->BlockNodeArray[i].urlKey1 == 0)&&(tempBlock->BlockNodeArray[i].urlKey2 == 0))                    
                {
                    tempBlock->BlockNodeArray[i].urlKey1 = key1;
                    tempBlock->BlockNodeArray[i].urlKey2 = key2;
                    tempBlock->BlockNodeArray[i].dwWebBlockIdx1 = dwWebBlockIdx1;
                    tempBlock->BlockNodeArray[i].dwWebBlockIdx2 = dwWebBlockIdx2;
                    tempBlock->BlockNodeArray[i].dwSiteSign = dwSiteSign;
                                                        
                    //ABU_LOG_DEBUG_r("put page with hash value %08x, %08x, into webdb's %llu th bucket, %d th blocknode, %d th node successfully.\n", 
                            //key1, key2, bucketIndex, blockNodeIdx, i);
                    
                    p->dwNumOfNodes += 1;

                    error = pthread_rwlock_unlock(&webDBDictRwLock);
                    if(error != 0)
                    {
                        ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_put()\n");
                        exit(-1);
                    }   
                                        
                    return 1;   
                }
            }
            
            if(NULL != tempBlock->next)
            {
                tempBlock = tempBlock->next;
                blockNodeIdx += 1;
            }
            else
                break;
        }while(1);
        
        //No Available Block;
        //Create a new Block
        if((tempBlock->next = (ABU_WEBDB_DICT_BLOCK_S* )calloc(1, sizeof(ABU_WEBDB_DICT_BLOCK_S))) == NULL)
        {
            ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCK in ABU_WEBDB_DICT_put\n");   
            exit(-1);
        }
        
        tempBlock = tempBlock->next;
        blockNodeIdx += 1;
        
        if((tempBlock->BlockNodeArray = (ABU_WEBDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_WEBDB_DICT_BLOCKINODE_S)))==NULL)
        {
            ABU_LOG_FATAL_r("Failed to allocate ABU_WEBDB_DICT_BLOCK in ABU_WEBDB_DICT_put\n");   
            exit(-1);
        }

        tempBlock->BlockNodeArray[0].urlKey1 = key1;
        tempBlock->BlockNodeArray[0].urlKey2 = key2;
        tempBlock->BlockNodeArray[0].dwWebBlockIdx1 = dwWebBlockIdx1;
        tempBlock->BlockNodeArray[0].dwWebBlockIdx2 = dwWebBlockIdx2;
        tempBlock->BlockNodeArray[0].dwSiteSign = dwSiteSign;
        
        p->dwNumOfNodes += 1;
        
        //ABU_LOG_DEBUG_r("after creating a new blocknode, put page with hash value %08x, %08x, into webdb's %llu th bucket, %d th blocknode, 0th node successfully.\n", key1, key2, bucketIndex, blockNodeIdx);
        
        error = pthread_rwlock_unlock(&webDBDictRwLock);
        if(error != 0)
        {
            ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_put().\n");
            exit(-1);
        }   
                
        return 1;
    }         
}

/**
@
@
@
@
*/
int ABU_WEBDB_DICT_get(ABU_WEBDB_DICT_S *p, UINT32 key1, UINT32 key2, ABU_WEBDB_DICT_BLOCKINODE_S *tempBlockNode)
{
    int error = 0;
    int i = 0;
    int blockIdx = 0;
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    error = pthread_rwlock_rdlock(&webDBDictRwLock);
    if(error != 0)
    {
        ABU_LOG_FATAL_r("unable to lock webDBDictRwLock in ABU_WEBDB_DICT_get()\n");
        exit(-1);
    }
    
    if((p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&(p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        tempBlockNode->urlKey1 = p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1;
        tempBlockNode->urlKey2 = p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2;
        tempBlockNode->dwWebBlockIdx1 = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1;
        tempBlockNode->dwWebBlockIdx2 = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2;
        tempBlockNode->dwSiteSign = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign;
        
        error = pthread_rwlock_unlock(&webDBDictRwLock);
        if(error != 0)        
        {
            ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_get() after we have found it.\n");
            exit(-1);
        }
        
        //ABU_LOG_DEBUG_r("find %08x, %08x in webdb's %llu th bucket\n", key1, key2, bucketIndex);
        return 1;   
    }
    
    ABU_WEBDB_DICT_BLOCK_S* tempBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;
    
    while(NULL != tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                tempBlockNode->urlKey1 = tempBlock->BlockNodeArray[i].urlKey1;
                tempBlockNode->urlKey2 = tempBlock->BlockNodeArray[i].urlKey2;
                tempBlockNode->dwWebBlockIdx1 = tempBlock->BlockNodeArray[i].dwWebBlockIdx1;
                tempBlockNode->dwWebBlockIdx2 = tempBlock->BlockNodeArray[i].dwWebBlockIdx2;
                tempBlockNode->dwSiteSign = tempBlock->BlockNodeArray[i].dwSiteSign;
                
                error = pthread_rwlock_unlock(&webDBDictRwLock);
                if(error != 0)        
                {
                    ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_get() after we have found it.\n");
                    exit(-1);
                }

                //ABU_LOG_DEBUG_r("find %08x, %08x in webdb's %llu bucket, %d th block, %d th node.\n", 
                        //key1, key2, bucketIndex, blockIdx, i);
                return 1; 
            }
        }
        
        tempBlock = tempBlock->next; 
        blockIdx += 1;
        
    }
    
    error = pthread_rwlock_unlock(&webDBDictRwLock);
    if(error != 0)        
    {
        ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_get() after we have not found it.\n");
        exit(-1);
    }

    //ABU_LOG_DEBUG_r("can not find %08x, %08x in webdb\n", key1, key2);
    return 0; 
}

/**
@
@
@
@
*/

#define UINT32_NUM_PER_MALLOC (1<<16) //65536 uint32 = 32968 items.

int ABU_WEBDB_DICT_getBySiteSign(ABU_WEBDB_DICT_S *p, UINT32 siteSign, UINT32 **resultArray, UINT32 *matchPageNum)
{
    int error = 0;
    int itemIndex = 0;
    int arraySize = 0;
    UINT32 *tempResultArray = (UINT32 *)calloc(UINT32_NUM_PER_MALLOC, sizeof(UINT32));
    arraySize = UINT32_NUM_PER_MALLOC;
    
    ABU_WEBDB_DICT_BLOCK_S *iteraterBlock = NULL;
    int bucketIndex = 0;
 
    error = pthread_rwlock_rdlock(&webDBDictRwLock);
    if(error != 0)
    {
        ABU_LOG_FATAL_r("unable to lock webDBDictRwLock in ABU_WEBDB_DICT_get()\n");
        exit(-1);
    }

    while(bucketIndex<(p->dwNumOfDictBuckets))
    {
        if(p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign == siteSign)
        {
            if(itemIndex >= arraySize)
            {
                tempResultArray = (UINT32 *)realloc(tempResultArray, (arraySize+UINT32_NUM_PER_MALLOC)*sizeof(UINT32));
                if(tempResultArray == NULL)
                {
                    ABU_LOG_FATAL("Failed to alloc UINT32 array.\n");
                    return -1;
                }
                //initialize newly allocated memory.
                memset(tempResultArray+arraySize, 0, UINT32_NUM_PER_MALLOC*sizeof(UINT32));
                arraySize += UINT32_NUM_PER_MALLOC;
            }
         
            memcpy(tempResultArray+itemIndex, &(p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1), sizeof(UINT32));
            itemIndex++;
            memcpy(tempResultArray+itemIndex, &(p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2), sizeof(UINT32));
            itemIndex++;
        }

        iteraterBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;

        int i;
        
        while(iteraterBlock)
        {
            for(i=0; i<p->sizeOfBlock;i++)
            {
                if(iteraterBlock->BlockNodeArray[i].dwSiteSign == siteSign)
                {
                    if(itemIndex >= arraySize)
                    {
                        tempResultArray = (UINT32 *)realloc(tempResultArray, (arraySize+UINT32_NUM_PER_MALLOC)*sizeof(UINT32));
                        if(tempResultArray == NULL)
                        {
                            ABU_LOG_FATAL("Failed to alloc UINT32 array.\n");
                            return -1;
                        }
                        //initialize newly allocated memory.
                        memset(tempResultArray+arraySize, 0, UINT32_NUM_PER_MALLOC*sizeof(UINT32));
                        arraySize += UINT32_NUM_PER_MALLOC;
                    }

                    memcpy(tempResultArray+itemIndex, &(iteraterBlock->BlockNodeArray[i].dwWebBlockIdx1), sizeof(UINT32));
                    itemIndex++;
                    memcpy(tempResultArray+itemIndex, &(iteraterBlock->BlockNodeArray[i].dwWebBlockIdx2), sizeof(UINT32));
                    itemIndex++;
                }
            }//end of for
            iteraterBlock = iteraterBlock->next;
        }//end of while(iteraterBlock)
        
        bucketIndex++;
    }//end of while(bucketIndex<...)
   
    error = pthread_rwlock_unlock(&webDBDictRwLock);
    if(error != 0)        
    {
        ABU_LOG_FATAL_r("unable to unlock webDBDictRwLock in ABU_WEBDB_DICT_get() after we have not found it.\n");
        exit(-1);
    }

    if(tempResultArray)
    {
        *resultArray = tempResultArray;
        *matchPageNum = (itemIndex>>1);
        return 0; 
    }
    else
    {
        //calloc/realloc failed.
        free(tempResultArray);
        *matchPageNum = 0;
        return -1;
    }
}
/**
@
@
@
@
*/
bool ABU_WEBDB_DICT_mod(ABU_WEBDB_DICT_S * p, UINT32 key1, UINT32 key2, 
        const ABU_WEBDB_DICT_BLOCKINODE_S* tempBlockNode)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if((error = pthread_rwlock_wrlock(&webDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    if((p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1 = tempBlockNode->dwWebBlockIdx1;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2 = tempBlockNode->dwWebBlockIdx2;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign = tempBlockNode->dwSiteSign;
        
        if((error = pthread_rwlock_unlock(&webDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;   
    }
    
    ABU_WEBDB_DICT_BLOCK_S* tempBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                tempBlock->BlockNodeArray[i].dwWebBlockIdx1 = tempBlockNode->dwWebBlockIdx1;
                tempBlock->BlockNodeArray[i].dwWebBlockIdx2 = tempBlockNode->dwWebBlockIdx2;
                tempBlock->BlockNodeArray[i].dwSiteSign = tempBlockNode->dwSiteSign;
                
                if((error = pthread_rwlock_unlock(&webDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&webDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}


/**
@
@
@
@
*/
bool ABU_WEBDB_DICT_del(ABU_WEBDB_DICT_S * p, UINT32 key1, UINT32 key2, UINT32 *pBlockIdx1, UINT32 *pBlockIdx2)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    error = pthread_rwlock_wrlock(&webDBDictRwLock);
    
    if(error != 0)
    {
        ABU_LOG_FATAL_r("failed to lock webDBDictRwLock");
        exit(-1);
    }
    
    if((p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        *pBlockIdx1 = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1;
        *pBlockIdx2 = p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2;
        
        //"memset" segment should be challenged..since it will set next to be 0x0..
        //thus this operation will lost BlockNode List Info which is appended to it.
        //So, we of course can't get correctly...
        //memset(&(p->WebDB_Dict_BucketNodeArray[bucketIndex]), 0, sizeof(ABU_WEBDB_DICT_BUCKETINODE_S));
        
        p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey1 = 0x00000000;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].urlKey2 = 0x00000000;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx1 = 0x00000000;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwWebBlockIdx2 = 0x00000000;
        p->WebDB_Dict_BucketNodeArray[bucketIndex].dwSiteSign = 0x00000000;
        
        error = pthread_rwlock_unlock(&webDBDictRwLock);
        if(error != 0)
        {
            ABU_LOG_FATAL_r("failed to unlock webDBDictRwLock");
            exit(-1);
        }
        
        --(p->dwNumOfNodes);
        return 1;   
    }
    
    ABU_WEBDB_DICT_BLOCK_S* tempBlock = p->WebDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                *pBlockIdx1 = tempBlock->BlockNodeArray[i].dwWebBlockIdx1;
                *pBlockIdx2 = tempBlock->BlockNodeArray[i].dwWebBlockIdx2;
                
                memset(&(tempBlock->BlockNodeArray[i]),0, sizeof(ABU_WEBDB_DICT_BLOCKINODE_S));
                            
                --(p->dwNumOfNodes);
                
                error = pthread_rwlock_unlock(&webDBDictRwLock);
                if(error != 0)
                {
                    ABU_LOG_FATAL_r("failed to unlock webDBDictRwLock");
                    return 0;
                }
                
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    error = pthread_rwlock_unlock(&webDBDictRwLock);
    if(error != 0)
    {
        ABU_LOG_FATAL_r("Fail to unlock webDBDictRwLock");
        return 0;
    }
    
    ABU_LOG_WARN_r("Fail to find the request URL, with hash value %08x,%08x", key1, key2);
    return 0;   
}

/**
*/
void ABU_WEBDB_DICT_free(ABU_WEBDB_DICT_S * p)
{
    int i;
    
    ABU_WEBDB_DICT_BLOCK_S* tempBlock;
    ABU_WEBDB_DICT_BLOCK_S* nextBlock;
    
    for( i = 0; i<p->dwNumOfDictBuckets; i++)
    {
        tempBlock = p->WebDB_Dict_BucketNodeArray[i].next;
        
        while(tempBlock)
        {
            nextBlock = tempBlock->next;
            free(tempBlock->BlockNodeArray);
            free(tempBlock);
            tempBlock = nextBlock;
        }
    }
    
    free(p->WebDB_Dict_BucketNodeArray);
    free(p);
    p=NULL;
}

/**
@
@
@
@
*/

ABU_GENERALDB_DICT_S* ABU_GENERALDB_DICT_create(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock)
{
    ABU_GENERALDB_DICT_S* AB_GENERALDB_MemoryDICT;
    
    if((AB_GENERALDB_MemoryDICT = (ABU_GENERALDB_DICT_S*)calloc(1, sizeof(ABU_GENERALDB_DICT_S))) == NULL)
    {
        perror("Failed to allocate AB_GENERALDB_MemoryDICT");
        return NULL;
    }
    
    AB_GENERALDB_MemoryDICT->dwNumOfDictBuckets = dwNumOfDictBuckets;
    AB_GENERALDB_MemoryDICT->dwNumOfNodes = 0;
    AB_GENERALDB_MemoryDICT->sizeOfBlock = sizeOfBlock;
    
    if((AB_GENERALDB_MemoryDICT->GeneralDB_Dict_BucketNodeArray = 
        (ABU_GENERALDB_DICT_BUCKETINODE_S*)calloc(AB_GENERALDB_MemoryDICT->dwNumOfDictBuckets, sizeof(ABU_LINKDB_DICT_BUCKETINODE_S))) == NULL)
    {
        perror("Failed to allocate AB_GENERALDB_MemoryDICT->DictNodeArray");
        return NULL;
    }
    return AB_GENERALDB_MemoryDICT;
}

/**
@ func name:ABU_GENERALDB_DICT_load(const char* szDictFileName) 
@ author: yhwu
@ date: 2007-01-11
@ desc:Load the sequence file: szDictFileName, to rebuild the WebDB dictionary
@ without scanning the harddist data file again.
@ param: 
@ szDictFileName-sequence filename, which contains the info of WebDB dictionary.
@ return: 1-success; 0-fail
*/
ABU_GENERALDB_DICT_S* ABU_GENERALDB_DICT_load(UINT32 dwNumOfDictBuckets, UINT8 sizeOfBlock, const char * szDictFileName)
{
    ABU_GENERALDB_DICT_S* p;
    p = ABU_GENERALDB_DICT_create(dwNumOfDictBuckets, sizeOfBlock);
    
    int     MAXBUFSIZE = 1024*45;   //one buffer contains 1024 nodes' info.
    int     bufLen;
    char    buffer[MAXBUFSIZE];
    char*   tempBufIndex;
    int     fd;                     //file descriptor of szDictFileName;
    int     key1, key2, pointer, other1, other2;

    fd = open(szDictFileName, O_RDONLY);
    while((read(fd, buffer, MAXBUFSIZE))>0)
    {
        bufLen = strlen(buffer);
        if(bufLen > 1024*45)
            bufLen = 1024*45;
        tempBufIndex = buffer;
        
        while(bufLen>0)
        {
             if(sscanf(tempBufIndex,"%08x,%08x,%08x,%08x,%08x ",&key1, &key2, &pointer, &other1, &other2))
             {       
                 ABU_GENERALDB_DICT_put(p, key1, key2, pointer, other1, other2);
                 bufLen -= 45;
                 tempBufIndex += 45;
             }       
             else    
                 perror("error occurs when sscanf\n");
        }
    }
    close(fd);
    return p;
}

/**
@ func name:ABU_GENERALDB_DICT_sync(const char* szDictFileName) 
@ author: yhwu
@ date: 2007-01-11
@
@ desc:Write the dictionary info into sequence file: szDictFileName.
@ For each Node in dictionary, we abstract 5 UINT32: key1, key2, pointer, other1, other2.
@ Then put them into tempbuffer, whose size is 1024*sizeof(5*(8+1)).
@ each UINT32(except the last one) is transformatted to 8 char in hexadecimal form plus a comma(,).
@ Blank Space(" ") follows each Node info(5 UINT32).
@ The form is as following:
@ 12345678,    5678,      12,        8,    678 87654321,....
@
@ param: 
@ szDictFileName-sequence filename, which is used to write the info.
@ return: 1-success; 0-fail
*/

bool ABU_GENERALDB_DICT_sync(ABU_GENERALDB_DICT_S* p, const char * szDictFileName)
{
    int     MAXBUFSIZE = 1024*45;   //one buffer contains 1024 nodes' info.
    int     numOfNodesInBuf=0;
    int     bucketIndex = 0;            //just to indicate the index of bucket.
    
    char    buffer[MAXBUFSIZE];

    char*   tempBufIndex = buffer;

    ABU_GENERALDB_DICT_BLOCKINODE_S tempNode;
    ABU_GENERALDB_DICT_BLOCK_S* iteraterBlock;

    int fd;

    fd = open(szDictFileName, O_WRONLY|O_CREAT|O_TRUNC, DICTFILEACCESS);
    
    int error = 0;
    if((error = pthread_rwlock_rdlock(&generalDBDictRwLock)) != 0)
    {
        ABU_LOG_FATAL_r("unable to lock generalDBDictRwLock in ABU_GENERALDB_DICT_sync()\n");
        exit(-1);
    }
    
    while(bucketIndex<(p->dwNumOfDictBuckets))
    {
        if((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1!=0)
            ||(p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2)!=0)
        {
            tempNode.urlKey1 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1;
            tempNode.urlKey2 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2;
            tempNode.pointer = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].pointer;
            tempNode.other1 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other1;
            tempNode.other2 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other2;

            sprintf(tempBufIndex, "%08x,%08x,%08x,%08x,%08x ", 
                    tempNode.urlKey1, tempNode.urlKey2, tempNode.pointer, tempNode.other1, tempNode.other2);
            
            numOfNodesInBuf++;
            tempBufIndex += 45;
            
            //if numOfNodesInBuf == 1024, buffer is full.
            //write buffer into file.
            if(numOfNodesInBuf == 1024)
            {
                write(fd, buffer, MAXBUFSIZE);
                numOfNodesInBuf = 0;
                tempBufIndex = buffer;
            }
            
        }

        iteraterBlock = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next;

        int i;
        
        while(iteraterBlock)
        {
            for(i=0; i<p->sizeOfBlock;i++)
            {
                if((iteraterBlock->BlockNodeArray[i].urlKey1!=0)||
                   (iteraterBlock->BlockNodeArray[i].urlKey2)!=0)
                {
                    tempNode.urlKey1 = iteraterBlock->BlockNodeArray[i].urlKey1;
                    tempNode.urlKey2 = iteraterBlock->BlockNodeArray[i].urlKey2;
                    tempNode.pointer = iteraterBlock->BlockNodeArray[i].pointer;
                    tempNode.other1 = iteraterBlock->BlockNodeArray[i].other1;
                    tempNode.other2 = iteraterBlock->BlockNodeArray[i].other2;

 
                    sprintf(tempBufIndex, "%08x,%08x,%08x,%08x,%08x ", 
                        tempNode.urlKey1, tempNode.urlKey2, tempNode.pointer, tempNode.other1, tempNode.other2);
            
                    numOfNodesInBuf++;
                    tempBufIndex += 45;
            
                    //if numOfNodesInBuf == 1024, buffer is full.
                    //write buffer into file.
                    if(numOfNodesInBuf == 1024)
                    {
                        write(fd, buffer, MAXBUFSIZE);
                        numOfNodesInBuf = 0;
                        tempBufIndex = buffer;
                    }
                        
                }
                else
                    continue;
            }//end of for

            iteraterBlock = iteraterBlock->next;
        
        }//end of while(iteraterBlock)
        
        bucketIndex++;
    }//end of while(bucketIndex++)

    if((error = pthread_rwlock_unlock(&generalDBDictRwLock)) != 0)
    {
        ABU_LOG_FATAL_r("unable to unlock generalDBDictRwLock in ABU_GENERALDB_DICT_sync()\n");
        exit(-1);
    }
    buffer[numOfNodesInBuf*45]='\0'; //enable strlen() func;
    
    if(numOfNodesInBuf>0)
    {
        write(fd, buffer, numOfNodesInBuf*45+1);
    }

    close(fd);
    
    return 1;
    
}


/**
  @
  @
  @
  */

bool ABU_GENERALDB_DICT_put(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2, 
        int pointer, int other1, int other2)
{   
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if((error = pthread_rwlock_wrlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    
    if((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == 0)&&
        (p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == 0))
    //bucket node is empty;
    {
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1 = key1;
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2 = key2;
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].pointer = pointer;
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other1 = other1;
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other2 = other2;
        
        if(p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next == NULL)
        {
            if((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next = 
                (ABU_GENERALDB_DICT_BLOCK_S*)calloc(1, sizeof(ABU_GENERALDB_DICT_BLOCK_S))) == NULL)
            {
                perror("Failed to allocate ABU_GENERALDB_DICT_BLOCK in ABU_GENERALDB_DICT_put");   
                return 0;
            }
            if(((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next)->BlockNodeArray = 
                (ABU_GENERALDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_GENERALDB_DICT_BLOCKINODE_S)))==NULL)
            {
                perror("Failed to allocate ABU_GENERALDB_DICT_BLOCKINODE_S in ABU_GENERALDB_DICT_put");    
                return 0;
            }
        }
        
        ++(p->dwNumOfNodes);
        
        if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;       
    }
    else //bucket node has been occupied
    {
        ABU_GENERALDB_DICT_BLOCK_S* tempBlock;

        tempBlock = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next;
        
        int i;
        while(tempBlock)
        {
            for(i=0; i< p->sizeOfBlock; i++)
            {
                if((tempBlock->BlockNodeArray[i].urlKey1 == 0)&&
                    (tempBlock->BlockNodeArray[i].urlKey2 == 0))                    
                {
                    tempBlock->BlockNodeArray[i].urlKey1 = key1;
                    tempBlock->BlockNodeArray[i].urlKey2 = key2;
                    tempBlock->BlockNodeArray[i].pointer = pointer;
                    tempBlock->BlockNodeArray[i].other1 = other1;
                    tempBlock->BlockNodeArray[i].other2 = other2;
                                                        
                    ++(p->dwNumOfNodes);
                
                    if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
                    {
                        errno = error;
                        return 0;
                    }
                    
                    return 1;   
                }
                else
                    continue;
            }
            if(tempBlock->next)
                tempBlock = tempBlock->next;
            else
                break;
        }
        
        //No Available Block;
        //Create a new Block
        if((tempBlock->next = (ABU_GENERALDB_DICT_BLOCK_S* )calloc(1, sizeof(ABU_GENERALDB_DICT_BLOCK_S))) == NULL)
        {
            perror("Failed to allocate ABU_GENERALDB_DICT_BLOCK in ABU_GENERALDB_DICT_put");   
            return 0;
        }
        
        tempBlock = tempBlock->next;
        
        if((tempBlock->BlockNodeArray = 
                (ABU_GENERALDB_DICT_BLOCKINODE_S*)calloc(p->sizeOfBlock,sizeof(ABU_GENERALDB_DICT_BLOCKINODE_S)))==NULL)
        {
            perror("Failed to allocate ABU_GENERALDB_DICT_BLOCKINODE_S in ABU_GENERALDB_DICT_put");    
            return 0;
        }

        tempBlock->BlockNodeArray[0].urlKey1 = key1;
        tempBlock->BlockNodeArray[0].urlKey2 = key2;
        tempBlock->BlockNodeArray[0].pointer = pointer;
        tempBlock->BlockNodeArray[0].other1 = other1;
        tempBlock->BlockNodeArray[0].other2 = other2;
        
        ++(p->dwNumOfNodes);
        
        if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;
    }         
}

/**
@
@
@
@
*/
bool ABU_GENERALDB_DICT_get(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2,
        ABU_GENERALDB_DICT_BLOCKINODE_S* tempBlockNode)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if((error = pthread_rwlock_rdlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    if((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        tempBlockNode->urlKey1 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1;
        tempBlockNode->urlKey2 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2;
        tempBlockNode->pointer = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].pointer;
        tempBlockNode->other1 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other1;
        tempBlockNode->other2 = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other2;
        
        if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;   
    }
    
    ABU_GENERALDB_DICT_BLOCK_S* tempBlock = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                tempBlockNode->urlKey1 = tempBlock->BlockNodeArray[i].urlKey1;
                tempBlockNode->urlKey2 = tempBlock->BlockNodeArray[i].urlKey2;
                tempBlockNode->pointer = tempBlock->BlockNodeArray[i].pointer;
                tempBlockNode->other1 = tempBlock->BlockNodeArray[i].other1;
                tempBlockNode->other2 = tempBlock->BlockNodeArray[i].other2;
                
                if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}

/**
@
@
@
@
*/
bool ABU_GENERALDB_DICT_mod(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2, 
        const ABU_GENERALDB_DICT_BLOCKINODE_S* tempBlockNode)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if((error = pthread_rwlock_wrlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    if((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].pointer = tempBlockNode->pointer;
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other1 = tempBlockNode->other1;
        p->GeneralDB_Dict_BucketNodeArray[bucketIndex].other2 = tempBlockNode->other2;
        
        if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        return 1;   
    }
    
    ABU_GENERALDB_DICT_BLOCK_S* tempBlock = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                tempBlock->BlockNodeArray[i].pointer = tempBlockNode->pointer;
                tempBlock->BlockNodeArray[i].other1 = tempBlockNode->other1;
                tempBlock->BlockNodeArray[i].other2 = tempBlockNode->other2;
                
                if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}


/**
@
@
@
@
*/
bool ABU_GENERALDB_DICT_del(ABU_GENERALDB_DICT_S * p, UINT32 key1, UINT32 key2)
{
    int error;
    
    UINT64 urlHashValue = key1;
    urlHashValue = urlHashValue<<32;
    urlHashValue = urlHashValue|key2;
    
    UINT64 bucketIndex = urlHashValue%(p->dwNumOfDictBuckets);
    
    if((error = pthread_rwlock_wrlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    if((p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey1 == key1)&&
        (p->GeneralDB_Dict_BucketNodeArray[bucketIndex].urlKey2 == key2))
    {
        memset(&(p->GeneralDB_Dict_BucketNodeArray[bucketIndex]), 0, sizeof(ABU_GENERALDB_DICT_BUCKETINODE_S));
        
        if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
        {
            errno = error;
            return 0;
        }
        
        --(p->dwNumOfNodes);
        return 1;   
    }
    
    ABU_GENERALDB_DICT_BLOCK_S* tempBlock = p->GeneralDB_Dict_BucketNodeArray[bucketIndex].next;
    
    int i;
    
    while(tempBlock)
    {
        for(i = 0; i< p->sizeOfBlock; i++)
        {
            if((tempBlock->BlockNodeArray[i].urlKey1 == key1)&&
                (tempBlock->BlockNodeArray[i].urlKey2 == key2))
            {
                memset(&(tempBlock->BlockNodeArray[i]), 0, sizeof(ABU_GENERALDB_DICT_BUCKETINODE_S));
                            
                if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
                {
                    errno = error;
                    return 0;
                }
                
                --(p->dwNumOfNodes);
                return 1;   
        
            }
            else
                continue;
        }
        tempBlock = tempBlock->next; 
    }
    
    if((error = pthread_rwlock_unlock(&generalDBDictRwLock)))
    {
        errno = error;
        return 0;
    }
    
    return 0;   
}

/**
*/
void ABU_GENERALDB_DICT_free(ABU_GENERALDB_DICT_S * p)
{
    int i;
    
    ABU_GENERALDB_DICT_BLOCK_S* tempBlock;
    ABU_GENERALDB_DICT_BLOCK_S* nextBlock;
    
    for( i = 0; i<p->dwNumOfDictBuckets; i++)
    {
        tempBlock = p->GeneralDB_Dict_BucketNodeArray[i].next;
        
        while(tempBlock)
        {
            nextBlock = tempBlock->next;
            free(tempBlock->BlockNodeArray);
            free(tempBlock);
            tempBlock = nextBlock;
        }
    }
    
    free(p->GeneralDB_Dict_BucketNodeArray);
    free(p);
    p = NULL;
}
