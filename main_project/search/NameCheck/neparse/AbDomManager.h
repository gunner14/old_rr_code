#ifndef __AB_DOM_MANAGER_H__
#define __AB_DOM_MANAGER_H__

#include "AbCommon.h"
#include "AbConfigResource.h"
#include"AbPackage.h"

class   AbDomField{
public:
    enum{
        DATA_TYPE_UINT8 = 0,
        DATA_TYPE_UINT16 = 1,
        DATA_TYPE_UINT32 =2,
        DATA_TYPE_UINT64 =3,
        DATA_TYPE_INT8  = 4,
        DATA_TYPE_INT16  =5,
        DATA_TYPE_INT32 = 6,
        DATA_TYPE_INT64  =7,
        DATA_TYPE_FLOAT = 8,
        DATA_TYPE_DOUBLE = 9,
        DATA_TYPE_CHAR = 10,
        DATA_TYPE_VARCHAR = 11,
        DATA_TYPE_DOM  = 12,
        DATA_TYPE_NUM = 13
    };

    AbDomField(string const& strName,
                       UINT16 unId,
                       string const& type = "varchar",
                       string const& min = string(),
                       string const& max = string(),
                       bool bOptional = true,
                       string const& def = string(),
                       UINT32 uiLen = 0,
                       string const& format= string(),
                       string const& fix= string()){
        m_strName = strName;
        m_unId = unId;
        m_ucDataType = GetType(type);
        m_strMin = min;
        m_strMax = max;
        m_bOptional = bOptional;
        if (m_ucDataType != DATA_TYPE_DOM)
            m_strDef = def;
        m_uiLen = uiLen;
        m_strFormat = format;
        m_strFix = fix;
    }
    AbDomField(string const& strName,
                       UINT16 unId,
                       UINT8 type = DATA_TYPE_VARCHAR,
                       string const& min = string(),
                       string const& max = string(),
                       bool bOptional = true,
                       string const& def = string(),
                       UINT32 uiLen = 0,
                       string const& format= string(),
                       string const& fix= string()){
        m_strName = strName;
        m_unId = unId;
        m_ucDataType = type;
        m_strMin = min;
        m_strMax = max;
        m_bOptional = bOptional;
        if (m_ucDataType != DATA_TYPE_DOM)
            m_strDef = def;
        m_uiLen = uiLen;
        m_strFormat = format;
        m_strFix = fix;
    }
    ~AbDomField(){
    }
public:
    string const& GetName() const { return m_strName;}
    UINT16 GetId() const { return m_unId;}
    UINT8 GetType() const { return m_ucDataType;}
    char const* GetTypeName() const { return m_arrTypeName[m_ucDataType];}
    string const& GetMin() const { return m_strMin;}
    string const& GetMax() const { return m_strMax;}
    bool IsOptional() const { return m_bOptional;}
    string const& GetDef() const { return m_strDef;}
    UINT32 GetLen() const { return m_uiLen;}
    string const& GetFormat() const { return m_strFormat;}
    string const& GetFix() const { return m_strFix;}
    bool IsFix() const { return m_strFix.length()>0;}
    bool IsValid(char const* szValue, UINT32 uiLen) const;
    UINT8 GetType(string const& type) const{
        for (UINT8 i=0; i<DATA_TYPE_NUM; i++){
            if (strcasecmp(type.c_str(), m_arrTypeName[i])==0){
                return i;
            }
        }
        return DATA_TYPE_VARCHAR;
    }

    char const* GetType(UINT8 ucType) const{
        if (DATA_TYPE_NUM <= ucType) return m_arrTypeName[DATA_TYPE_VARCHAR];
        return m_arrTypeName[ucType];
    }

    bool IsDom() const { return DATA_TYPE_DOM == m_ucDataType;}

    bool IsNumber() const { 
        return DATA_TYPE_DOUBLE>=m_ucDataType;
    }
    bool IsChar() const {
        return (DATA_TYPE_CHAR==m_ucDataType) || (DATA_TYPE_VARCHAR==m_ucDataType);

    }
    bool IsInteger() const {
        return DATA_TYPE_INT64>=m_ucDataType;        
    }
    bool IsFloat() const{
        return (DATA_TYPE_FLOAT==m_ucDataType) || (DATA_TYPE_DOUBLE==m_ucDataType);
    }
    bool IsUnsigned() const{
        return DATA_TYPE_UINT64>=m_ucDataType;        
    }
private:
    string              m_strName;
    UINT16              m_unId;
    UINT8               m_ucDataType;
    string              m_strMin;
    string              m_strMax;
    bool                m_bOptional;
    string              m_strDef;
    UINT32              m_uiLen;
    string              m_strFormat;
    string              m_strFix;
    static char*        m_arrTypeName[DATA_TYPE_NUM];
};

class  AbDomObject{
public:
    AbDomObject(string strName){
        m_strName = strName;
    }
    ~AbDomObject(){
        m_fieldNameHash.clear();

        hash_map<UINT16, AbDomField*>::iterator iter = m_fieldIdHash.begin();
        while(iter != m_fieldIdHash.end()){
            delete (*iter).second;
            iter++;
        }
        m_fieldIdHash.clear();
    }
public:
    string const& GetDomName() const { return m_strName;}
    UINT16 GetFieldNum() const { return m_fieldNameHash.size();}
    AbDomField const* GetFieldById(UINT16 id) const{
        hash_map<UINT16, AbDomField*>::const_iterator iter = m_fieldIdHash.find(id);
        if (iter != m_fieldIdHash.end()){
            return (*iter).second;
        }
        return NULL;
    }
    AbDomField const* GetFieldByIndex(UINT16 index) const{
        hash_map<UINT16, UINT16>::const_iterator iter = m_indexIdHash.find(index);
        if (iter == m_indexIdHash.end()) return NULL;
        return (*m_fieldIdHash.find((*iter).second)).second;//must exist
    }
    AbDomField const* GetField(string const& strFieldName) const{
        hash_map<string, AbDomField*, AbHashString>::const_iterator iter = m_fieldNameHash.find(strFieldName);
        if (iter != m_fieldNameHash.end()){
            return (*iter).second;
        }
        return NULL;
    }
    void GetFieldNames(list<string>& names) const{
        names.clear();
        hash_map<string, AbDomField*, AbHashString>::const_iterator iter = m_fieldNameHash.begin();
        if (iter != m_fieldNameHash.end()){
            names.push_front((*iter).first);
            iter++;
        }
    }
    void GetFieldIDs(list<UINT16>& ids) const{
        ids.clear();
        hash_map<UINT16, AbDomField*>::const_iterator iter = m_fieldIdHash.begin();
        if (iter != m_fieldIdHash.end()){
            ids.push_front((*iter).first);
            iter++;
        }
    }
    //0:success, 1: name exist, 2:id exist
    int AddField(string const& strName,
                       UINT16 unId,
                       string const& type = AbDomField::DATA_TYPE_VARCHAR,
                       string const& min = string(),
                       string const& max = string(),
                       bool bOptional = true,
                       string const& def = string(),
                       UINT32 uiLen = 0,
                       string format= string(),
                       string fix=string()){
        if (GetField(strName)) return 1;
        if (GetFieldById(unId)) return 2;
        AbDomField* dom = new AbDomField(strName,
                                             unId,
                                             type,
                                             min,
                                             max,
                                             bOptional,
                                             def,
                                             uiLen,
                                             format,
                                             fix);
        m_fieldNameHash[strName] = dom;
        m_indexIdHash[m_fieldIdHash.size()] = unId;
        m_fieldIdHash[unId] = dom;
        return 0;
    }
private:
    string      m_strName;
    hash_map<string, AbDomField*, AbHashString>  m_fieldNameHash;
    hash_map<UINT16, AbDomField*>  m_fieldIdHash;
    hash_map<UINT16/*index*/, UINT16/*id*/>  m_indexIdHash;
};

class  AbDomManager{
public:
    enum{
        MAX_FIELD_NUM = 512
    };
    AbDomManager(){}
    ~AbDomManager(){
        hash_map<string, AbDomObject*, AbHashString>::iterator iter = m_domHash.begin();
        while(iter != m_domHash.end()){
            delete (*iter).second;
            iter++;
        }
        m_domHash.clear();
    }
public:
    //0:success, -1:failure
    int AddDoms(char const* szDomDefXml, UINT32 uiLen);
    UINT32 GetDomCount() const {
        return m_domHash.size();
    }
    AbDomObject const* GetDomObject(string const& strDomName) const{
        hash_map<string, AbDomObject*, AbHashString>::const_iterator iter = m_domHash.find(strDomName);
        if (iter != m_domHash.end()) return (*iter).second;
        return NULL;
    }
    char const* GetError() const { return m_szError;}
    bool CheckField(AbPackage const* pPackage,
                               AbDomObject const* pDom, 
                               AB_MSG_DATA_ITEM_S const* pItem,
                               AbDomField const* pField,
                               AB_MSG_DATA_ITEM_S* pOutItem);
private:
    int AddDom(AbXmlTreeNodeData* pDomRoot);
private:
    hash_map<string, AbDomObject*, AbHashString>    m_domHash;
    char        m_szError[512];
};



#endif
