/**
 *@file SPCheck.cpp
 *@author dairui.cui@renren-inc.com
 *@brief provided the School and place name entity recognize.
 */

#include "SPCheck.h"

static const char BASIC_DICT[] = "basicDict.txt";
static const char GROUP_DICT[] = "groupDict.txt";
static const char STEM_DICT[] = "merchant.stem.multiple";
static const char FEAT_DICT[] = "merchant.feat";
static const char PLACE_DICT[] = "place.dict";
static const char SCHOOL_SHORTNAME_DICT[] = "school.shortname";
static const char SCHOOL_SHORTTYPE_DICT[] = "school.shorttype";

static const char SCHOOL_TOKEN_FLAG[] = "/s";
static const char NAME_TOKEN_FLAG[] = "/n";
static const char COMPANY_TOKEN_FLAG[] = "/c";
static const char PLACE_TOKEN_FLAG[] = "/p";
static const char OTHER_TOKEN_FLAG[] = "/o";

SPChecker::SPChecker():m_stpWordMap(NULL),m_stpShortTypeMap(NULL),m_stpNameLib(NULL),m_stpAddrLib(NULL)
{

}

SPChecker::~SPChecker()
{
    Clear();
}

void SPChecker::SetWorkPath(const string& strWorkPath)
{
    m_strWorkPath = strWorkPath;
}

void SPChecker::Init()
{
    const char *szDictArray[5] = {BASIC_DICT, GROUP_DICT,STEM_DICT, FEAT_DICT, PLACE_DICT};

    string szBasicDict = m_strWorkPath + "/" + BASIC_DICT;
    string szGroupDict = m_strWorkPath + "/" + GROUP_DICT;
    string szShortDict = m_strWorkPath + "/" + SCHOOL_SHORTNAME_DICT;
    string szSTypeDict = m_strWorkPath + "/" + SCHOOL_SHORTTYPE_DICT;

    const char *szAddrDictArray[2] = {szBasicDict.c_str(), szGroupDict.c_str()};

    m_stpNameLib = ABU_NAME_createLib((const char *)m_strWorkPath.c_str(), szDictArray, 5);

    if(m_stpNameLib == NULL)
    {
        cout<<"can't create name lib for parsing"<<endl;
        exit(-1);
    }
    
    m_stpAddrLib = ABU_ADDR_createLib((const char **)szAddrDictArray,2);

    if(m_stpAddrLib == NULL)
    {
        cout<<"can't create addr lib for parsing"<<endl;
        exit(-1);
    }

    FILE* fpStem = fopen(szShortDict.c_str(),"r");
    if (NULL == fpStem)
    {
        cout<<"open "<<szShortDict<<" error"<<endl;
        exit(-1);
    }
    m_stpWordMap = createHashMap(10240);
    char szWord[50];
    while(!feof(fpStem))
    {
        memset(szWord,0,sizeof(szWord));
        fgets(szWord,sizeof(szWord),fpStem);
        abstru_rstrip(szWord);
        insertHashMapNode(m_stpWordMap,szWord,"1");
    }
    if(fpStem != NULL)
    {
        fclose(fpStem);
        fpStem = NULL;
    }
    FILE* fpSType = fopen(szSTypeDict.c_str(),"r");
    if (NULL == fpSType)
    {
        cout << "open " << szSTypeDict << "error" << endl;
        exit(-1);
    }
    m_stpShortTypeMap = createHashMap(1000);
    char szSType[40];
    while(!feof(fpSType))
    {
        memset(szSType,0,sizeof(szSType));
        fgets(szSType,sizeof(szSType),fpSType);
        abstru_rstrip(szSType);
        insertHashMapNode(m_stpShortTypeMap,szSType,"1");
    }
    if(fpSType != NULL)
    {
        fclose(fpSType);
        fpSType = NULL;
    }
}

void SPChecker::Clear()
{
    ABU_NAME_destroyLib(m_stpNameLib);
    ABU_ADDR_destroyLib(m_stpAddrLib);
    destroyHashMap(m_stpWordMap);
    destroyHashMap(m_stpShortTypeMap);
}

bool  SPChecker::IsPlaceName(const string& szWord)
{
    bool bFlag = false;
    ABU_ADDR_RECORD_S *stpAddrRecord= ABU_ADDR_parseRecord(m_stpAddrLib,szWord.c_str());
    if(stpAddrRecord != NULL)
    {
        char* szProvince = stpAddrRecord->szProvince;
        char* szCity = stpAddrRecord->szCity;
        char* szCounty = stpAddrRecord->szCounty;
        char* szTown = stpAddrRecord->szTown;
        char* szStreet = stpAddrRecord->szStreet;
        char* szStreetNo = stpAddrRecord->szStreetNO;
        char* szAddrLeft = stpAddrRecord->szAddrLeft;
        if ((NULL != szProvince || NULL != szCity || NULL != szCounty) && (NULL == szTown && NULL == szStreet && NULL == szStreetNo && NULL == szAddrLeft))
        {
            bFlag = true;
        }
    }
    
    ABU_ADDR_destroyRecord(stpAddrRecord);
    return bFlag;
}

bool SPChecker::IsSchool(const string& szWord, string& szSchool,string& szAppendix)
{
    bool bFlag = false;
    int wordLen = szWord.length();
    if (wordLen == 0)
    {
        return bFlag;
    }
   
    szSchool = "";
    szAppendix = ""; 

    ABU_NAME_RECORD_S *stpNameRecord= ABU_NAME_parseRecord_bycity(m_stpNameLib,szWord.c_str(),"");
    if(stpNameRecord != NULL)
    {
        if(NULL != stpNameRecord->szBestType) 
        {
            bFlag = true;
        }

        if (NULL != getValueHashMap(m_stpWordMap,szWord.c_str()))
        {
            bFlag = true;
        }

        if(bFlag == false)
        {
            /*deal PlaceName + shortType cases. jiangxicaijing*/
            if ((NULL != stpNameRecord->szProvince || NULL != stpNameRecord->szCity || NULL != stpNameRecord->szCounty) && NULL != stpNameRecord->szStem)
            {
                if(NULL != getValueHashMap(m_stpShortTypeMap,stpNameRecord->szStem))
                {
                    bFlag = true;
                }
            }
        }
        else
        {
            if (NULL != stpNameRecord->szAppendix)
            {
                szAppendix = stpNameRecord->szAppendix;
                szSchool = szWord.substr(0,wordLen - szAppendix.length());
            }
            else
            {
                szSchool = szWord;
            }
        }
    }
    ABU_NAME_destroyRecord(stpNameRecord);
    return bFlag;
}

void SPChecker::PrintSegResults(const string& szWord)
{
    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = NULL;
    int nameLen = szWord.length();
    stpSegResult = ab_segmenter_seg(szWord.c_str(),nameLen,m_stpNameLib->stpSegDict,m_stpNameLib->stpSegLib);
    if (NULL != stpSegResult)
    {
        char * szSegResult = ABU_WORDS_SEG_SL_getSegResult(stpSegResult);
        cout << "seg results:" << szSegResult;
        ab_segmenter_free(stpSegResult);
        ABU_FREE(szSegResult);
        stpSegResult = NULL;
    }
}

void SPChecker::split(const string& s, char c, vector<string>& v)
{
    string::size_type i = 0;
    string::size_type j = s.find(c); 
    while(j != string::npos)
    { 
        v.push_back(s.substr(i, j-i));
        i = ++j;
        j = s.find(c, j);
    }//find start from position j
}
void SPChecker::Preprocess(const string& strSrc,string& strDest)
{
    int QueryLen = strSrc.length();
    char* szTemp = (char*)calloc(QueryLen * 2,sizeof(char));
    memcpy(szTemp,strSrc.c_str(),QueryLen);
    abstru_remove_badcode_gb(szTemp);
    abstru_remove_punct(szTemp);
    if (0 == ABU_WORDS_PREPROCESS_all(szTemp))
    {
        strDest = szTemp;
    }
    else
    {
        strDest = strSrc;
    }
    ABU_FREE(szTemp);
}

void SPChecker::splitSentence(vector<string> & vecDest,const vector<string> & vecSrc)
{
    int nSrcSize = vecSrc.size();
    string strSrc;

    ABU_WORDS_SEGRESULT_SL_S *stpSegResult = NULL;
    for(int i = 0;i < nSrcSize;i++)
    {
        strSrc = vecSrc[i];
        stpSegResult = ab_segmenter_seg(strSrc.c_str(),strSrc.length(),m_stpNameLib->stpSegDict,m_stpNameLib->stpSegLib);
        if(stpSegResult != NULL)
        {
            bool bFind = false;    
            int nOffset = 0;
            for(int32_t j = 0;j < stpSegResult->dwWordNumber;j++)
            {
                ABU_WORDS_WORD_S *stpWord = stpSegResult->pWordArray + j;
                if(NULL != stpWord)
                {
                    if(j > 0 && nOffset >= 4 
                            && (ABU_WORDS_UTILITY_isProvince(stpWord) ||ABU_WORDS_UTILITY_isCity(stpWord) || ABU_WORDS_UTILITY_isCounty(stpWord)) )
                    {
                        bFind = true;
                        break;
                    }
                    nOffset += strlen(stpWord->szWord);
                }
            }
            if(bFind == true)
            {
                string strFirst = strSrc.substr(0,nOffset);
                string strSecond = strSrc.substr(nOffset,strSrc.length());
                vecDest.push_back(strFirst);
                vecDest.push_back(strSecond);
            }
            else
            {
                vecDest.push_back(strSrc);
            }
        }
        ab_segmenter_free(stpSegResult);
        stpSegResult = NULL;
    } 
}

void SPChecker::ParseQueryInput(const string& szQueryInput,string& szResult)
{
    szResult = "";
    vector<string> vecToken;
    string szQuery = "";
    Preprocess(szQueryInput,szQuery);
    szQuery += " ";
    split(szQuery,' ',vecToken);
    int nSize = vecToken.size();
    /*first of all,split the query input string by space*/
    for(int i = 0;i < nSize;i++)
    {
        string szToken = vecToken[i];
        if (szToken == " ")
        {
            continue;
        }
        string szSchool;
        string szAppendix;
        bool bIsSchool = IsSchool(szToken,szSchool,szAppendix);
        if(true == bIsSchool)
        {
            if(szAppendix.length() > 0)
            {
                szResult += szSchool;
                szResult += SCHOOL_TOKEN_FLAG;
                szResult += " ";
                szResult += szAppendix;
                szResult += OTHER_TOKEN_FLAG;
                szResult += " ";
            }
            else
            {
                szResult += szToken;
                szResult += SCHOOL_TOKEN_FLAG;
                szResult += " ";
            }
        }

        bool bIsPlaceName = IsPlaceName(szToken);
        if(true == bIsPlaceName)
        {
            szResult += szToken;
            szResult += PLACE_TOKEN_FLAG;
            szResult += " ";
        }

        if(false == bIsPlaceName && false == bIsSchool)
        {
            szResult += szToken;
            szResult += OTHER_TOKEN_FLAG;
            szResult += " ";
        }
    }
    szResult = szResult.substr(0,szResult.length() - 1);
}

void SPChecker::ParseQueryByFile(const string& strFileIn,const string& strFileOut)
{
    char szName[10240];
    int nameLen = 0;
    int lines = 0;
    bool newLine = true;
    FILE* fpIn = fopen(strFileIn.c_str(),"r");
    FILE* fpOut= fopen(strFileOut.c_str(),"w");
    while(!feof(fpIn))
    {
        memset(szName,0,sizeof(szName));
        fgets(szName,sizeof(szName),fpIn);
        
        nameLen = strlen(szName);
        if(nameLen == 0)
            break;

        if(szName[nameLen - 1] != '\n')
        {
            if(newLine == true)
            {
                newLine = false;
                continue;
            }
            else
            {
                continue;
            }
        }
        else if(newLine == false)
        {
            newLine = true;
            continue;
        }
        
        lines += 1;
        if(lines % 100000 == 0)
        {
            cout<<lines<<"-";
        }

        if(nameLen > 512)
        {
            cout<<"!!!can't parse " << lines << "longer than 512 bytes" << endl;
            continue;
        }

        szName[nameLen - 1] = '\0';        
        if(szName[0] == '\0')
        {
            continue;
        }
        string szResult;
        ParseQueryInput(szName,szResult);
        fprintf(fpOut,"parse:%s\n",szResult.c_str());
    }

    if(NULL != fpIn)
    {
        fclose(fpIn);
        fpIn = NULL;
    }

    if(NULL != fpOut)
    {
        fclose(fpOut);
        fpOut = NULL;
    }
}

