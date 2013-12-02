#ifndef __PLATFORM_CLIENT_H__
#define __PLATFORM_CLIENT_H__

#include "AbStd.h"
#include "AbStream.h"
#include "AbXmlPackageConvert.h"

/*
* PlatformStream connect multi IP, send and receive data from them;
* if one of them not work, it will connenct the other then send and receive data
*
*/
class PlatformStream{
public:
	enum{
		WAIT_MILLI_SECOND = 3000,

		BUF_SIZE_ERROR = 2047,
	};
public:
	PlatformStream(){
		m_uiCurIP = 0;
		m_uiIPNum = 0;
		m_bConnected = false;
		m_ppszIP = NULL;
		m_punPort = NULL;
		memset(m_szError, 0x00, BUF_SIZE_ERROR+1);
	}
	~PlatformStream(){
		Clear();
	}
private:
	PlatformStream(PlatformStream const &);
	PlatformStream & operator = (PlatformStream const &);
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
	* send and receive data, if current ip not work, it will choose others until no more
	* @param szSendData
	*		 data buffer to send
	* @param uiSendDataLen
	*		 data's length to send
	* @param header
	*		 received message's header
	* typedef struct AB_MSG_HEADER_S{
	*		UINT32    m_uiMsgLen;  //data len, not include head.
	*		UINT16    m_unMsgType;  //msg type
	*		UINT16    m_unMsgItemNum; //the number for key/value
	*		UINT16    m_unVersion;  //msg's version number
	*		UINT16    m_unCheckSum; //msg head's checksum.
	*	}AB_MSG_HEADER_S;
	* @param szRecvData
	*		 buffer for received data
	* @param uiRecvDataLen
	*		 szRecvData's size
	* @return 0 if success
	* @return -1 if failure
	*/
	int SendRecv(char const * szSendData, UINT32 uiSendDataLen, AB_MSG_HEADER_S& header, char * szRecvData, UINT32 uiRecvDataLen);
	/**
	* return the error msg if operate failed
	*/
	char const * GetError() const {
		return m_szError;
	}
private:
	int ReConnect();

	void Clear();
private:
	AbStream	m_stream;
	UINT32		m_uiCurIP;
	UINT32		m_uiIPNum;
	bool		m_bConnected;
	char**		m_ppszIP;
	UINT16*		m_punPort;
	char		m_szError[BUF_SIZE_ERROR+1];
};


/*
* PlatformPackage deal with data as AbPackage,
* and some good features are provided
*/
class PlatformPackage{
public:
	enum{
		DEF_MAX_DATA_SIZE = 2*1024*1024,
		MAX_KEY_LEVEL_NUM = 16,
		BUF_SIZE_ERROR = 2047
	};
public:
	/*
	* construct
	* @param uiMaxDataSize
	*		 max data size of the package
	*/
	PlatformPackage(UINT32 uiMaxDataSize = DEF_MAX_DATA_SIZE){
		m_uiMaxDataSize = uiMaxDataSize;
		m_pszDataBuf = new char[m_uiMaxDataSize];
		m_uiDataLen = 0;
		m_unKeyNum = 0;
		m_bWithHeader = false;
		m_xmlConvert.Init();
		memset(m_szError, 0x00, BUF_SIZE_ERROR+1);
	}
	~PlatformPackage(){
		delete [] m_pszDataBuf;
	}
	PlatformPackage(PlatformPackage const & package){
		m_uiMaxDataSize = package.m_uiMaxDataSize;
		m_pszDataBuf = new char[m_uiMaxDataSize];
		m_uiDataLen = package.m_uiDataLen;
		memcpy(m_pszDataBuf, package.m_pszDataBuf, m_uiDataLen);
		m_unKeyNum = package.m_unKeyNum;
		m_bWithHeader = package.m_bWithHeader;
		m_xmlConvert.Init();
		memcpy(m_szError, package.m_szError, BUF_SIZE_ERROR+1);
	}
	PlatformPackage & operator = (PlatformPackage const & package){
		if(this != &package){
			delete m_pszDataBuf;
			m_uiMaxDataSize = package.m_uiMaxDataSize;
			m_pszDataBuf = new char[m_uiMaxDataSize];
			m_uiDataLen = package.m_uiDataLen;
			memcpy(m_pszDataBuf, package.m_pszDataBuf, m_uiDataLen);
			m_unKeyNum = package.m_unKeyNum;
			m_bWithHeader = package.m_bWithHeader;
			memcpy(m_szError, package.m_szError, BUF_SIZE_ERROR+1);
		}
		return *this;
	}
public:
	/*
	* start to pack message
	* @param bWithHeader
	*		 mark if the package has header
	*/
	void BeginPackMsg(bool bWithHeader = false);
	/*
	* add a key-value pair
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
	int AddKeyValue(char const * szKey, char const * szValue, UINT32 uiValueLen, bool bKV = false);
	/*
	* add a key-value pair
	* @param szKey
	*		 key
	* @param ullValue
	*		 value
	* @return 0 if success
	* @return -1 if failure
	*/
	int AddKeyValue(char const * szKey, UINT64 ullValue);
	/*
	* end to pack the message
	* @param unMsgType
	*		 message type, which is used if the package has header
	* @param unVersion
	*		 message version, which is used if the package has header
	* @return 0 if success
	* @return -1 if failure
	*/
	int PackMsg(UINT16 unMsgType = 0, UINT16 unVersion = 0);
	/*
	* return the packed message's data
	*/
	char const * GetPackedMsg(){
		return m_pszDataBuf;
	}
	/*
	* return the packed message's length
	*/
	UINT32 GetPackedMsgSize(){
		return m_uiDataLen;
	}
	/*
	* unpack the input data to package
	* @param szData
	*		 data
	* @param uiDataLen
	*		 data's length
	* @return 0 if success
	* @return -1 if failure
	*/
	int UnPackMsg(char const * szData, UINT32 uiDataLen);
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
	char const * GetKey(char const * szKey, UINT32 & uiValueLen, bool & bKV);
	/*
	* get key-value pair by key
	* @param szKey
	*		 key
	* @param ullValue
	*		 return the value
	* @return 0 if success
	* @return -1 if failure
	*/
	int GetKey(char const * szKey, UINT64& ullValue);
	/*
	* get key-value pair by key
	* @param szKey
	*		 key
	* @param item
	*		 return key-value pair
	* typedef struct AB_MSG_DATA_ITEM_S{
	*		char *    m_szKey; //the key 
	*		char *    m_szData; //the data
	*		UINT16   m_unKeyLen; //the key's length in byte
	*		UINT32   m_uiDataLen; //the data's length in byte
	*		bool     m_bKeyValue;
	*	}AB_MSG_DATA_ITEM_S;
	* @return 0 if success
	* @return -1 if failure
	*/
	int GetKey(char const * szKey, AB_MSG_DATA_ITEM_S & item);
	/*
	* get key-value pair by index
	* @param item
	*		 return key-value pair
	* @param unKeyIndex
	*		 index, start from 0
	* @return 0 if success
	* @return -1 if failure
	*/
	int GetKey(AB_MSG_DATA_ITEM_S & item, UINT16 unKeyIndex);
	/*
	* get key-value pair by index from uiBeginPos
	* @param item
	*		 return key-value pair
	* @param unKeyIndex
	*		 index, start from 0
	* @param uiBeginPos
	*		 begin position, if get successfully, it will be modified to the next begin position
	*		 you can set unKeyIndex=0 and uiBeginPos=0 as iterator to get all key-value until return -1
	* @return 0 if success
	* @return -1 if failure
	*/
	int GetKey(AB_MSG_DATA_ITEM_S & item, UINT16 unKeyIndex, UINT32 & uiBeginPos);
	/*
	* convert the package to xml
	* @param szRoot
	*		 the root of xml
	* @param szBuf
	*		 buffer for xml
	* @param uiBufLen
	*		 input as buffer's size, and output as xml's length		 
	* @return 0 if success
	* @return -1 if failure
	*/
	int ToXml(char const * szRoot, char * szBuf, UINT32 & uiBufLen);
	/*
	* convert xml to package
	* @param szXml
	*		 xml data
	* @return 0 if success
	* @return -1 if failure
	*/
	int FromXml(char const * szXml);
	/*
	* return error message if operate failed
	*/
	char const * GetErrMsg() const {
		return m_szError;
	}
private:
	int StatKeyNum();
	int CheckKey(char const * szKey);
private:
	AbXmlPackageConvert	m_xmlConvert;
	UINT32	m_uiMaxDataSize;
	char *	m_pszDataBuf;
	UINT32	m_uiDataLen;
	UINT16	m_unKeyNum;
	bool	m_bWithHeader;
	char	m_szError[BUF_SIZE_ERROR+1];
};

/*
* PlatformOperator used as interface to the service provide by PLATFORM team
*/
class PlatformOperator{
public:
	enum{
		DEF_MAX_RECV_DATA_LEN = 2*1024*1024,
		DEF_MAX_SEND_DATA_LEN = 2*1024*1024,
		BUF_SIZE_ERROR = 2047,
	};
public:
	/*
	* constructor
	* @param uiMaxRecvDataLen
	*		 max length of received data
	* @param uiMaxSendDataLen
	*		 max length of sent data
	*/
	PlatformOperator(UINT32 uiMaxRecvDataLen = DEF_MAX_RECV_DATA_LEN, 
					UINT32 uiMaxSendDataLen = DEF_MAX_SEND_DATA_LEN):m_recvPack(uiMaxRecvDataLen),
			m_sendPack(uiMaxSendDataLen){
			m_uiMaxRecvDataLen = uiMaxRecvDataLen;
			m_pszData = new char[m_uiMaxRecvDataLen+1];
			memset(m_pszData, 0x00, m_uiMaxRecvDataLen);
			m_uiDataLen = 0;
			memset(m_szError, 0x00, BUF_SIZE_ERROR+1);
	}
	~PlatformOperator(){
		delete [] m_pszData;
	}
private:
	PlatformOperator(PlatformOperator const &);
	PlatformOperator & operator = (PlatformOperator const &);
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
	* send key-value to service and receive reply
	* @param unMsgType
	*		 send key-value use this message type
	* @return 0 if success
	* @return -1 if failure
	*/
	int Operator(UINT16 unMsgType);
	/*
	* return received data
	*/
	char const * GetData() const {
		return m_pszData;
	}
	/*
	* return the length of the received data
	*/
	UINT32 GetDataLen() const {
		return m_uiDataLen;
	}
	/*
	* deal with the received data as package, and unpack it
	* @return 0 if success
	* @return -1 if failure
	*/
	int UnPackRecv();
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
	* get key-value pair by key
	* @param szKey
	*		 key
	* @param ullValue
	*		 return the value
	* @return 0 if success
	* @return -1 if failure
	*/
	int GetKV(char const * szKey, UINT64 & ullValue);
	/*
	* return error message if operate failed
	*/
	char const * GetError() const {
		return m_szError;
	}
private:
	
private:
	PlatformStream	m_multiStream;
	char*		m_pszData;
	UINT32		m_uiDataLen;
	UINT32		m_uiMaxRecvDataLen;
	PlatformPackage	m_recvPack;
	PlatformPackage	m_sendPack;
	char		m_szError[BUF_SIZE_ERROR+1];
};


#endif //__PLATFORM_CLIENT_H__

