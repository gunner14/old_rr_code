#ifndef __B2C_CLIENT_H__
#define __B2C_CLIENT_H__

#include "AbStd.h"
#include "PlatformClient.h"

enum {
    MSG_TYPE_QUERY_BIZ = 1,
    MSG_TYPE_QUERY_TAG_THREAD = 3,
    MSG_TYPE_QUERY_MULTI_TAG_THREAD = 5,
    MSG_TYPE_QUERY_ONE_THREAD = 7,
    MSG_TYPE_QUERY_THREAD_PAGE = 9,
    MSG_TYPE_QUERY_NEW_BIZ = 11,
    MSG_TYPE_QUERY_NEW_THREAD = 13,
    MSG_TYPE_QUERY_NEW_POST = 15,
    MSG_TYPE_QUERY_MULTI_THREAD = 17,
    MSG_TYPE_QUERY_MULTI_BIZ = 19,
    MSG_TYPE_QUERY_BIZ_TAG = 21,
    MSG_TYPE_QUERY_CATA_TAG = 23,
    MSG_TYPE_QUERY_USER_POST = 25,
    MSG_TYPE_QUERY_MULTI_POST = 27,
    MSG_TYPE_QUERY_ONE_POST = 29,
    MSG_TYPE_QUERY_POST_PAGE = 31,
    MSG_TYPE_QUERY_THREAD_POST_PAGE = 33,
    B2C_MSG_BIZ_CREATE = 101,
    B2C_MSG_BIZ_DELETE = 103,
    B2C_MSG_BIZ_UPDATE = 105,
    B2C_MSG_TAG_CREATE  =      201,
    B2C_MSG_TAG_DELETE  =      203,
    B2C_MSG_TAG_UPDATE  =      205,
    B2C_MSG_TAG_MOVE    =      207,
    B2C_MSG_TAG_DEL_THREAD =   209,
    B2C_MSG_TAG_MOVE_THREAD =  211,
    B2C_MSG_THREAD_CREATE = 301,
    B2C_MSG_THREAD_DELETE = 303,
    B2C_MSG_THREAD_UPDATE = 305,
    B2C_MSG_POST_CREATE  =      401,
    B2C_MSG_POST_DELETE  =      403,
    B2C_MSG_POST_UPDATE  =      405,
    B2C_MSG_SYNC_COMMIT =   503,    // backup synclog msg
    B2C_MSG_SYNC_REPORT =   505,    // sync report(bps to bbts)
    B2C_MSG_SYNC_STOP =     507
};

/*
* B2cReader read data from b2c
*/
class B2cReader{
public:
	enum{
		MAX_RECV_DATA_LEN = 1024*1024,
		B2C_MAX_PACKAGE_LEN = 64*1024,
		BUF_SIZE_ERROR = 2047,
	};
public:
	B2cReader():m_operator(MAX_RECV_DATA_LEN, B2C_MAX_PACKAGE_LEN){
			memset(m_szError, 0x00, BUF_SIZE_ERROR+1);
	}
	~B2cReader(){
	}
private:
	B2cReader(B2cReader const &);
	B2cReader & operator = (B2cReader const &);
public:
	/**
	* initialize the class
	* @param uiIPNum
	*        the number of IP in ppszIP
	* @param ppszIP
	*        array of IP
	* @param punPort
	*        array of port
	* @return 0 if successfully
	* @return -1 if failure
	*/
	int Init(UINT32 uiIPNum, char  const * const * ppszIP, UINT16* punPort);
	/**
	* reset key-value set to send
	* @return 0 if successfully
	* @return -1 if failure
	*/
	int ResetKV();
	/*
	* set key-value pair
	* @param szKey
	*		 key
	* @param szValue
	*		 value
	* @param uiValueLen
	*		 the length of szValue
	* @param bKV
	*		 mark if the key-value is a key-value package
	* @return 0 if success
	* @return -1 if failure
	*/
	int SetKV(char const * szKey, char const * szValue, UINT32 uiValueLen, bool bKeyValue = false);
	/*
	* set key-value pair
	* @param szKey
	*		 key
	* @param ullValue
	*		 value
	* @return 0 if success
	* @return -1 if failure
	*/
	int SetKV(char const * szKey, UINT64 ullValue);
	/*
	* use key-value to query one biz
	* @return 0 if success
	* @return -1 if failure
	*/
	int QueryOneBiz(){
		return Fetch(MSG_TYPE_QUERY_BIZ);	
	}
	/*
	* use key-value to query one thread
	* @return 0 if success
	* @return -1 if failure
	*/
	int QueryOneThread(){
		return Fetch(MSG_TYPE_QUERY_ONE_THREAD);
	}
	/*
	* use key-value to query one post
	* @return 0 if success
	* @return -1 if failure
	*/
	int QueryOnePost(){
		return Fetch(MSG_TYPE_QUERY_ONE_POST);
	}
	/*
	* use key-value to query using msg-type
	* @param unMsgType
	*		 message type
	* @return 0 if success
	* @return -1 if failure
	*/
	int Query(UINT16 unMsgType){
		return Fetch(unMsgType);
	}
	/*
	* return received data
	*/
	char const * GetData() const {
		return m_operator.GetData();
	}
	/*
	* return the length of the received data
	*/
	UINT32 GetDataLen() const {
		return m_operator.GetDataLen();
	}
	/*
	* get key-value pair by key
	* @param szKey
	*		 key
	* @param uiValueLen
	*		 return the value's length
	* @param bKV
	*		 output parameter, mark if value is key-value package
	* @return NULL if failure, else return value
	*/
	char const * GetKV(char const * szKey, UINT32 & uiValueLen, bool & bKV);
	/*
	* return error message if operate failed
	*/
	char const * GetError() const {
		return m_szError;
	}
private:
	int Fetch(UINT16 unMsgType);
private:
	PlatformOperator	m_operator;
	char		m_szError[BUF_SIZE_ERROR+1];
};

/*
* B2cWriter write data to b2c
*/
class B2cWriter{
public:
	enum{
		MAX_RECV_DATA_LEN = 64*1024,
		B2C_MAX_PACKAGE_LEN = 64*1024,
		B2C_RET_ERR_MSG_SIZE = 2047,
		BUF_SIZE_ERROR = 2047,
	};
public:
	B2cWriter():m_operator(MAX_RECV_DATA_LEN, B2C_MAX_PACKAGE_LEN){
			memset(m_szRetErrMsg, 0x00, B2C_RET_ERR_MSG_SIZE+1);
			memset(m_szError, 0x00, BUF_SIZE_ERROR+1);
	}
	~B2cWriter(){
	}
private:
	B2cWriter(B2cWriter const &);
	B2cWriter & operator = (B2cWriter const &);
public:
	/**
	* initialize the class
	* @param uiIPNum
	*        the number of IP in ppszIP
	* @param ppszIP
	*        array of IP
	* @param punPort
	*        array of port
	* @return 0 if successfully
	* @return -1 if failure
	*/
	int Init(UINT32 uiIPNum, char  const * const * ppszIP, UINT16* punPort);
	/**
	* reset key-value set to send
	* @return 0 if successfully
	* @return -1 if failure
	*/
	int ResetKV();
	/*
	* set key-value pair
	* @param szKey
	*		 key
	* @param szValue
	*		 value
	* @param uiValueLen
	*		 the length of szValue
	* @param bKV
	*		 mark if the key-value is a key-value package
	* @return 0 if success
	* @return -1 if failure
	*/
	int SetKV(char const * szKey, char const * szValue, UINT32 uiValueLen, bool bKeyValue = false);
	/*
	* set key-value pair
	* @param szKey
	*		 key
	* @param ullValue
	*		 value
	* @return 0 if success
	* @return -1 if failure
	*/
	int SetKV(char const * szKey, UINT64 ullValue);
	/*
	* use key-value to create biz
	* @return 0 if success
	* @return -1 if failure
	*/
	int CreateBiz(){
		return Put(B2C_MSG_BIZ_CREATE);	
	}
	/*
	* use key-value to update biz
	* @return 0 if success
	* @return -1 if failure
	*/
	int UpdateBiz(){
		return Put(B2C_MSG_BIZ_UPDATE);	
	}
	/*
	* use key-value to delete biz
	* @return 0 if success
	* @return -1 if failure
	*/
	int DeleteBiz(){
		return Put(B2C_MSG_BIZ_DELETE);	
	}
	/*
	* use key-value to create thread
	* @return 0 if success
	* @return -1 if failure
	*/
	int CreateThread(){
		return Put(B2C_MSG_THREAD_CREATE);	
	}
	/*
	* use key-value to update thread
	* @return 0 if success
	* @return -1 if failure
	*/
	int UpdateThread(){
		return Put(B2C_MSG_THREAD_UPDATE);	
	}
	/*
	* use key-value to delete thread
	* @return 0 if success
	* @return -1 if failure
	*/
	int DeleteThread(){
		return Put(B2C_MSG_THREAD_DELETE);	
	}
	/*
	* use key-value to create post
	* @return 0 if success
	* @return -1 if failure
	*/
	int CreatePost(){
		return Put(B2C_MSG_POST_CREATE);	
	}
	/*
	* use key-value to update post
	* @return 0 if success
	* @return -1 if failure
	*/
	int UpdatePost(){
		return Put(B2C_MSG_POST_UPDATE);	
	}
	/*
	* use key-value to delete post
	* @return 0 if success
	* @return -1 if failure
	*/
	int DeletePost(){
		return Put(B2C_MSG_POST_DELETE);	
	}
	/*
	* use key-value to write using msg-type
	* @param unMsgType
	*		 message type
	* @return 0 if success
	* @return -1 if failure
	*/
	int Write(UINT16 unMsgType){
		return Put(unMsgType);
	}
	/*
	* return received data
	*/
	char const * GetData() const {
		return m_operator.GetData();
	}
	/*
	* return the length of the received data
	*/
	UINT32 GetDataLen() const {
		return m_operator.GetDataLen();
	}
	/*
	* return the received error msg
	*/
	char const * GetRetErrMsg() const {
		return (NULL != m_szRetErrMsg)?m_szRetErrMsg:"";
	}
	/*
	* return the received retcode
	*/
	UINT32 GetRetCode() const {
		return m_uiArrRet[0];
	}
	/*
	* return the received bid
	*/
	UINT32 GetRetBid() const {
		return m_uiArrRet[1];
	}
	/*
	* return the received tid
	*/
	UINT32 GetRetTid() const {
		return m_uiArrRet[2];
	}
	/*
	* return the received pid
	*/
	UINT32 GetRetPid() const {
		return m_uiArrRet[3];
	}
	/*
	* return error message if operate failed
	*/
	char const * GetError() const {
		return m_szError;
	}
private:
	int Put(UINT16 unMsgType);
private:
	PlatformOperator	m_operator;
	UINT32				m_uiArrRet[4];
	char				m_szRetErrMsg[B2C_RET_ERR_MSG_SIZE+1];
	char		m_szError[BUF_SIZE_ERROR+1];
};


#endif //__B2C_CLIENT_H__

