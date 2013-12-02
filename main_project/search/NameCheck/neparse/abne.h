#ifndef _ABNE
#define _ABNE
extern "C"
{
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_ADDR_RECOGNISE.h"
#include "AB_UTILITY_ADDR_SIMILARITY.h"
#include "AB_UTILITY_NAME.h"
#include "AB_UTILITY_NAME_SIMILARITY.h"   
#include "AB_UTILITY_TEL_RECOGNISE.h" //don't using AB_UTILITY_TEL.h
#include "AB_UTILITY_TEL_SIMILARITY.h"
#include "AB_UTILITY_MAPTREE.h"
}
#include "AbuVector.hpp"
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;

#ifndef UINT32
typedef unsigned int UINT32;
#endif

class ABNE
{
public:
    ABNE(list dictList);
    ~ABNE();

    struct ABAddress parseAddr(string addr);
    struct ABAddress parseAddr_bycity(string addr, string city);

    struct ABName parseName(string name);
    struct ABName parseName_bycity(string name, string city);
    
    struct ABTel parseTel(string phone);
    
    list recogAddrs(string sentence);//识别原始字符串
    list recogTels(string sentence);

    list getAddrs(string sentence);//识别结构体
    list getTels(string sentence);
private:
    ABU_ADDR_LIB_S *m_addrLib;
    ABU_NAME_LIB_S *m_nameLib;
    ABU_TEL_LIB_S *m_telLib;
    ABU_ADDR_RECOG_LIB_S *m_addrRecogLib;
    ABU_TEL_RECOG_LIB_S *m_telRecogLib;
};

class ABNAMESIMI
{
public:
    ABNAMESIMI(list dictList);
    ~ABNAMESIMI();
    //parse
    UINT32 appendParseResult(string name);
    UINT32 appendParseResultWithCity(string name, string city);
    bool isColl(UINT32 index_i, UINT32 index_j);
    unsigned int getSimi(UINT32 index_i, UINT32 index_j);
    struct ABName getParseResult(UINT32 index);
    void resetContainer() {
        p_container->resetContainer();
    }
    UINT32 getResultNum() {
        return p_container->getResultNum();
    }
    UINT32 getSize() {
        return p_container->getSize();
    }
    int releaseParseResult(UINT32 index) {
        return p_container->releaseItem(index);
    }
private:
    ABU_NAME_LIB_S *m_nameLib;
    AbuVector<ABU_NAME_S> *p_container;
};

class ABADDRSIMI
{
public:
    ABADDRSIMI(list dictList);
    ~ABADDRSIMI();
    //parse
    UINT32 appendParseResult(string addr);
    UINT32 appendParseResultWithCity(string addr, string city);
    bool isColl(UINT32 index_i, UINT32 index_j);
    unsigned int getSimi(UINT32 index_i, UINT32 index_j);
    struct ABAddress getParseResult(UINT32 index);
    void resetContainer() {
        p_container->resetContainer();
    }
    UINT32 getResultNum() {
        return p_container->getResultNum();
    }
    UINT32 getSize() {
        return p_container->getSize();
    }
    int releaseParseResult(UINT32 index) {
        return p_container->releaseItem(index);
    }
private:
    ABU_ADDR_LIB_S *m_addrLib;
    AbuVector<ABU_ADDR_S> *p_container;
};

class ABTELSIMI
{
public:
    ABTELSIMI(list dictList);
    ~ABTELSIMI();
    //parse
    UINT32 appendParseResult(string tel);
    bool isColl(UINT32 index_i, UINT32 index_j);
    unsigned int getSimi(UINT32 index_i, UINT32 index_j);
    void resetContainer() {
        p_container->resetContainer();
    }
    UINT32 getResultNum() {
        return p_container->getResultNum();
    }
    UINT32 getSize() {
        return p_container->getSize();
    }
    int releaseParseResult(UINT32 index) {
        return p_container->releaseItem(index);
    }
private:
    ABU_TEL_RECOG_LIB_S *m_telRecogLib;
    AbuVector<ABU_TEL_RECOG_S> *p_container;
};

struct ABAddress
{
	string province;
	string city;
    string county;
	string town;
	string street;
	string streetNO;
    int streetNODigit;
	string left;
    string brief;
	int detailLevel;
    int credit;

    string origin;
    string formatted;
};

struct ABName
{
    string province;
    string city;
    string county;

    string stemPre;    
    string stem;
    string stemPost;
    string type;
    string org;

    string assoPrefix;
    string assoSuffix;
    
    string appendix;
    
    //special info
    string minStem;
    string maxType;
    string bestType;    
    string multipleShop;
    string branchPlace;
};

struct ABTel
{
	string countryCode;
	string cityCode;
	string tel;
	string ext;

    string origin;
    string formatted;
};

#endif
