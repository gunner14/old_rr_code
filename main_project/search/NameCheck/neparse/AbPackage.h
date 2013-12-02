#ifndef  __AB_PACKAGE_H__
#define  __AB_PACKAGE_H__

#include "AbCommon.h"
#include "AbMsgHeader.h"
#include "AbStd.h"



class  AbPackage{
public:
    class AbPackageItem{
    public:
        AB_MSG_DATA_ITEM_S const* m_pItem;
        AbPackageItem* m_next;
    };
//    typedef map<char const*, AB_MSG_DATA_ITEM_S*, AbCharCaseLess >  AB_KEYVALUE_HASH_MAP;
    enum{
        ERR_MSG_LEN=511
    };
    AbPackage(){
        m_unMaxKeyNum = 60;
        m_pPackageItem = new AbPackageItem[m_unMaxKeyNum];
        memset(m_arrKeyHash, 0x00, sizeof(m_arrKeyHash));
        m_pKeyValue = new AB_MSG_DATA_ITEM_S[m_unMaxKeyNum];
        m_unKeyNum = 0;
        m_uiMaxPackMsgLen = 1024 * 1024; //1M
        m_szPackMsg = new char[m_uiMaxPackMsgLen + 4];
        m_uiCurPackPos = 0;
        m_bWithHead = true;
        memset(m_szErr, 0x00, ERR_MSG_LEN + 1);
        m_unOffset = 0;
        m_bFree = true;
    }
    AbPackage(UINT16 unKeyNum, UINT32 uiMaxPackMsgLen){
        m_unMaxKeyNum = unKeyNum;
        m_pPackageItem = new AbPackageItem[m_unMaxKeyNum];
        memset(m_arrKeyHash, 0x00, sizeof(m_arrKeyHash));
        m_pKeyValue = new AB_MSG_DATA_ITEM_S[m_unMaxKeyNum];
        m_unKeyNum = 0;
        m_uiMaxPackMsgLen = uiMaxPackMsgLen;
        if (m_uiMaxPackMsgLen < AB_MSG_HEADER_LEN_)  m_uiMaxPackMsgLen = AB_MSG_HEADER_LEN_;
        m_szPackMsg = new char[m_uiMaxPackMsgLen + 4];
        m_uiCurPackPos = 0;
        m_bWithHead = true;
        memset(m_szErr, 0x00, ERR_MSG_LEN + 1);
        m_unOffset = 0;
        m_bFree = true;
    }

    AbPackage(UINT16 unKeyNum, char* buffer, UINT32 uiBufferLen) {
        m_unMaxKeyNum = unKeyNum;
        m_pPackageItem = new AbPackageItem[m_unMaxKeyNum];
        memset(m_arrKeyHash, 0x00, sizeof(m_arrKeyHash));
        m_pKeyValue = new AB_MSG_DATA_ITEM_S[m_unMaxKeyNum];
        m_unKeyNum = 0;
        m_uiMaxPackMsgLen = uiBufferLen;
        m_szPackMsg = buffer;
        m_uiCurPackPos = 0;
        m_bWithHead = false;
        memset(m_szErr, 0x00, ERR_MSG_LEN + 1);
        m_unOffset = 0;
        m_bFree = false;
    }

    ~AbPackage(){
//        m_keyHashMap.clear();
        if(m_pKeyValue){
            delete [] m_pKeyValue;
        }
        if (m_pPackageItem) delete [] m_pPackageItem;
        if(m_bFree) {
            if(m_szPackMsg){
                delete [] m_szPackMsg;
            }
        }
    }
public:
    //0: success, -1: failure.
    int  UnPackMsg(char const* szMsg, UINT32 uiMsgLen);
    int  UnPackSelf();
    UINT16 GetKeyNum() const { return m_unKeyNum;}

    //NULL: not exist, otherwise, return the key/value
    AB_MSG_DATA_ITEM_S const*  GetKey( char const* szKey) const{
        UINT8 ucKey = GetHaskKey(szKey);
        AB_MSG_DATA_ITEM_S const* pItem = NULL;
        AbPackageItem* pKeys = this->m_arrKeyHash[ucKey];
        while(pKeys){
            if (strcasecmp(szKey, pKeys->m_pItem->m_szKey) == 0) {
                pItem = pKeys->m_pItem;
                break;
            }
            pKeys = pKeys->m_next;
        }
        return pItem;
    }
    //NULL: not exist, otherwise, return the key/value, index from 0.
    AB_MSG_DATA_ITEM_S const*  GetKey(UINT16 index) const{
        if (m_unKeyNum <= index) return NULL;
        return m_pKeyValue + index ;
    }
    int GetUShortValueFromKey(char const* szKey, UINT16& value) const {
        AB_MSG_DATA_ITEM_S const* item = GetKey(szKey);
        if(NULL == item) return -1;
        value = strtoul(item->m_szData, NULL, 0);
        return 0;
    }

    int GetUIntValueFromKey(char const* szKey, UINT32& value) const {
        AB_MSG_DATA_ITEM_S const* item = GetKey(szKey);
        if(NULL == item)
            return -1;
        value = strtoul(item->m_szData, NULL, 0);
        return 0;
    }

    int GetIntValueFromKey(char const* szKey, int& value) const{
        AB_MSG_DATA_ITEM_S const* item = GetKey(szKey);
        if(NULL == item)
            return -1;
        value = strtol(item->m_szData, NULL, 0);
        return 0;
    }

    int GetUllValueFromKey(char const* szKey, UINT64& value) const {
        AB_MSG_DATA_ITEM_S const* item = GetKey(szKey);
        if(NULL == item)
            return -1;
        value = strtoull(item->m_szData, NULL, 0);
        return 0;
    }

    void BeginPackMsg(bool bWithHead = true, UINT16 unOffset=0);
    //0: success, -1:exceed the memory
    int AddKeyValue(char const* szKey, char const* szData, UINT32 uiDataLen, bool bKeyValue = false);
    //0: success, -1:exceed the memory
    int AddKeyValue(char const* szKey, UINT16 unData);
    //0: success, -1:exceed the memory
    int AddKeyValue(char const* szKey, UINT32 uiData);
    //0: success, -1:exceed the memory
    int AddKeyValue(char const* szKey, UINT64 ullData);
	//0: success, -1: length error
	int AddOffsetData(char const* szData, UINT16 unOffsetDataLen){
		if (unOffsetDataLen != this->m_unOffset){
			sprintf(m_szErr, "Package's offset is %u, but added data's length is %u", m_unOffset, unOffsetDataLen);
			return -1;
		}
		memcpy(m_szPackMsg + (m_bWithHead?AB_MSG_HEADER_LEN_:0), szData, m_unOffset);
		return 0;
	}
    //0: success, -1:failure, pack tcp-msg
    int PackMsg(UINT16 unMsgType, UINT16 unVersion);
    //0: success, -1:failure, pack key/value msg
    int PackMsg();

    UINT32  GetPackedMsgSize() const { return m_uiCurPackPos;}
    char const* GetPackedMsg() const { return m_szPackMsg;}

    char const* GetErrMsg() const { return m_szErr;}
private:
    UINT8 GetHaskKey(char const* szKey) const{
        UINT8 ucKey = 0;
        if (szKey){
            UINT32 i = 0;
            while(szKey[i]) {
                ucKey +=((szKey[i]>='a')&&(szKey[i]<='z'))?(UINT8)(szKey[i]-'a'+'A'):(UINT8)szKey[i];
                i++;
            }
        }
        return ucKey;
    }
private:
    //key's hash map
//    AB_KEYVALUE_HASH_MAP  m_keyHashMap;
    AbPackageItem*          m_pPackageItem;
    AbPackageItem*          m_arrKeyHash[256];
    //key/value object array
    AB_MSG_DATA_ITEM_S*     m_pKeyValue;
    //m_pKeyValue's max capacity
    UINT16                  m_unMaxKeyNum;
    //exsiting key/value num in m_pKeyValue
    UINT16                  m_unKeyNum;
    //memory buf for pack message
    char*                   m_szPackMsg;
    //max length for memory buf --->m_szPackMsg;
    UINT32                  m_uiMaxPackMsgLen;
    //used's memory buf---m_szPackMsg;
    UINT32                  m_uiCurPackPos;
    //append the head
    bool                    m_bWithHead;
    //error msg for error
    char    m_szErr[ERR_MSG_LEN + 1];
    //tmp header
    AB_MSG_HEADER_S         m_header;
    //offset
    UINT16                  m_unOffset;
    //need to free memory
    bool                    m_bFree;
};

#endif
