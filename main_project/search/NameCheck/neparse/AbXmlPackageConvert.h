#ifndef __AB_XML_PACKAGE_CONVERT_H__
#define __AB_XML_PACKAGE_CONVERT_H__

#include "AbPackage.h"
#include "AbConfigResource.h"
#include "AbUtf8ReplaceMachine.h"

class AbXmlPackageConvert{
public:
    AbXmlPackageConvert(){
        memset(m_szError, 0x00, 512);
        m_pXmlReplace = NULL;
        m_szKeyBuf = NULL;
    }
    ~AbXmlPackageConvert(){
        if (m_pXmlReplace) delete m_pXmlReplace;
        if (m_szKeyBuf) delete m_szKeyBuf;
    }
public:
    int Init();
    int ConvertXmlToKv(char const * pszSrc, UINT32 uiSrcLen, char* szDesc, UINT32& uiDescLen);
    int ConvertKvToXml(char const* pszRoot, char const * pszSrc, UINT32 uiSrcLen, char* szDesc, UINT32& uiDescLen);
    int ConvertKvToXmlNode(char const* pszRoot, AbPackage* pPackage, char* szDesc, UINT32& uiDescLen);
    int ConvertKvToXmlNode(char const* pszNodeName, char const * pszSrc, UINT32 uiSrcLen, char* szDesc, UINT32& uiDescLen);
    char const* GetError() const{ return m_szError;}
private:
    int ConvertXmlToKv(apr_xml_elem const * tree, char* szDesc, UINT32& uiDescLen);
    int ConvertKvToXml(AB_MSG_DATA_ITEM_S const& item, char* szDesc, UINT32& uiDescLen);
private:
    AbUtf8ReplaceMachine*   m_pXmlReplace;
    char m_szError[512];
    char*                    m_szKeyBuf;
	UINT32					 m_uiKeyBufLen;
};

#endif
