#ifndef __AB_UTF8_REPLACE_MACHINE_H__
#define __AB_UTF8_REPLACE_MACHINE_H__
#include <string.h>
class AbUtf8ReplaceCharItem{
public:
    AbUtf8ReplaceCharItem(){
        memset(m_arrNextCharItem, 0x00, sizeof(AbUtf8ReplaceCharItem*) * 256);
        m_szRepStr = NULL;
        m_bHasChild = false;
    }
    ~AbUtf8ReplaceCharItem(){
        for (int i=0; i<256; i++){
            if (m_arrNextCharItem[i])  delete m_arrNextCharItem[i];
        }
    }
public:
public:
    AbUtf8ReplaceCharItem*  m_arrNextCharItem[256];
    char*                m_szRepStr;
    bool                 m_bHasChild;
};

class AbUtf8ReplaceMachine{
public:
    AbUtf8ReplaceMachine(int iNum, bool bCase){
        m_bCase = bCase;
        m_iMaxStrNum = iNum;
        m_szRelStr = new char*[m_iMaxStrNum];
        for (int i=0; i<m_iMaxStrNum; i++){
            m_szRelStr[i] = NULL;
        }
        m_iCurNum = 0;
        
    }
    ~AbUtf8ReplaceMachine(){
        if (m_szRelStr){
            for (int i=0; i<m_iCurNum; i++){
                if (m_szRelStr[i]) delete [] m_szRelStr[i];
            }
            delete [] m_szRelStr;
        }
    }
public:
    //add the replace str.0:success, -1:failure
    void AddStr(char const* szUtf8, char const* szRepl);
    void Replace(char const* szUtf8, char* szOut, unsigned int &uiOutLen);
private:
    AbUtf8ReplaceCharItem const* IsExistWord(AbUtf8ReplaceCharItem const *pFilter, char const* szUtf8, unsigned char ucLen) const;

private:
    bool              m_bCase;
    AbUtf8ReplaceCharItem  m_filter;
    int     m_iMaxStrNum;
    int     m_iCurNum;
    char**  m_szRelStr;

};

#endif

