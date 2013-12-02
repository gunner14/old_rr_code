/**
 * @file SPCheck.h
 * @author dairui.cui@renren-inc.com
 * @brief some utility function used in this module
 */
#ifndef SPCHECKER_H_20120420
#define SPCHECKER_H_20120420

#include "abstring.h"
#include "abutility.h"
#include "name_util.h"
#include "AB_UTILITY_NAME.h"
#include "AB_UTILITY_NAME_RESOURCE.h"
#include "AB_UTILITY_ADDR.h"
#include "AB_UTILITY_ADDR_RESOURCE.h"
#include "AB_UTILITY_COMMON.h"
#include "AB_UTILITY_WORDS_UTILITY.h"
#include "HASHMAP.h"
#include "AB_UTILITY_WORDS_SEG_SL.h"

#include <string> 
#include <vector>
#include <iostream>
using namespace std;

class SPChecker
{
public:
    SPChecker();
    ~SPChecker();
    void Init();
    void Clear();
    void SetWorkPath(const string& strWorkPath);
    void ParseQueryInput(const string& strQueryInput, string& szResult);
    void ParseQueryByFile(const string& strFileIn,const string& strFileOut);
    void PrintSegResults(const string& szWord);
    void Preprocess(const string& strSrc,string& strDest);
    bool IsPlaceName(const string& strInput);
    bool IsSchool(const string& strInput,string& strSchool,string& strAppendix);
    void split(const string& s, char c, vector<string>& v);
    void splitSentence(vector<string>& vecDest,const vector<string>& vecSrc);

private:
    HashMap* m_stpWordMap;
    HashMap* m_stpShortTypeMap;
    ABU_NAME_LIB_S* m_stpNameLib;
    ABU_ADDR_LIB_S* m_stpAddrLib;
    string m_strWorkPath;
};
#endif
