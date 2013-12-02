/*******************************************************************************
* File name: AB_UTILITY_PACK.h
* Author: zhangyong  Version: 0.2  Date: 13/01/2007
* Description: 
*
*******************************************************************************/
#ifndef _AB_UTILITY_PACK_H
#define _AB_UTILITY_PACK_H 

#include "AB_UTILITY_COMMON.h"

#define MAX_NV_NUM   32
#define MAX_NAME_LEN  20
#define MAX_VALUE_LEN  256

#define MAX_READ_LEN 4096

// protocol macroes
#define COMPRESSED 0x01
#define UNCOMPRESSED 0x00

#define ABU_PACK_VERSION 1

/** 
  * @brief protocol header
  */
typedef struct {
	UINT8 version;
	UINT32 length;
	UINT8 flag;
}ABU_PACK_HEAD_S;

/** 
  * @brief Packet APP struct
  */
typedef struct {
	// header
	char  szName[MAX_NV_NUM][MAX_NAME_LEN];
	char  szValue[MAX_NV_NUM][MAX_VALUE_LEN];
	UINT16 wNumOfNVPair;
	UINT32 dwBufferSize;
	// body
	char *sBuffer;
}ABU_PACK_S;


#ifdef AB_DEBUG 
#define pack_print(pack) do { \
    assert(pack!=NULL);\
    ABU_LOG_print(ABU_LOG_LEVEL_DEBUG, "+++++++++++++++++++++%-20s:%-20s+++++++++++++++++++++++++++++++\n", (char*)__FILE__,(char*) __FUNCTION__);\
    int i = 0;\
    for (i=0;i< pack->wNumOfNVPair; i++) {\
        ABU_LOG_print(ABU_LOG_LEVEL_DEBUG, "name:%-20s\tvalue:%s\n", pack->szName[i], pack->szValue[i]);\
    }\
    ABU_LOG_print(ABU_LOG_LEVEL_DEBUG, "bufferLen:%10d\n", pack->dwBufferSize);\
    ABU_LOG_print(ABU_LOG_LEVEL_DEBUG, "++++++++++++++++++++++++++++++++++++++++++++++++++++\n");\
} while (0);
#endif



int writen(int fd, const void *buf, size_t n);

int readn(int fd, void *buf, size_t n);

ABU_PACK_S * ABU_PACK_create(void);

void ABU_PACK_free(ABU_PACK_S **pPack);

/** 
  * @brief You have to malloc mem for sBuf and free it by yourself.
  * 
  * @param pPack 
  * @param sBuf 
  * @param wBufSize  
  * 
  * @return  0: succ / -1: failed
  */
int ABU_PACK_putBuf(ABU_PACK_S *pPack, char *sBuf, UINT32 wBufSize);

/** 
  * @brief return the specified buffer in the pack
  * 
  * @param pPack 
  * 
  * @return 
  */
char * ABU_PACK_getBuf(ABU_PACK_S *pPack);

/** 
  * @brief get the value for a specified name
  * 
  * @param pPack 
  * @param szName 
  * 
  * @return 
  */
char* ABU_PACK_getValue(ABU_PACK_S* pPack,char* szName);
/** 
  * @brief to get the invalue for a specified name
  * 
  * @param ABU_PACK_S* 
  * @param szName 
  * 
  * @return 
  */
int ABU_PACK_getIntValue(ABU_PACK_S*, char *szName);

/**
	@brief to set a value for a name
	@return  
         -1: faild as the length of name or value is not valid
		 -2: failed due to an internal error
		 -3: pPack is NULL
		 -4: NUM > MAX_NV_NUM
		   0:  succ to add 
		   1:  the same name has been there but the value is updated
*/
int ABU_PACK_putValue(ABU_PACK_S *, char * szName, char * szValue);
int ABU_PACK_putIntValue(ABU_PACK_S*, char *szName, int value);

/**
	@brief read a package from a BLOCK socket
	@param [in] fd
	@param [out] pPack
	@param [out] pBytesLeft the left unread bytes in the socket 
	@return 
        >0:  the byte size of read
		-1: failed for this packet. You can continue read next packet
		-2: fatal error. The socket needs be reconnected 
		-3: need to skip left bytes indicated by  *pBytesLeft
*/
int ABU_NET_readPack(int fd,  ABU_PACK_S *  pPack,int *pBytesLeft); 		  // Block
/**
	@brief read a package from a NON-BLOCK socket
	@param [in] fd
	@param [in] the timeout value to read
	@param [out] pPack
	@param [out] pBytesLeft  the bytes unread left
	@return >0:  the byte size of read
            -1: timeout for this packet, left bytes indicated by  *pBytesLeft
            -2: fatal error. The socket needs be reconnected
            -3: other error, need to read left bytes indicated by  *pBytesLeft
*/
int ABU_NET_readPackTO(int fd, ABU_PACK_S * pPack, int timeout, int * pBytesLeft); // NON-Block


/** 
  * @brief used to read packet buffer when the packet is too big to be read once.BLOCK
  * 
  * @param fd 
  * @param ptr 
  * @param nbytes 
  * 
  * @return 
  */
int ABU_NET_readAll(int fd, void * ptr, int nbytes);

/** 
  * @brief used to read packet buffer when the packet is too big to be read once.NON-BLOCK 
  * 
  * @param fd 
  * @param ptr 
  * @param nbytes 
  * @param timeout 
  * 
  * @return 
  */
int ABU_NET_readAllTO(int fd, void * ptr, int nbytes, int timeout);

/** 
  * @brief read to end. only to be used in case of errors 
  * 
  * @param fd 
  * 
  * @return   0: succ ; < 0: failed
  */
int ABU_NET_readToEnd(int fd);

/** 
  * @brief read to end. only to be used in case of errors 
  * 
  * @param fd 
  * @param timeout 
  * 
  * @return 
  */
int ABU_NET_readToEndTO(int fd, int timeout);

/**
	@brief     used to write to fd with pPack (block)
	@return  -1: faild 
		    >0: bytes has write
*/
int ABU_NET_writePack(int fd, ABU_PACK_S * pPack);

/** 
  * @brief to write a pack into socket after compress its buffer
  *         Note: only pPack->sBuffer is compressed
  * 
  * @param fd 
  * @param pPack 
  * 
  * @return 
  */
int ABU_NET_writePackCompr(int fd, ABU_PACK_S * pPack);

/** 
  * @brief  be equal to writen (BLOCK)
  * 
  * @param fd 
  * @param buf 
  * @param n 
  * 
  * @return 
  */
int ABU_NET_writeAll(int fd, char * buf, int n);
#endif

