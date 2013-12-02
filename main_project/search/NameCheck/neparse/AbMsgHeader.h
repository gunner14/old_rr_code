/*********************************************************
*Filename: AbMsgHeader.h
*Author: wbdiao
*Version: 0.1v
*Date:  2007-03-05
*Description: 
	It's the defination from msg's header struct and 
	data struct.
*Note:
***********************************************************/

#ifndef __AB_MSG_HEADER_H__
#define __AB_MSG_HEADER_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AbTypeDef.h"

#define  AB_MSG_HEADER_LEN_   12

/**
    it's the defination for the msg header.
*/
typedef struct AB_MSG_HEADER_S{
    UINT32    m_uiMsgLen;  //data len, not include head.
    UINT16    m_unMsgType;  //msg type
    UINT16    m_unMsgItemNum; //the number for key/value
    UINT16    m_unVersion;  //msg's version number
    UINT16    m_unCheckSum; //msg head's checksum.
}AB_MSG_HEADER_S;

/**
    it's the defination for a key/value struct
*/
typedef struct AB_MSG_DATA_ITEM_S{
    char *    m_szKey; //the key 
    char *    m_szData; //the data
    UINT16   m_unKeyLen; //the key's length in byte
    UINT32   m_uiDataLen; //the data's length in byte
    bool     m_bKeyValue;
}AB_MSG_DATA_ITEM_S;

/**
    the chain for the key/value pair.
*/
typedef struct AB_MSG_DATA_S{
    UINT16    m_unNum; //the count for key/value pairs.
    bool      m_bAlloc; //false, AB_MSG_DATA_ITEM_S's key/value memory need be free.
                        //otherwize, it uses buf's memory.
    AB_MSG_DATA_ITEM_S*  m_keyValues; //the head for key/value pairs
}AB_MSG_DATA_S;


/**
    the following is the method to operate the msg header and data.
 */

/**
@name  AB_MSG_PACK_HEAD
@desc  pack the header to char buf and convert the host code to net code
@param 
       szbuf: the buffer to accept the encoded header,all number in net code;
       header: the msg header, all number member in host code;
@return
       void
*/
void  AB_MSG_PACK_HEAD(char* szBuf, AB_MSG_HEADER_S const*  header);

/**
@name  AB_MSG_UNPACK_HEAD
@desc  it does the reverse work for AB_MSG_PACK_HEAD
@param 
       szbuf: the buffer of an encoded header,all number in net code;
       header: the msg header;
@return
       -1:failure, 0:success
*/
int  AB_MSG_UNPACK_HEAD(char const* szBuf, AB_MSG_HEADER_S* header);


/**
@name  AB_MSG_PACK_DATA
@desc  pack the key/value pairs to buf for send.
@param 
       szbuf: buf to accept the packed data, it's alloced by the method;
       len: length of the data in buf;
       data: the key/value pair to pack.
@return
       -1: failure
       0 : success
*/
int  AB_MSG_PACK_DATA(char** szbuf, UINT32* len, AB_MSG_DATA_S const* data);
/**
@name  AB_MSG_UNPACK_DATA
@desc  It does the reverse work for AB_MSG_PACK_DATA.
@param 
       szbuf: msg data's buffer
       uiBufLen; msg data length
       unItemNum: number of key/value in buf;
       bAlloc:   true,alloc the key/value memory in AB_MSG_DATA_ITEM_S,
                 otherwize, use the memory in buf.
       data: the key/value pair for unpack.
@return
       -1: failure
       0 : success
*/
int  AB_MSG_UNPACK_DATA(char const *szBuf, UINT32 uiBufLen, UINT16 unItemNum, bool bAlloc, AB_MSG_DATA_S* data);
/**
@name
    AB_MSG_FREE_DATA
@desc
    Free the key/value pairs according to the m_bAlloc sign.
    and reset the data's other data member.
@param
    data:  AB_MSG_DATA_S's data struct.
@return
    void
*/
void  AB_MSG_FREE_DATA(AB_MSG_DATA_S* data);
/**
@name
    AB_MSG_GET_KEY_VALUE_BY_INDEX
@desc
    Get unIndex pos key/value from the szBuf.
@param
    szBuf:  data buf
    uiLen:  length of buf.
    uiIndex: the No. for key/value pair in buf.
    item: key/value pair.
    bAlloc: true, alloc the memory for key/value,
            false, not alloc the memory for key/value.
@return
    0: success
    -1: failure
*/
int  AB_MSG_GET_KEY_VALUE_BY_INDEX(char const *szBuf, UINT32 uiLen, UINT16 unIndex, AB_MSG_DATA_ITEM_S* item, bool bAlloc);
/**
@name
    AB_MSG_GET_KEY_VALUE_BY_KEY
@desc
    Get unIndex pos key/value from the szBuf.
@param
    szBuf:  data buf
    uiLen:  length of buf.
    szKey: key name for key/value pair in buf.
    item: key/value pair.
    bAlloc: true, alloc the memory for key/value,
            false, not alloc the memory for key/value.
@return
    0: success
    -1: failure
*/
int  AB_MSG_GET_KEY_VALUE_BY_KEY(char const *szBuf, UINT32 uiLen, char const* szKey, AB_MSG_DATA_ITEM_S* item, bool bAlloc);

/**
@name
    AB_MSG_APPEND_KEY_VALUE
@desc
    Append a new key/value pair to the szBuf.
    if szBuf is less the neccessary space, it will realloc it.
@param
    szBuf:  data buf
    uiLen:  input the length of buf and output the new length of the buf.
    uiStart: input the pos to append and output the new pos for next append.
    szKey: key for key/value pair.
    szValue: value for key/value pair.
    len:  length for szValue.
@return
    0: success
    -1: failure
*/
int  AB_MSG_APPEND_KEY_VALUE(char **szBuf, UINT32* uiLen, UINT32* uiStart, char const* szKey, char const* szValue, UINT32 len, bool bKeyValue = false);

/**
@name
    AB_MSG_APPEND_KEY_VALUE_BUF
@desc
    Append a new key/value pair to the szBuf.
    if szBuf is less the neccessary space, return -1.
@param
    szBuf:  data buf
    uiLen:  input the length of buf and output the new length of the buf.
    uiStart: input the pos to append and output the new pos for next append.
    szKey: key for key/value pair.
    szValue: value for key/value pair.
    len:  length for szValue.
@return
    0: success
    -1: failure
*/
int  AB_MSG_APPEND_KEY_VALUE_BUF(char *szBuf, UINT32 uiLen, UINT32* uiStart, char const* szKey, char const* szValue, UINT32 len, bool bKeyValue = false);

int  AB_MSG_APPEND_KEY_VALUE_UINT32(char *szBuf, UINT32 uiLen, UINT32* uiStart, char const* szKey, UINT32 value);

int  AB_MSG_APPEND_KEY_VALUE_UINT16(char *szBuf, UINT32 uiLen, UINT32* uiStart, char const* szKey, UINT16 value);
/**
Get ITEM's len
*/
UINT32 AB_MSG_GET_ITEM_LEN(UINT16 unKeyLen, UINT32 unDataLen);


#endif

